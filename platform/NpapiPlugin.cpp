#include "NpapiPlugin.hpp"
#include "NpapiPluginInstance.hpp"
#include "../Exception.hpp"

//*** global functions

// on some platforms pointers to plugin functions are set
// via separate NP_GetEntryPoints routine, on some -
// via NP_Initialize

#if !defined(XP_UNIX) || defined(XP_MACOSX)
NPError OSCALL NP_GetEntryPoints(NPPluginFuncs* pluginFuncs)
{
	Inanity::Platform::NpapiPlugin::GetPluginFuncs(pluginFuncs);
	return NPERR_NO_ERROR;
}
#endif

#if !defined(XP_UNIX)
NPError OSCALL NP_Initialize(NPNetscapeFuncs* browserFuncs)
#elif defined(XP_MACOSX)
NP_EXPORT(NPError) NP_Initialize(NPNetscapeFuncs* browserFuncs)
#else
NP_EXPORT(NPError) NP_Initialize(NPNetscapeFuncs* browserFuncs, NPPluginFuncs* pluginFuncs)
#endif
{
	// save pointers to browser functions
	Inanity::Platform::NpapiPlugin::browserFuncs = *browserFuncs;

#if defined(XP_UNIX) && !defined(XP_MACOSX)
	Inanity::Platform::NpapiPlugin::GetPluginFuncs(pluginFuncs);
#endif

	// get special identifiers
#define I(name) \
	Inanity::Platform::NpapiPlugin::name##Identifier \
	= Inanity::Platform::NpapiPlugin::browserFuncs.getstringidentifier(#name)

	I(apply);
	I(call);
	I(__reclaim);

#undef I

	return NPERR_NO_ERROR;
}

#ifdef XP_UNIX
NP_EXPORT(NPError) NP_Shutdown(void)
#else
NPError OSCALL NP_Shutdown(void)
#endif
{
	return NPERR_NO_ERROR;
}

#ifdef XP_UNIX

NP_EXPORT(char*) NP_GetPluginVersion(void)
{
	return (char*)Inanity::Platform::NpapiPlugin::GetInfo().version;
}

NP_EXPORT(const char*) NP_GetMIMEDescription(void)
{
	return Inanity::Platform::NpapiPlugin::GetInfo().mime;
}

NP_EXPORT(NPError) NP_GetValue(void* future, NPPVariable aVariable, void* aValue)
{
	switch(aVariable)
	{
	case NPPVpluginNameString:
		*(const char**)aValue = Inanity::Platform::NpapiPlugin::GetInfo().name;
		break;
	case NPPVpluginDescriptionString:
		*(const char**)aValue = Inanity::Platform::NpapiPlugin::GetInfo().description;
		break;
	default:
		return NPERR_INVALID_PARAM;
	}
	return NPERR_NO_ERROR;
}

#endif

BEGIN_INANITY_PLATFORM

//*** NpapiPlugin

NPNetscapeFuncs NpapiPlugin::browserFuncs;

NPIdentifier NpapiPlugin::applyIdentifier;
NPIdentifier NpapiPlugin::callIdentifier;
NPIdentifier NpapiPlugin::__reclaimIdentifier;

void NpapiPlugin::GetPluginFuncs(NPPluginFuncs* pluginFuncs)
{
	pluginFuncs->newp = &NPP_New;
	pluginFuncs->destroy = &NPP_Destroy;
	pluginFuncs->setwindow = &NPP_SetWindow;
	pluginFuncs->newstream = &NPP_NewStream;
	pluginFuncs->destroystream = &NPP_DestroyStream;
	pluginFuncs->writeready = &NPP_WriteReady;
	pluginFuncs->write = &NPP_Write;
	pluginFuncs->event = &NPP_HandleEvent;
	pluginFuncs->urlnotify = &NPP_URLNotify;
	pluginFuncs->getvalue = &NPP_GetValue;
}

NpapiPluginInstance* NpapiPlugin::GetInstance(NPP instance, NPError& error)
{
	if(!instance)
	{
		error = NPERR_INVALID_INSTANCE_ERROR;
		return nullptr;
	}
	if(!instance->pdata)
	{
		error = NPERR_GENERIC_ERROR;
		return nullptr;
	}
	return (NpapiPluginInstance*)instance->pdata;
}

NPError NpapiPlugin::NPP_New(
	NPMIMEType pluginType,
	NPP npp,
	uint16_t mode,
	int16_t argc,
	char* argn[],
	char* argv[],
	NPSavedData* saved)
{
	try
	{
		ptr<NpapiPluginInstance> instance = CreateInstance(argc, argn, argv);
		instance->Reference();

		instance->Init(npp);

		return NPERR_NO_ERROR;
	}
	catch(Exception* exception)
	{
		MakePointer(exception);
		return NPERR_GENERIC_ERROR;
	}
}

#define GET_INSTANCE() \
	NPError error; \
	NpapiPluginInstance* instance = GetInstance(npp, error); \
	if(!instance) \
		return error
#define GET_INSTANCE_NO_ERROR() \
	NPError error; \
	NpapiPluginInstance* instance = GetInstance(npp, error); \
	if(!instance) \
		return

NPError NpapiPlugin::NPP_Destroy(NPP npp, NPSavedData** save)
{
	GET_INSTANCE();

	instance->Destroy();
	instance->Dereference();
	npp->pdata = nullptr;

	return NPERR_NO_ERROR;
}

NPError NpapiPlugin::NPP_SetWindow(NPP npp, NPWindow* window)
{
	GET_INSTANCE();

	return instance->NppSetWindow(window);
}

NPError NpapiPlugin::NPP_NewStream(NPP npp, NPMIMEType type, NPStream* stream, NPBool seekable, uint16_t* stype)
{
	GET_INSTANCE();

	return instance->NppNewStream(type, stream, seekable, stype);
}

NPError NpapiPlugin::NPP_DestroyStream(NPP npp, NPStream* stream, NPReason reason)
{
	GET_INSTANCE();

	return instance->NppDestroyStream(stream, reason);
}

int32_t NpapiPlugin::NPP_WriteReady(NPP npp, NPStream* stream)
{
	GET_INSTANCE();

	return instance->NppWriteReady(stream);
}

int32_t NpapiPlugin::NPP_Write(NPP npp, NPStream* stream, int32_t offset, int32_t len, void* buffer)
{
	GET_INSTANCE();

	return instance->NppWrite(stream, offset, len, buffer);
}

int16_t NpapiPlugin::NPP_HandleEvent(NPP npp, void* e)
{
	GET_INSTANCE();

	return instance->NppHandleEvent(e);
}

void NpapiPlugin::NPP_URLNotify(NPP npp, const char* url, NPReason reason, void* notifyData)
{
	GET_INSTANCE_NO_ERROR();

	return instance->NppURLNotify(url, reason, notifyData);
}

NPError NpapiPlugin::NPP_GetValue(NPP npp, NPPVariable variable, void* retValue)
{
	GET_INSTANCE();

	return instance->NppGetValue(variable, retValue);
}

END_INANITY_PLATFORM
