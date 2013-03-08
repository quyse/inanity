#ifndef ___INANITY_GRAPHICS_WIN32_OUTPUT_HPP___
#define ___INANITY_GRAPHICS_WIN32_OUTPUT_HPP___

#include "Output.hpp"
#include "../windows.hpp"

BEGIN_INANITY

class Win32Window;

END_INANITY

BEGIN_INANITY_GRAPHICS

/// Класс области графического вывода на Win32.
/** Представляет всегда окно, обычное или полноэкранное.
*/
class Win32Output : public Output
{
private:
	ptr<Win32Window> window;

public:
	Win32Output(ptr<Win32Window> window);

	ptr<Win32Window> GetWindow() const;
	HWND GetHWND() const;
};

END_INANITY_GRAPHICS

#endif
