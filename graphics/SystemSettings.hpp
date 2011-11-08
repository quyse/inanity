#ifndef ___INANITY_GRAPHICS_SETTINGS_HPP___
#define ___INANITY_GRAPHICS_SETTINGS_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

/// Настройки экрана (графического режима).
struct ScreenSettings
{
	/// Полноэкранный режим?
	bool fullscreen;
	/// Ширина экрана в пикселах.
	unsigned width;
	/// Высота экрана в пикселах.
	unsigned height;
};

/// Настройки графики.
struct SystemSettings
{
	/// Настройки экрана.
	ScreenSettings screenSettings;
	//окно
	HWND hWnd;
	//"железный" режим? (false - референсный драйвер)
	bool hardware;
	//можно ли получать скриншот экрана
	bool screenCanBeRead;
};

END_INANITY_GRAPHICS

#endif
