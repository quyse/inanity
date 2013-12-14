#ifndef ___INANITY_GUI_WINDOW_HPP___
#define ___INANITY_GUI_WINDOW_HPP___

#include "ContentContainer.hpp"
#include "../String.hpp"

BEGIN_INANITY_GUI

/// Window GUI element.
class Window : public ContentContainer
{
private:
	String title;

public:
	Window(ptr<Visualizer> visualizer, const String& title);

	void SetTitle(const String& title);
};

END_INANITY_GUI

#endif
