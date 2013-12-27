#ifndef ___INANITY_PLATFORM_NPAPI_PLUGIN_INSTANCE_HPP___
#define ___INANITY_PLATFORM_NPAPI_PLUGIN_INSTANCE_HPP___

#include "npapi.hpp"
#include "../input/input.hpp"

BEGIN_INANITY_INPUT

class Manager;

END_INANITY_INPUT

BEGIN_INANITY_PLATFORM

#ifdef ___INANITY_PLATFORM_WINDOWS
class Win32Window;
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

	ptr<Input::Manager> inputManager;

#ifdef ___INANITY_PLATFORM_WINDOWS
	// Either window of hdc is not-null, depending on windowless mode.
	ptr<Win32Window> window;
	HDC hdc;

	/// Paint (in case of windowless window).
	virtual void Paint(HDC hdc);
#endif

	NpapiPluginInstance();

public:

#ifdef ___INANITY_PLATFORM_WINDOWS
	ptr<Win32Window> GetWindow() const;
#endif

	//*** Internal methods.
	void Init(NPP npp);
	NPError SetWindow(NPWindow* window);
	int16_t HandleEvent(void* event);
	NPError GetValue(NPPVariable variable, void* retValue);
};

END_INANITY_PLATFORM

#endif
