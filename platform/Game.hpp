#ifndef ___INANITY_PLATFORM_GAME_HPP___
#define ___INANITY_PLATFORM_GAME_HPP___

#include "platform.hpp"
#include "../graphics/graphics.hpp"
#include "../input/input.hpp"

BEGIN_INANITY_GRAPHICS

class System;
class Device;
class Presenter;

END_INANITY_GRAPHICS

BEGIN_INANITY_INPUT

class Manager;

END_INANITY_INPUT

BEGIN_INANITY_PLATFORM

class Window;

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

public:
	static ptr<Graphics::System> CreateDefaultGraphicsSystem();

	static ptr<Input::Manager> CreateInputManager(ptr<Window> window);
};

END_INANITY_PLATFORM

#endif
