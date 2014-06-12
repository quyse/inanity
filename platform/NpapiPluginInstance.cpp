#include "NpapiPluginInstance.hpp"
#include "NpapiPlugin.hpp"
#include "../input/Frame.hpp"
#include "../script/np/State.hpp"
#include "../script/np/Any.hpp"
#include "../MemoryStream.hpp"
#include "../File.hpp"
#include "../Exception.hpp"
#include "../Log.hpp"
#include <sstream>

#if defined(___INANITY_PLATFORM_WINDOWS)
#include "Win32Window.hpp"
#include "../input/Win32WmManager.hpp"
#elif defined(___INANITY_PLATFORM_LINUX)
#else
#error Unknown platform
#endif

BEGIN_INANITY_PLATFORM

struct NpapiPluginInstance::UrlStream : public Object
{
	ptr<ReceiveHandler> receiveHandler;
	ptr<MemoryStream> stream;

	UrlStream(ptr<ReceiveHandler> receiveHandler)
	: receiveHandler(receiveHandler) {}
};

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
#elif defined(___INANITY_PLATFORM_LINUX)
#else
#error Unknown platform
#endif
}

NpapiPluginInstance::~NpapiPluginInstance()
{
	if(scriptObject)
		scriptObject->GetState()->ReclaimInstance(scriptObject);
}

#ifdef ___INANITY_PLATFORM_WINDOWS

void NpapiPluginInstance::Paint(HDC hdc)
{
	// do nothing by default
}

#endif

#ifdef ___INANITY_PLATFORM_LINUX

void NpapiPluginInstance::Paint(const XGraphicsExposeEvent& event)
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

	return scriptObject->GetState()->CreateAny(variant);
}

ptr<Script::Np::Any> NpapiPluginInstance::GetPluginDomObject() const
{
	NPObject* npObject;
	NPVariant variant;
	if(NpapiPlugin::browserFuncs.getvalue(npp, NPNVPluginElementNPObject, &npObject) == NPERR_NO_ERROR)
		OBJECT_TO_NPVARIANT(npObject, variant);
	else
		NULL_TO_NPVARIANT(variant);

	return scriptObject->GetState()->CreateAny(variant);
}

void NpapiPluginInstance::GetUrl(const String& url, ptr<ReceiveHandler> receiveHandler)
{
	ptr<UrlStream> urlStream = NEW(UrlStream(receiveHandler));
	urlStream->Reference();
	NpapiPlugin::browserFuncs.geturlnotify(
		npp,
		url.c_str(),
		nullptr, // target
		&*urlStream);
}

void NpapiPluginInstance::PostUrl(const String& url, ptr<File> postData, ptr<ReceiveHandler> receiveHandler)
{
	ptr<UrlStream> urlStream = NEW(UrlStream(receiveHandler));
	urlStream->Reference();
	NpapiPlugin::browserFuncs.posturlnotify(
		npp,
		url.c_str(),
		nullptr, // target
		(uint32_t)postData->GetSize(),
		(const char*)postData->GetData(),
		false, // is file
		&*urlStream);
}

void NpapiPluginInstance::AsyncCallRoutine(void* data)
{
	try
	{
		Handler* handler = (Handler*)data;
		handler->Fire();
		handler->Dereference();
	}
	catch(Exception* exception)
	{
		std::ostringstream ss;
		MakePointer(exception)->PrintStack(ss);
		Log::Error("Async call exception: " + ss.str());
	}
}

void NpapiPluginInstance::AsyncCall(ptr<Handler> handler)
{
	handler->Reference();
	NpapiPlugin::browserFuncs.pluginthreadasynccall(
		npp,
		&AsyncCallRoutine,
		&*handler);
}

void NpapiPluginInstance::Invalidate()
{
	NPRect rect;
	rect.top = 0;
	rect.left = 0;
	rect.bottom = height;
	rect.right = width;
	NpapiPlugin::browserFuncs.invalidaterect(npp, &rect);
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

void NpapiPluginInstance::Destroy() {}

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

#elif defined(___INANITY_PLATFORM_LINUX)

#else
#error Unknown platform
#endif

	return NPERR_NO_ERROR;
}

NPError NpapiPluginInstance::NppNewStream(NPMIMEType type, NPStream* stream, NPBool seekable, uint16_t* stype)
{
	UrlStream* urlStream = (UrlStream*)stream->notifyData;
	urlStream->stream = NEW(MemoryStream());
	urlStream->Reference();
	*stype = NP_NORMAL;
	return NPERR_NO_ERROR;
}

NPError NpapiPluginInstance::NppDestroyStream(NPStream* stream, NPReason reason)
{
	UrlStream* urlStream = (UrlStream*)stream->notifyData;

	NPError error = NPERR_NO_ERROR;

	try
	{
		// finalize with data or error
		if(reason == NPRES_DONE)
			urlStream->receiveHandler->FireData(urlStream->stream->ToFile());
		else
			urlStream->receiveHandler->FireError(NEW(Exception("Abnormal end of URL stream")));
	}
	catch(Exception* exception)
	{
		MakePointer(exception);
		error = NPERR_GENERIC_ERROR;
	}

	// clear references
	urlStream->stream = nullptr;
	urlStream->receiveHandler = nullptr;
	urlStream->Dereference();

	return error;
}

int32_t NpapiPluginInstance::NppWriteReady(NPStream* stream)
{
	return 0x10000;
}

int32_t NpapiPluginInstance::NppWrite(NPStream* stream, int32_t offset, int32_t len, void* buffer)
{
	UrlStream* urlStream = (UrlStream*)stream->notifyData;

	try
	{
		urlStream->stream->Write(buffer, len);
		return len;
	}
	catch(Exception* exception)
	{
		MakePointer(exception);
		return -1;
	}
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

#elif defined(___INANITY_PLATFORM_LINUX)

	return 0;

#else
#error Unknown platform
#endif
}

void NpapiPluginInstance::NppURLNotify(const char* url, NPReason reason, void* notifyData)
{
	UrlStream* urlStream = (UrlStream*)notifyData;

	if(urlStream->receiveHandler)
	{
		if(reason != NPRES_DONE)
			// ignore exceptions from handler
			try
			{
				urlStream->receiveHandler->FireError(NEW(Exception("Failed getting URL stream")));
			}
			catch(Exception* exception)
			{
				MakePointer(exception);
			}

		urlStream->receiveHandler = nullptr;
	}

	urlStream->Dereference();
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
