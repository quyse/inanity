#include "State.hpp"
#include "values.ipp"
#include "wrappers.hpp"
#include "MetaProvider.ipp"
#include "Namespace.hpp"
#include "../../platform/NpapiPluginInstance.hpp"
#include "../../File.hpp"
#include "../../Exception.hpp"

BEGIN_INANITY_NP

#ifdef _MSC_VER
__declspec(thread) State* currentState = nullptr;
__declspec(thread) int currentStateCounter = 0;
#else
thread_local State* currentState = nullptr;
thread_local int currentStateCounter = 0;
#endif

State::Scope::Scope(State* state)
{
	if(currentStateCounter)
	{
		if(currentState != state)
			THROW("Current NPAPI script state already set");
	}
	else
		currentState = state;
	++currentStateCounter;
}

State::Scope::~Scope()
{
	if(!--currentStateCounter)
		currentState = nullptr;
}

State::State(ptr<Platform::NpapiPluginInstance> pluginInstance)
: pluginInstance(pluginInstance)
{
	// create pools
	npObjectWrapperPool = NEW(TypedPool<NPObjectWrapper>());
	npObjectNamespacePool = NEW(TypedPool<NPObjectNamespace>());
	anyPool = NEW(ObjectPool<Any>());

	Scope scope(this);
	rootNamespace = NEW(Namespace(""));
}

State* State::GetCurrent()
{
#ifdef _DEBUG
	if(!currentState)
		THROW("No current NPAPI script state");
#endif
	return currentState;
}

Platform::NpapiPluginInstance* State::GetPluginInstance() const
{
	return pluginInstance;
}

ptr<Function> State::LoadScript(ptr<File> file)
{
	THROW("Not implemented");
}

void State::ReclaimInstance(RefCounted* object)
{
	std::pair<Instances::iterator, Instances::iterator> range = instances.equal_range(object);
	for(Instances::iterator i = range.first; i != range.second; ++i)
		i->second->object = nullptr;
	instances.erase(range.first, range.second);
}

ptr<Script::Any> State::NewBoolean(bool boolean)
{
	return CreateAny(Value<bool>::To(boolean));
}

ptr<Script::Any> State::NewNumber(int number)
{
	return CreateAny(Value<int>::To(number));
}

ptr<Script::Any> State::NewNumber(float number)
{
	return CreateAny(Value<float>::To(number));
}

ptr<Script::Any> State::NewNumber(double number)
{
	return CreateAny(Value<double>::To(number));
}

ptr<Script::Any> State::NewString(const String& string)
{
	return CreateAny(Value<String>::To(string));
}

ptr<Script::Any> State::NewArray(int length)
{
	THROW("Not implemented");
}

ptr<Script::Any> State::NewDict()
{
	THROW("Not implemented");
}

void State::Register(MetaProvider::ClassBase* classMeta)
{
	Scope scope(this);

	const char* name = classMeta->GetFullName();
	// loop for namespaces
	ptr<Namespace> nm = rootNamespace;
	for(;;)
	{
		// get next word
		int i;
		for(i = 0; name[i] && name[i] != '.'; ++i);
		String word(name, i);

		// open namespace
		nm = nm->OpenNamespace(word);

		// if it's the end
		if(!name[i])
		{
			// set class there
			nm->SetClassMeta(classMeta);
			break;
		}

		// else it's not the end
		name += i + 1;
	}
}

bool State::CheckClass(NPClass* npClass) const
{
	return classes.find(npClass) != classes.end();
}

NPObjectWrapper* State::CreateNPObjectWrapper()
{
	return npObjectWrapperPool->New(this);
}

void State::InvalidateNPObjectWrapper(NPObjectWrapper* wrapper)
{
	if(!wrapper->object)
		return;

	std::pair<Instances::iterator, Instances::iterator> range = instances.equal_range(wrapper->object);
	for(Instances::iterator i = range.first; i != range.second; ++i)
		if(i->second == wrapper)
		{
			instances.erase(i);
			wrapper->object = nullptr;
			return;
		}

	THROW("No NPObject to invalidate");
}

void State::DeleteNPObjectWrapper(NPObjectWrapper* wrapper)
{
	npObjectWrapperPool->Delete(wrapper);
}

NPObjectNamespace* State::CreateNPObjectNamespace()
{
	return npObjectNamespacePool->New(this);
}

void State::DeleteNPObjectNamespace(NPObjectNamespace* wrapper)
{
	npObjectNamespacePool->Delete(wrapper);
}

ptr<Any> State::GetRootNamespace()
{
	NPObjectNamespace* wrapper = rootNamespace->GetWrapper();
	NPVariant variant;
	OBJECT_TO_NPVARIANT(wrapper, variant);
	return CreateAnyViaCopy(variant);
}

void State::DuplicateVariant(NPVariant& variant)
{
	if(NPVARIANT_IS_OBJECT(variant))
		Platform::NpapiPlugin::browserFuncs.retainobject(NPVARIANT_TO_OBJECT(variant));
	else if(NPVARIANT_IS_STRING(variant))
	{
		// copy string
		NPString string = NPVARIANT_TO_STRING(variant);
		NPUTF8* stringCopy = (NPUTF8*)Platform::NpapiPlugin::browserFuncs.memalloc(string.UTF8Length);
		memcpy(stringCopy, string.UTF8Characters, string.UTF8Length);
		STRINGN_TO_NPVARIANT(stringCopy, string.UTF8Length, variant);
	}
}

ptr<Any> State::CreateAny(NPVariant variant)
{
	return anyPool->New(this, variant);
}

ptr<Any> State::CreateAnyViaCopy(NPVariant variant)
{
	DuplicateVariant(variant);
	return CreateAny(variant);
}

NPVariant State::ConvertObject(MetaProvider::ClassBase* classMeta, RefCounted* object)
{
	// if object is null
	if(!object)
	{
		// return null
		NPVariant variant;
		NULL_TO_NPVARIANT(variant);
		return variant;
	}

	// check if the object is already in cache
	std::pair<Instances::const_iterator, Instances::const_iterator> range = instances.equal_range(object);
	// find an object with right meta
	for(Instances::const_iterator i = range.first; i != range.second; ++i)
		if(i->second->GetClassWrapper()->classMeta == classMeta)
		{
			// found an object
			NPVariant variant;
			OBJECT_TO_NPVARIANT(i->second, variant);
			DuplicateVariant(variant);
			return variant;
		}

	// create new object
	NPObject* npObject = Platform::NpapiPlugin::browserFuncs.createobject(
		pluginInstance->GetNpp(),
		classMeta->GetClassWrapper());
	NPObjectWrapper* wrapper = static_cast<NPObjectWrapper*>(npObject);
	wrapper->object = object;

	// ensure that class in classes
	classes.insert(classMeta->GetClassWrapper());

	// save it to cache
	instances.insert(std::make_pair(object, wrapper));

	// return
	NPVariant variant;
	OBJECT_TO_NPVARIANT(wrapper, variant);
	return variant;
}

END_INANITY_NP
