#ifndef ___INANITY_PLATFORM_GAME_HPP___
#define ___INANITY_PLATFORM_GAME_HPP___

#include "platform.hpp"
#include "../graphics/graphics.hpp"

BEGIN_INANITY_GRAPHICS

class Device;

END_INANITY_GRAPHICS

BEGIN_INANITY_PLATFORM

/// Класс стандартной игры.
class Game : public Object
{
public:
	/// Настройки графического режима.
	struct GraphicsSettings
	{
		/// Полноэкранный режим?
		bool fullscreen;
		/// Хеш устройства.
		char deviceHash[20];
		/// Хеш монитора.
		char monitorHash[20];
		/// Хеш режима монитора (для полноэкранного режима).
		char modeHash[20];
		/// Размеры окна (для оконного режима).
		int windowWidth, windowHeight;
		/// Положение окна (для оконного режима).
		/** Задаётся в координатах относительно монитора. */
		int windowX, windowY;
	};

private:
	ptr<Graphics::Device> device;
	ptr<Graphics::Presenter> presenter;
};

END_INANITY_PLATFORM

#endif
