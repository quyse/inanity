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

BEGIN_INANITY_PLATFORM

//*** NpapiPlugin

NPNetscapeFuncs NpapiPlugin::browserFuncs;

void NpapiPlugin::GetPluginFuncs(NPPluginFuncs* pluginFuncs)
{
	pluginFuncs->newp = &NPP_New;
	pluginFuncs->destroy = &NPP_Destroy;
	pluginFuncs->setwindow = &NPP_SetWindow;
	pluginFuncs->event = &NPP_HandleEvent;
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

NPError NpapiPlugin::NPP_Destroy(NPP npp, NPSavedData** save)
{
	GET_INSTANCE();

	instance->Dereference();
	npp->pdata = nullptr;

	return NPERR_NO_ERROR;
}

NPError NpapiPlugin::NPP_SetWindow(NPP npp, NPWindow* window)
{
	GET_INSTANCE();

	return instance->NppSetWindow(window);
}

int16_t NpapiPlugin::NPP_HandleEvent(NPP npp, void* e)
{
	GET_INSTANCE();

	return instance->NppHandleEvent(e);
}

NPError NpapiPlugin::NPP_GetValue(NPP npp, NPPVariable variable, void* retValue)
{
	GET_INSTANCE();

	return instance->NppGetValue(variable, retValue);
}

END_INANITY_PLATFORM
