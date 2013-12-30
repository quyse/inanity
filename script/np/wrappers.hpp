#ifndef ___INANITY_SCRIPT_NP_WRAPPERS_HPP___
#define ___INANITY_SCRIPT_NP_WRAPPERS_HPP___

#include "MetaProvider.hpp"
#include "../../platform/npapi.hpp"

BEGIN_INANITY_NP

struct NPClassWrapper;

struct NPObjectWrapper : public NPObject
{
	ptr<State> state;
	ptr<RefCounted> object;

	NPObjectWrapper(ptr<State> state);

	NPClassWrapper* GetClassWrapper() const;
};

struct NPClassWrapper : public NPClass
{
	// Functions needed in NPClass struct.
	static NPObject* npAllocate(NPP npp, NPClass *aClass);
	static void npDeallocate(NPObject *npobj);
	static void npInvalidate(NPObject *npobj);
	static bool npHasMethod(NPObject *npobj, NPIdentifier name);
	static bool npInvoke(NPObject *npobj, NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result);
	static bool npInvokeDefault(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result);
	static bool npHasProperty(NPObject *npobj, NPIdentifier name);
	static bool npGetProperty(NPObject *npobj, NPIdentifier name, NPVariant *result);
	static bool npSetProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value);
	static bool npRemoveProperty(NPObject *npobj, NPIdentifier name);
	static bool npEnumerate(NPObject *npobj, NPIdentifier **value, uint32_t *count);
	static bool npConstruct(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result);

	MetaProvider::ClassBase* classMeta;

	NPClassWrapper(MetaProvider::ClassBase* classMeta);
};

class Namespace;

struct NPClassNamespace;

struct NPObjectNamespace : public NPObject
{
	ptr<State> state;
	Namespace* object;

	NPObjectNamespace(ptr<State> state);
};

struct NPClassNamespace : public NPClass
{
	// Functions needed in NPClass struct.
	static NPObject* npAllocate(NPP npp, NPClass *aClass);
	static void npDeallocate(NPObject *npobj);
	static void npInvalidate(NPObject *npobj);
	static bool npHasMethod(NPObject *npobj, NPIdentifier name);
	static bool npInvoke(NPObject *npobj, NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result);
	static bool npInvokeDefault(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result);
	static bool npHasProperty(NPObject *npobj, NPIdentifier name);
	static bool npGetProperty(NPObject *npobj, NPIdentifier name, NPVariant *result);
	static bool npSetProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value);
	static bool npRemoveProperty(NPObject *npobj, NPIdentifier name);
	static bool npEnumerate(NPObject *npobj, NPIdentifier **value, uint32_t *count);
	static bool npConstruct(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result);

	NPClassNamespace();

	/// Only instance of the struct.
	static NPClassNamespace instance;
};

END_INANITY_NP

#endif
