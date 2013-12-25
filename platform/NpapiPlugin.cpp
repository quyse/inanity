#include "NpapiPlugin.hpp"

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

NpapiPlugin::Instance* NpapiPlugin::GetInstance(NPP instance, NPError& error)
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
	return (NpapiPlugin::Instance*)instance->pdata;
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
	ptr<Instance> instance = CreateInstance();
	instance->Reference();
	npp->pdata = (Instance*)instance;

	browserFuncs.setvalue(npp, NPPVpluginWindowBool, (void*)!instance->windowless);
	browserFuncs.setvalue(npp, NPPVpluginTransparentBool, (void*)instance->transparent);
	browserFuncs.setvalue(npp, NPPVpluginUsesDOMForCursorBool, (void*)1);

	return NPERR_NO_ERROR;
}

#define GET_INSTANCE() \
	NPError error; \
	Instance* instance = GetInstance(npp, error); \
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

	return instance->SetWindow(window);
}

int16_t NpapiPlugin::NPP_HandleEvent(NPP npp, void* e)
{
	GET_INSTANCE();

	return instance->HandleEvent(e);
}

NPError NpapiPlugin::NPP_GetValue(NPP npp, NPPVariable variable, void* retValue)
{
	GET_INSTANCE();

	return instance->GetValue(variable, retValue);
}

//*** NpapiPlugin::Instance

NpapiPlugin::Instance::Instance() :
	name("Inanity NPAPI Plugin"),
	description("Inanity NPAPI Plugin"),
	windowless(true),
	transparent(false)
{}

NPError NpapiPlugin::Instance::SetWindow(NPWindow* window)
{
#ifdef ___INANITY_PLATFORM_WINDOWS

	hWnd = 0;
	hdc = 0;
	switch(window->type)
	{
	case NPWindowTypeWindow:
		hWnd = (HWND)window->window;
		break;
	case NPWindowTypeDrawable:
		hdc = (HDC)window->window;
		break;
	default:
		return NPERR_INVALID_PARAM;
	}

#else
#error Unknown platform
#endif

	return NPERR_NO_ERROR;
}

NPError NpapiPlugin::Instance::GetValue(NPPVariable variable, void* retValue)
{
	switch(variable)
	{
	case NPPVpluginNameString:
		*(const char**)retValue = name;
		break;
	case NPPVpluginDescriptionString:
		*(const char**)retValue = description;
		break;
	case NPPVpluginWindowBool:
		*(bool*)retValue = !windowless;
		break;
	case NPPVpluginTransparentBool:
		*(bool*)retValue = transparent;
		break;
	default:
		return NPERR_INVALID_PARAM;
	}

	return NPERR_NO_ERROR;
}

END_INANITY_PLATFORM
