#include "State.hpp"
#include "values.ipp"
#include "wrappers.hpp"
#include "MetaProvider.ipp"
#include "Namespace.hpp"
#include "../../platform/NpapiPluginInstance.hpp"
#include "../../File.hpp"
#include "../../Exception.hpp"

BEGIN_INANITY_NP

State* State::current = nullptr;

State::State(Platform::NpapiPluginInstance* pluginInstance)
: pluginInstance(pluginInstance)
{
	if(!current)
		current = this;

	// create pools
	npObjectWrapperPool = NEW(TypedPool<NPObjectWrapper>());
	npObjectNamespacePool = NEW(TypedPool<NPObjectNamespace>());
	anyPool = NEW(ObjectPool<Any>());

	rootNamespace = NEW(Namespace(""));
}

State::~State()
{
	if(current == this)
		current = nullptr;
}

State* State::GetCurrent()
{
	return current;
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
	std::pair<Instances::iterator, Instances::iterator> range = instances.equal_range(wrapper->object);
	for(Instances::iterator i = range.first; i != range.second; ++i)
		if(i->second == wrapper)
		{
			instances.erase(i);
			wrapper->object = nullptr;
			break;
		}
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
	Platform::NpapiPlugin::browserFuncs.retainobject(wrapper);
	NPVariant variant;
	OBJECT_TO_NPVARIANT(wrapper, variant);
	return CreateAny(variant);
}

ptr<Any> State::CreateAny(NPVariant variant)
{
	return anyPool->New(this, variant);
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
			Platform::NpapiPlugin::browserFuncs.retainobject(i->second);
			NPVariant variant;
			OBJECT_TO_NPVARIANT(i->second, variant);
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
