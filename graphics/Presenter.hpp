#ifndef ___INANITY_GRAPHICS_PRESENTER_HPP___
#define ___INANITY_GRAPHICS_PRESENTER_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

class Device;
class FrameBuffer;
class MonitorMode;

/// Абстрактный класс "представлятеля" графики на экран.
class Presenter : public Object
{
public:
	/// Получить устройство, создавшее Presenter.
	virtual ptr<Device> GetDevice() const = 0;

	/// Get screen width.
	virtual int GetWidth() const = 0;
	/// Get screen height.
	virtual int GetHeight() const = 0;

	/// Get a default framebuffer.
	virtual ptr<FrameBuffer> GetFrameBuffer() const = 0;

	/// Сменить режим экрана.
	virtual void SetMode(ptr<MonitorMode> mode) = 0;

	/// Отобразить очередной кадр.
	/** Обычно, это переворот двойного буфера. */
	virtual void Present() = 0;

	/// Изменить размер области вывода.
	/** Вызывается окном, когда его размер изменяется. */
	virtual void Resize(int width, int height) = 0;
};

END_INANITY_GRAPHICS

#endif
