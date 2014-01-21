#include "wrappers.hpp"
#include "State.hpp"
#include "MetaProvider.ipp"
#include "Namespace.hpp"
#include "../../platform/NpapiPlugin.hpp"

BEGIN_INANITY_NP

//*** struct NPObjectWrapper

NPObjectWrapper::NPObjectWrapper(ptr<State> state)
: state(state) {}

NPClassWrapper* NPObjectWrapper::GetClassWrapper() const
{
	return static_cast<NPClassWrapper*>(_class);
}

//*** struct NPClassWrapper

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
		const MetaProvider::ClassBase::IdentifierMap& methods = cls->GetMethodsByIdentifier();
		if(methods.find(name) != methods.end())
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
		const MetaProvider::ClassBase::IdentifierMap& methods = cls->GetMethodsByIdentifier();
		MetaProvider::ClassBase::IdentifierMap::const_iterator i = methods.find(name);
		if(i != methods.end())
			return cls->GetMethods()[i->second]->GetThunk()(objectWrapper, args, (int)argCount, result);
	}

	return false;
}

bool NPClassWrapper::npInvokeDefault(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	// we don't support calling object as function
	return false;
}

bool NPClassWrapper::npHasProperty(NPObject *npobj, NPIdentifier name)
{
	// we don't support properties
	return false;
}

bool NPClassWrapper::npGetProperty(NPObject *npobj, NPIdentifier name, NPVariant *result)
{
	// we don't support properties
	return false;
}

bool NPClassWrapper::npSetProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value)
{
	// we don't support properties
	return false;
}

bool NPClassWrapper::npRemoveProperty(NPObject *npobj, NPIdentifier name)
{
	// we don't support properties
	return false;
}

bool NPClassWrapper::npEnumerate(NPObject *npobj, NPIdentifier **value, uint32_t *count)
{
	NPObjectWrapper* objectWrapper = static_cast<NPObjectWrapper*>(npobj);
	MetaProvider::ClassBase* classMeta = objectWrapper->GetClassWrapper()->classMeta;

	const MetaProvider::ClassBase::IdentifierMap& methods = classMeta->GetMethodsByIdentifier();
	NPIdentifier* identifiers = (NPIdentifier*)Platform::NpapiPlugin::browserFuncs.memalloc(sizeof(NPIdentifier) * methods.size());
	int j = 0;
	for(MetaProvider::ClassBase::IdentifierMap::const_iterator i = methods.begin(); i != methods.end(); ++i)
		identifiers[j++] = i->first;

	*value = identifiers;
	*count = (uint32_t)methods.size();
	return true;
}

bool NPClassWrapper::npConstruct(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	// objects can't be called as constructors
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

//*** struct NPObjectNamespace

NPObjectNamespace::NPObjectNamespace(ptr<State> state)
: state(state) {}

//*** struct NPClassNamespace

NPClassNamespace NPClassNamespace::instance;

NPObject* NPClassNamespace::npAllocate(NPP npp, NPClass *aClass)
{
	return State::GetCurrent()->CreateNPObjectNamespace();
}

void NPClassNamespace::npDeallocate(NPObject *npobj)
{
	NPObjectNamespace* wrapper = static_cast<NPObjectNamespace*>(npobj);
	wrapper->state->DeleteNPObjectNamespace(wrapper);
}

void NPClassNamespace::npInvalidate(NPObject *npobj)
{
	NPObjectNamespace* wrapper = static_cast<NPObjectNamespace*>(npobj);
	wrapper->object = nullptr;
}

bool NPClassNamespace::npHasMethod(NPObject *npobj, NPIdentifier name)
{
	MetaProvider::ClassBase* classMeta = static_cast<NPObjectNamespace*>(npobj)->object->GetClassMeta();
	if(!classMeta)
		return false;

	// upcasting loop
	for(MetaProvider::ClassBase* cls = classMeta; cls; cls = cls->GetParent())
	{
		const MetaProvider::ClassBase::IdentifierMap& staticMethods = cls->GetStaticMethodsByIdentifier();
		if(staticMethods.find(name) != staticMethods.end())
			return true;
	}

	return false;
}

bool NPClassNamespace::npInvoke(NPObject *npobj, NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	MetaProvider::ClassBase* classMeta = static_cast<NPObjectNamespace*>(npobj)->object->GetClassMeta();
	if(!classMeta)
		return false;

	// upcasting loop
	for(MetaProvider::ClassBase* cls = classMeta; cls; cls = cls->GetParent())
	{
		const MetaProvider::ClassBase::IdentifierMap& staticMethods = cls->GetStaticMethodsByIdentifier();
		MetaProvider::ClassBase::IdentifierMap::const_iterator i = staticMethods.find(name);
		if(i != staticMethods.end())
			return cls->GetStaticMethods()[i->second]->GetThunk()(args, (int)argCount, result);
	}

	return false;
}

bool NPClassNamespace::npInvokeDefault(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	// namespaces can't be called as function
	return false;
}

bool NPClassNamespace::npHasProperty(NPObject *npobj, NPIdentifier name)
{
	NPObjectNamespace* wrapper = static_cast<NPObjectNamespace*>(npobj);
	const Namespace::Namespaces& namespaces = wrapper->object->GetNamespaces();
	return namespaces.find(name) != namespaces.end();
}

bool NPClassNamespace::npGetProperty(NPObject *npobj, NPIdentifier name, NPVariant *result)
{
	NPObjectNamespace* wrapper = static_cast<NPObjectNamespace*>(npobj);
	const Namespace::Namespaces& namespaces = wrapper->object->GetNamespaces();
	Namespace::Namespaces::const_iterator i = namespaces.find(name);
	if(i != namespaces.end())
	{
		NPObjectNamespace* propertyWrapper = i->second->GetWrapper();
		Platform::NpapiPlugin::browserFuncs.retainobject(propertyWrapper);
		OBJECT_TO_NPVARIANT(propertyWrapper, *result);
		return true;
	}
	return false;
}

bool NPClassNamespace::npSetProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value)
{
	// don't allow setting properties on namespaces
	return false;
}

bool NPClassNamespace::npRemoveProperty(NPObject *npobj, NPIdentifier name)
{
	// don't allow removing properties from namespaces
	return false;
}

bool NPClassNamespace::npEnumerate(NPObject *npobj, NPIdentifier **value, uint32_t *count)
{
	Namespace* object = static_cast<NPObjectNamespace*>(npobj)->object;
	const Namespace::Namespaces& namespaces = object->GetNamespaces();
	MetaProvider::ClassBase* classMeta = object->GetClassMeta();

	size_t identifiersCount = namespaces.size();
	if(classMeta)
		identifiersCount += classMeta->GetStaticMethodsByIdentifier().size();

	NPIdentifier* identifiers = (NPIdentifier*)Platform::NpapiPlugin::browserFuncs.memalloc(sizeof(NPIdentifier) * identifiersCount);
	int j = 0;

	for(Namespace::Namespaces::const_iterator i = namespaces.begin(); i != namespaces.end(); ++i)
		identifiers[j++] = i->first;

	if(classMeta)
	{
		const MetaProvider::ClassBase::IdentifierMap& staticMethods = classMeta->GetStaticMethodsByIdentifier();
		for(MetaProvider::ClassBase::IdentifierMap::const_iterator i = staticMethods.begin(); i != staticMethods.end(); ++i)
			identifiers[j++] = i->first;
	}

	*value = identifiers;
	*count = (uint32_t)identifiersCount;
	return true;
}

bool NPClassNamespace::npConstruct(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	MetaProvider::ClassBase* classMeta = static_cast<NPObjectNamespace*>(npobj)->object->GetClassMeta();
	if(!classMeta)
		return false;

	MetaProvider::ConstructorBase* constructor = classMeta->GetConstructor();
	if(!constructor)
		return false;

	return constructor->GetThunk()(args, argCount, result);
}

NPClassNamespace::NPClassNamespace()
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
