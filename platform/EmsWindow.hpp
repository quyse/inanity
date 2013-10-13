#ifndef ___INANITY_PLATFORM_EMS_WINDOW_HPP___
#define ___INANITY_PLATFORM_EMS_WINDOW_HPP___

#include "Window.hpp"

#ifndef ___INANITY_PLATFORM_EMSCRIPTEN
#error EmsWindow is for Emscripten platform
#endif

BEGIN_INANITY_PLATFORM

/// Dummy window class for Emscripten platform.
class EmsWindow : public Window
{
private:
	String title;
	ptr<Handler> activeHandler;

	/// Emscripten window can be only one.
	static EmsWindow* instance;

	/// Callback to emscripten to execute as a main loop.
	static void MainLoop();

public:
	EmsWindow(const String& title, int width, int height);
	~EmsWindow();

	//*** Window's methods.
	void SetTitle(const String& title);
	void Close();
	void Run(ptr<Handler> activeHandler);
	ptr<Graphics::Output> CreateOutput();
};

END_INANITY_PLATFORM

#endif
