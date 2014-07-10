#include "State.hpp"
#include "Any.hpp"
#include "Assembly.hpp"
#include "ClassBase.hpp"
#include "MetaProvider.ipp"
#include "values.ipp"
#include "../../Exception.hpp"
#include <mono/jit/jit.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/assembly.h>
#ifdef _DEBUG
#include <mono/metadata/mono-debug.h>
#endif

BEGIN_INANITY_MONO

State* State::instance = nullptr;

State::State(const String& baseAssemblyFileName) :
	domain(nullptr),
	baseImage(nullptr),
	baseClass(nullptr),
	baseClassPointerField(nullptr),
	referenceQueue(nullptr)
{
	BEGIN_TRY();

	instance = this;

	// init domain
	domain = mono_jit_init_version("inanity", "v4.0.30319");
	if(!domain)
		THROW("Can't initialize Mono JIT");

#ifdef _DEBUG
	static const char* options[] = {
		"--soft-breakpoints",
		"--debugger-agent=transport=dt_socket,address=127.0.0.1:10000"
	};
	mono_jit_parse_options(sizeof(options) / sizeof(const char*), (char**)options);

	mono_debug_domain_create(domain);
	mono_debug_init(MONO_DEBUG_FORMAT_MONO);
#endif

	// load base assembly
	if(!baseAssemblyFileName.empty())
	{
		MonoImageOpenStatus status;
		MonoAssembly* assembly = mono_assembly_open(baseAssemblyFileName.c_str(), &status);
		if(!assembly || status != MONO_IMAGE_OK)
			THROW(String("Can't load base assembly: ") + mono_image_strerror(status));

		baseImage = mono_assembly_get_image(assembly);

		// init base class
		baseClass = mono_class_from_name(baseImage, "Inanity", "Object");
		if(!baseClass)
			THROW("Can't get base class");
		baseClassPointerField = mono_class_get_field_from_name(baseClass, "ptr");
		if(!baseClassPointerField)
			THROW("Can't get base class pointer field");

		// create reference queue
		referenceQueue = mono_gc_reference_queue_new(&ReferenceQueueCallback);
		if(!referenceQueue)
			THROW("Can't create GC reference queue");

		// create any pool
		anyPool = NEW(ObjectPool<Any>());
	}
	else
	{
		// generation mode
		baseClass = nullptr;
		baseClassPointerField = nullptr;
	}

	END_TRY("Can't create Mono script state");
}

State::~State()
{
	instance = nullptr;

	for(Instances::const_iterator i = instances.begin(); i != instances.end(); ++i)
		ReclaimInstanceObject(i->second);
	instances.clear();

	if(referenceQueue)
		mono_gc_reference_queue_free(referenceQueue);

	mono_jit_cleanup(domain);
}

MonoDomain* State::GetDomain() const
{
	return domain;
}

MonoImage* State::GetBaseImage() const
{
	return baseImage;
}

ptr<Assembly> State::LoadAssembly(const String& fileName)
{
	BEGIN_TRY();

	MonoImageOpenStatus status;
	MonoAssembly* assembly = mono_assembly_open(fileName.c_str(), &status);
	if(!assembly || status != MONO_IMAGE_OK)
		THROW(String("Can't load assembly ") + mono_image_strerror(status));

	return NEW(Assembly(this, assembly));

	END_TRY("Can't load Mono assembly");
}

ptr<Function> State::LoadScript(ptr<File> file)
{
	THROW("Mono state doesn't support loading scripts");
}

void State::ReferenceQueueCallback(void* data)
{
	State::instance->ObjectCollected((uint32_t)data);
}

void State::ObjectCollected(uint32_t gcHandle)
{
	// find handle
	InstancesByHandle::iterator i = instancesByHandle.find(gcHandle);
	if(i == instancesByHandle.end())
		return;

	// get object address
	RefCounted* object = i->second;

	// free handle
	ReclaimInstanceObject(gcHandle);

	// remove object from instances
	std::pair<Instances::iterator, Instances::iterator> range = instances.equal_range(object);
	for(Instances::iterator j = range.first; j != range.second; ++j)
		if(j->second == gcHandle)
		{
			instances.erase(j);
			break;
		}
}

void State::ReclaimInstanceObject(uint32_t gcHandle)
{
	MonoObject* monoObject = mono_gchandle_get_target(gcHandle);
	RefCounted* nullValue = nullptr;
	if(monoObject)
		mono_field_set_value(monoObject, baseClassPointerField, &nullValue);
	mono_gchandle_free(gcHandle);
	instancesByHandle.erase(gcHandle);
}

void State::ReclaimInstance(RefCounted* object)
{
	std::pair<Instances::iterator, Instances::iterator> range = instances.equal_range(object);
	for(Instances::iterator i = range.first; i != range.second; ++i)
		ReclaimInstanceObject(i->second);
	instances.erase(range.first, range.second);
}

ptr<Script::Any> State::NewBoolean(bool boolean)
{
	MonoBoolean b = boolean;
	return CreateAny(mono_value_box(domain, mono_get_boolean_class(), &b));
}

ptr<Script::Any> State::NewNumber(int number)
{
	return CreateAny(mono_value_box(domain, mono_get_int32_class(), &number));
}

ptr<Script::Any> State::NewNumber(float number)
{
	return CreateAny(mono_value_box(domain, mono_get_single_class(), &number));
}

ptr<Script::Any> State::NewNumber(double number)
{
	return CreateAny(mono_value_box(domain, mono_get_double_class(), &number));
}

ptr<Script::Any> State::NewString(const String& string)
{
	return CreateAny((MonoObject*)Value<String>::To(string));
}

ptr<Script::Any> State::NewArray(int length)
{
	return CreateAny((MonoObject*)mono_array_new(domain, mono_get_object_class(), length));
}

ptr<Script::Any> State::NewDict()
{
	THROW("Not implemented yet");
}

void State::Register(MetaProvider::ClassBase* classMeta)
{
	classes.push_back(classMeta);
}

ptr<Any> State::CreateAny(MonoObject* object)
{
	return anyPool->New(this, object);
}

bool State::IsObjectWrapper(MonoObject* value)
{
	return mono_object_isinst(value, baseClass);
}

RefCounted* State::UnwrapObject(MonoObject* value)
{
	if(!value)
		return nullptr;

	// get a value of pointer field
	RefCounted* pointer;
	mono_field_get_value(value, baseClassPointerField, &pointer);
	return pointer;
}

void State::SetObjectIntoWrapper(MonoObject* monoObject, RefCounted* object)
{
	mono_field_set_value(monoObject, baseClassPointerField, &object);
}

MonoObject* State::ConvertObject(MetaProvider::ClassBase* classMeta, RefCounted* object)
{
	MonoClass* monoClass = classMeta->GetMonoClass();

	// check if this object already converted
	std::pair<Instances::iterator, Instances::iterator> range = instances.equal_range(object);
	for(Instances::iterator i = range.first; i != range.second; ++i)
	{
		MonoObject* monoObject = mono_gchandle_get_target(i->second);
		if(!monoObject)
			continue;
		if(monoClass == mono_object_get_class(monoObject))
			// found an object
			return monoObject;
	}

	// object not converted yet, create new Mono object
	MonoObject* monoObject = mono_object_new(domain, monoClass);
	mono_runtime_object_init(monoObject);
	// store a pointer into field
	SetObjectIntoWrapper(monoObject, object);

	// create weak gc handle
	uint32_t gcHandle = mono_gchandle_new_weakref(monoObject, true);

	// save an instance
	instances.insert(std::make_pair(object, gcHandle));
	instancesByHandle.insert(std::make_pair(gcHandle, object));

	return monoObject;
}

END_INANITY_MONO
