#ifndef ___INANITY_PLATFORM_NPAPI_PLUGIN_HPP___
#define ___INANITY_PLATFORM_NPAPI_PLUGIN_HPP___

#include "platform.hpp"
#ifdef ___INANITY_PLATFORM_WINDOWS
#include "windows.hpp"
#endif
#include "../deps/npapi/npfunctions.h"

BEGIN_INANITY_PLATFORM

/// Class which handles all global information for plugin,
/// and works as an instance of a plugin.
class NpapiPlugin
{
public:
	/// Base class of instance of the plugin.
	class Instance : public Object
	{
		friend class NpapiPlugin;
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

#ifdef ___INANITY_PLATFORM_WINDOWS
		// Either hWnd of hdc is not-null, depending on windowless mode.
		HWND hWnd;
		HDC hdc;
#endif

		Instance();

	public:
		NPError SetWindow(NPWindow* window);
		virtual int16_t HandleEvent(void* event) = 0;
		NPError GetValue(NPPVariable variable, void* retValue);
	};

private:
	static Instance* GetInstance(NPP npp, NPError& error);

	//*** Plugin functions.
	static NPError NPP_New(
		NPMIMEType pluginType,
		NPP npp,
		uint16_t mode,
		int16_t argc,
		char* argn[],
		char* argv[],
		NPSavedData* saved);
	static NPError NPP_Destroy(NPP npp, NPSavedData** save);
	static NPError NPP_SetWindow(NPP npp, NPWindow* window);
	static int16_t NPP_HandleEvent(NPP npp, void* event);
	static NPError NPP_GetValue(NPP npp, NPPVariable variable, void* retValue);

public:
	static NPNetscapeFuncs browserFuncs;

	/// Create instance of plugin.
	/** This function is not defined. It should be defined once
	per executable (DLL) and should return new instance
	of a class derived from Instance. */
	static ptr<Instance> CreateInstance();

	/// Fill provided structure with pointers to plugin functions.
	static void GetPluginFuncs(NPPluginFuncs* pluginFuncs);
};

END_INANITY_PLATFORM

#endif
