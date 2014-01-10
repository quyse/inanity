#include "NpapiPluginInstance.hpp"
#include "NpapiPlugin.hpp"
#include "../input/Frame.hpp"
#include "../script/np/State.hpp"
#include "../script/np/Any.hpp"

#if defined(___INANITY_PLATFORM_WINDOWS)
#include "Win32Window.hpp"
#include "../input/Win32WmManager.hpp"
#else
#error Unknown platform
#endif

BEGIN_INANITY_PLATFORM

NpapiPluginInstance::NpapiPluginInstance(bool needInputManager) :
	name("Inanity NPAPI Plugin"),
	description("Inanity NPAPI Plugin"),
	windowless(true),
	transparent(false),
	npp(nullptr)
{
#if defined(___INANITY_PLATFORM_WINDOWS)
	if(needInputManager)
		inputManager = NEW(Input::Win32WmManager());
#else
#error Unknown platform
#endif
}

#ifdef ___INANITY_PLATFORM_WINDOWS

void NpapiPluginInstance::Paint(HDC hdc)
{
	// do nothing by default
}

#endif

void NpapiPluginInstance::PostInit()
{
	// redefine in derived class
}

NPP NpapiPluginInstance::GetNpp() const
{
	return npp;
}

NpapiPluginInstance* NpapiPluginInstance::FromNpp(NPP npp)
{
	return (NpapiPluginInstance*)npp->pdata;
}

ptr<Script::Np::Any> NpapiPluginInstance::GetWindowDomObject() const
{
	NPObject* npObject;
	NPVariant variant;
	NPError err = NpapiPlugin::browserFuncs.getvalue(npp, NPNVWindowNPObject, &npObject);
	if(err == NPERR_NO_ERROR)
		OBJECT_TO_NPVARIANT(npObject, variant);
	else
		NULL_TO_NPVARIANT(variant);

	return Script::Np::State::GetCurrent()->CreateAny(variant);
}

ptr<Script::Np::Any> NpapiPluginInstance::GetPluginDomObject() const
{
	NPObject* npObject;
	NPVariant variant;
	if(NpapiPlugin::browserFuncs.getvalue(npp, NPNVPluginElementNPObject, &npObject) == NPERR_NO_ERROR)
		OBJECT_TO_NPVARIANT(npObject, variant);
	else
		NULL_TO_NPVARIANT(variant);

	return Script::Np::State::GetCurrent()->CreateAny(variant);
}

void NpapiPluginInstance::Init(NPP npp)
{
	this->npp = npp;
	npp->pdata = this;

	NpapiPlugin::browserFuncs.setvalue(npp, NPPVpluginWindowBool, (void*)!windowless);
	NpapiPlugin::browserFuncs.setvalue(npp, NPPVpluginTransparentBool, (void*)transparent);
	NpapiPlugin::browserFuncs.setvalue(npp, NPPVpluginUsesDOMForCursorBool, (void*)1);

	PostInit();
}

NPError NpapiPluginInstance::NppSetWindow(NPWindow* npWindow)
{
#if defined(___INANITY_PLATFORM_WINDOWS)

	switch(npWindow->type)
	{
	case NPWindowTypeWindow:
		{
			HWND hNewWnd = (HWND)npWindow->window;
			if(!window || window->GetHWND() != hNewWnd)
			{
				window = Win32Window::CreateExisting(hNewWnd, false);
				window->SetInputManager(inputManager);
			}
			hdc = 0;
		}
		break;
	case NPWindowTypeDrawable:
		{
			hdc = (HDC)npWindow->window;
			window = 0;
			width = npWindow->width;
			height = npWindow->height;
		}
		break;
	default:
		return NPERR_INVALID_PARAM;
	}

#else
#error Unknown platform
#endif

	return NPERR_NO_ERROR;
}

int16_t NpapiPluginInstance::NppHandleEvent(void* e)
{
#if defined(___INANITY_PLATFORM_WINDOWS)

	NPEvent* event = (NPEvent*)e;

	// process input events
	if(inputManager && inputManager->ProcessWindowMessage(event->event, event->wParam, event->lParam))
		return 1;

	switch(event->event)
	{
	case WM_PAINT:
		{
			// get hdc to paint
			HDC hdc = this->hdc;
			PAINTSTRUCT ps;
			if(window)
				hdc = BeginPaint(window->GetHWND(), &ps);
			if(!hdc)
				return 0;

			// do painting
			Paint(hdc);

			if(window)
				EndPaint(window->GetHWND(), &ps);
		}
		return 1;
	default:
		return 0;
	}

#else
#error Unknown platform
#endif
}

NPError NpapiPluginInstance::NppGetValue(NPPVariable variable, void* retValue)
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
	case NPPVpluginScriptableNPObject:
		if(scriptObject)
			*(NPObject**)retValue = NPVARIANT_TO_OBJECT(scriptObject->GetVariant());
		else
			return NPERR_INVALID_PARAM;
		break;
	default:
		return NPERR_INVALID_PARAM;
	}

	return NPERR_NO_ERROR;
}

END_INANITY_PLATFORM
