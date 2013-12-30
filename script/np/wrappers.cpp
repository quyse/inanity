#include "wrappers.hpp"
#include "State.hpp"
#include "MetaProvider.ipp"
#include "../../platform/NpapiPlugin.hpp"

BEGIN_INANITY_NP

//*** class NPObjectWrapper

NPObjectWrapper::NPObjectWrapper(ptr<State> state)
: state(state) {}

NPClassWrapper* NPObjectWrapper::GetClassWrapper() const
{
	return static_cast<NPClassWrapper*>(_class);
}

//*** class NPClassWrapper

NPObject* NPClassWrapper::npAllocate(NPP npp, NPClass *aClass)
{
	return State::GetCurrent()->CreateNPObjectWrapper();
}

void NPClassWrapper::npDeallocate(NPObject *npobj)
{
	NPObjectWrapper* wrapper = static_cast<NPObjectWrapper*>(npobj);
	wrapper->state->DeleteNPObjectWrapper(wrapper);
}

void NPClassWrapper::npInvalidate(NPObject *npobj)
{
	NPObjectWrapper* wrapper = static_cast<NPObjectWrapper*>(npobj);
	wrapper->state->InvalidateNPObjectWrapper(wrapper);
}

bool NPClassWrapper::npHasMethod(NPObject *npobj, NPIdentifier name)
{
	MetaProvider::ClassBase* classMeta = static_cast<NPObjectWrapper*>(npobj)->GetClassWrapper()->classMeta;

	// upcasting loop
	for(MetaProvider::ClassBase* cls = classMeta; cls; cls = cls->GetParent())
	{
		const MetaProvider::ClassBase::MethodsByIdentifier& methodsByIdentifier = cls->GetMethodsByIdentifier();
		if(methodsByIdentifier.find(name) != methodsByIdentifier.end())
			return true;
	}

	return false;
}

bool NPClassWrapper::npInvoke(NPObject *npobj, NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	NPObjectWrapper* objectWrapper = static_cast<NPObjectWrapper*>(npobj);
	MetaProvider::ClassBase* classMeta = objectWrapper->GetClassWrapper()->classMeta;

	// upcasting loop
	for(MetaProvider::ClassBase* cls = classMeta; cls; cls = cls->GetParent())
	{
		const MetaProvider::ClassBase::MethodsByIdentifier& methodsByIdentifier = cls->GetMethodsByIdentifier();
		MetaProvider::ClassBase::MethodsByIdentifier::const_iterator it = methodsByIdentifier.find(name);
		if(it != methodsByIdentifier.end())
			return cls->GetMethods()[it->second]->GetThunk()(objectWrapper, args, (int)argCount, result);
	}

	return false;
}

bool NPClassWrapper::npInvokeDefault(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	// we don't support functions yet
	return false;
}

bool NPClassWrapper::npHasProperty(NPObject *npobj, NPIdentifier name)
{
	// we don't support properties yet
	return false;
}

bool NPClassWrapper::npGetProperty(NPObject *npobj, NPIdentifier name, NPVariant *result)
{
	// we don't support properties yet
	return false;
}

bool NPClassWrapper::npSetProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value)
{
	// we don't support properties yet
	return false;
}

bool NPClassWrapper::npRemoveProperty(NPObject *npobj, NPIdentifier name)
{
	// we don't support properties yet
	return false;
}

bool NPClassWrapper::npEnumerate(NPObject *npobj, NPIdentifier **value, uint32_t *count)
{
	NPObjectWrapper* objectWrapper = static_cast<NPObjectWrapper*>(npobj);
	MetaProvider::ClassBase* classMeta = objectWrapper->GetClassWrapper()->classMeta;

	const MetaProvider::ClassBase::Methods& methods = classMeta->GetMethods();
	NPIdentifier* identifiers = (NPIdentifier*)Platform::NpapiPlugin::browserFuncs.memalloc(sizeof(NPIdentifier) * methods.size());
	for(size_t i = 0; i < methods.size(); ++i)
		identifiers[i] = Platform::NpapiPlugin::browserFuncs.getstringidentifier(methods[i]->GetName());
	return true;
}

bool NPClassWrapper::npConstruct(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	// FIX ME: we have to support this, in order to work with constructors
	// in this method npobj will be constructor which called with javascript 'new' operator
	return false;
}

NPClassWrapper::NPClassWrapper(MetaProvider::ClassBase* classMeta)
: classMeta(classMeta)
{
	// fill NPClass fields
	structVersion = NP_CLASS_STRUCT_VERSION;
	allocate = &npAllocate;
	deallocate = &npDeallocate;
	invalidate = &npInvalidate;
	hasMethod = &npHasMethod;
	invoke = &npInvoke;
	invokeDefault = &npInvokeDefault;
	hasProperty = &npHasProperty;
	getProperty = &npGetProperty;
	setProperty = &npSetProperty;
	removeProperty = &npRemoveProperty;
	enumerate = &npEnumerate;
	construct = &npConstruct;
}

END_INANITY_NP
