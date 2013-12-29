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

class State;

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

	NpapiPluginInstance(bool needInputManager, bool needScriptState);

#if defined(___INANITY_PLATFORM_WINDOWS)

	ptr<Input::Win32WmManager> inputManager;
	// Either window or hdc is not-null, depending on windowless mode.
	ptr<Win32Window> window;
	HDC hdc;

	/// Paint (in case of windowless window).
	virtual void Paint(HDC hdc);

public:
	ptr<Win32Window> GetWindow() const;

#else
#error Unknown platform.
#endif

	ptr<Script::Np::State> scriptState;

public:
	NPP GetNpp() const;
	static NpapiPluginInstance* FromNpp(NPP npp);

	ptr<Script::Np::State> GetScriptState() const;

	//*** Internal methods.
	void Init(NPP npp);
	NPError SetWindow(NPWindow* window);
	int16_t HandleEvent(void* event);
	NPError GetValue(NPPVariable variable, void* retValue);
};

END_INANITY_PLATFORM

#endif
