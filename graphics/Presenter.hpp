#ifndef ___INANITY_GRAPHICS_PRESENTER_HPP___
#define ___INANITY_GRAPHICS_PRESENTER_HPP___

#include "graphics.hpp"
#include "PresentMode.hpp"

BEGIN_INANITY_GRAPHICS

class Device;
class RenderBuffer;

/// Абстрактный класс "представлятеля" графики на экран.
class Presenter : public Object
{
public:
	/// Получить устройство, создавшее Presenter.
	virtual ptr<Device> GetDevice() = 0;

	/// Получить вторичный буфер.
	virtual ptr<RenderBuffer> GetBackBuffer() = 0;

	/// Сменить режим экрана.
	virtual void SetMode(const PresentMode& mode) = 0;

	/// Отобразить очередной кадр.
	/** Обычно, это переворот двойного буфера. */
	virtual void Present() = 0;

	/// Изменить размер области вывода.
	/** Вызывается окном, когда его размер изменяется. */
	virtual void Resize(int width, int height) = 0;
};

END_INANITY_GRAPHICS

#endif
