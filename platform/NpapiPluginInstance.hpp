#ifndef ___INANITY_PLATFORM_NPAPI_PLUGIN_INSTANCE_HPP___
#define ___INANITY_PLATFORM_NPAPI_PLUGIN_INSTANCE_HPP___

#include "npapi.hpp"
#include "../input/input.hpp"
#include "../script/np/np.hpp"
#include "../Handler.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_INPUT

#if defined(___INANITY_PLATFORM_WINDOWS)
class Win32WmManager;
#else
#error Unknown platform
#endif

END_INANITY_INPUT

BEGIN_INANITY_NP

class Any;

END_INANITY_NP

BEGIN_INANITY_PLATFORM

#if defined(___INANITY_PLATFORM_WINDOWS)
class Win32Window;
#else
#error Unknown platform
#endif

/// Base class of instance of the NPAPI plugin.
class NpapiPluginInstance : public Object
{
public:
	typedef DataHandler<ptr<File> > ReceiveHandler;

protected:
	//*** Settings, should be set in derived class' constructor.
	/// Plugin name.
	const char* name;
	/// Plugin description.
	const char* description;
	/// Windowless mode.
	bool windowless;
	/// Transparent mode.
	bool transparent;

	/// Plugin instance handle.
	NPP npp;

	/// Size of client area.
	int width, height;

	NpapiPluginInstance(bool needInputManager);

#if defined(___INANITY_PLATFORM_WINDOWS)

	ptr<Input::Win32WmManager> inputManager;
	// Either window or hdc is not-null, depending on windowless mode.
	ptr<Win32Window> window;
	HDC hdc;

	/// Paint (in case of windowless window).
	virtual void Paint(HDC hdc);

#else
#error Unknown platform.
#endif

	/// Redefine in derived class to perform initialization.
	virtual void PostInit();

	ptr<Script::Np::Any> scriptObject;

	struct UrlStream;

	static void AsyncCallRoutine(void* data);

public:
	NPP GetNpp() const;
	static NpapiPluginInstance* FromNpp(NPP npp);

	ptr<Script::Np::Any> GetWindowDomObject() const;
	ptr<Script::Np::Any> GetPluginDomObject() const;

	/// Make HTTP GET request.
	void GetUrl(const String& url, ptr<ReceiveHandler> receiveHandler);
	/// Make HTTP POST request.
	void PostUrl(const String& url, ptr<File> postData, ptr<ReceiveHandler> receiveHandler);

	/// Make async call.
	void AsyncCall(ptr<Handler> handler);

	//*** Internal methods.
	void Init(NPP npp);
	NPError NppSetWindow(NPWindow* window);
	NPError NppNewStream(NPMIMEType type, NPStream* stream, NPBool seekable, uint16_t* stype);
	NPError NppDestroyStream(NPStream* stream, NPReason reason);
	int32_t NppWriteReady(NPStream* stream);
	int32_t NppWrite(NPStream* stream, int32_t offset, int32_t len, void* buffer);
	int16_t NppHandleEvent(void* event);
	void NppURLNotify(const char* url, NPReason reason, void* notifyData);
	NPError NppGetValue(NPPVariable variable, void* retValue);
};

END_INANITY_PLATFORM

#endif
