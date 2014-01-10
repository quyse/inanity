#ifndef ___INANITY_PLATFORM_NPAPI_PLUGIN_INSTANCE_HPP___
#define ___INANITY_PLATFORM_NPAPI_PLUGIN_INSTANCE_HPP___

#include "npapi.hpp"
#include "../input/input.hpp"
#include "../script/np/np.hpp"

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

public:
	NPP GetNpp() const;
	static NpapiPluginInstance* FromNpp(NPP npp);

	ptr<Script::Np::Any> GetWindowDomObject() const;
	ptr<Script::Np::Any> GetPluginDomObject() const;

	//*** Internal methods.
	void Init(NPP npp);
	NPError NppSetWindow(NPWindow* window);
	int16_t NppHandleEvent(void* event);
	NPError NppGetValue(NPPVariable variable, void* retValue);
};

END_INANITY_PLATFORM

#endif
