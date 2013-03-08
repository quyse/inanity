#ifndef ___INANITY_GRAPHICS_SYSTEM_HPP___
#define ___INANITY_GRAPHICS_SYSTEM_HPP___

#include "graphics.hpp"
#include "shaders/shaders.hpp"

BEGIN_INANITY

class Window;

END_INANITY

BEGIN_INANITY_SHADERS

class ShaderGenerator;

END_INANITY_SHADERS

BEGIN_INANITY_GRAPHICS

class Device;
class ShaderCompiler;

/// Класс графической подсистемы.
/** Абстрактная фабрика для создания объектов
графических устройств. */
class System : public Object
{
public:
	/// Информация о графическом устройстве.
	class DeviceInfo
	{
	};

public:
	/// Создать окно, пригодное для использования с данной графической подсистемой.
	virtual ptr<Window> CreateDefaultWindow() = 0;
	/// Создать главное графическое устройство.
	virtual ptr<Device> CreatePrimaryDevice() = 0;

	/// Создать компилятор шейдеров.
	virtual ptr<ShaderCompiler> CreateShaderCompiler() = 0;
	/// Создать генератор шейдеров.
	virtual ptr<Shaders::ShaderGenerator> CreateShaderGenerator() = 0;
};

END_INANITY_GRAPHICS

#endif
