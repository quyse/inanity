#ifndef ___INANITY_PLATFORM_NPAPI_PLUGIN_HPP___
#define ___INANITY_PLATFORM_NPAPI_PLUGIN_HPP___

#include "npapi.hpp"

BEGIN_INANITY_PLATFORM

class NpapiPluginInstance;

/// Class which handles all global information for plugin,
/// and works as an instance of a plugin.
class NpapiPlugin
{
private:
	static NpapiPluginInstance* GetInstance(NPP npp, NPError& error);

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
	of a class derived from NpapiPluginInstance. */
	static ptr<NpapiPluginInstance> CreateInstance(int argc, char* argn[], char* argv[]);

	/// Fill provided structure with pointers to plugin functions.
	static void GetPluginFuncs(NPPluginFuncs* pluginFuncs);
};

END_INANITY_PLATFORM

#endif
