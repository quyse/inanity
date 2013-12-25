#ifndef ___INANITY_PLATFORM_WINDOW_HPP___
#define ___INANITY_PLATFORM_WINDOW_HPP___

#include "platform.hpp"
#include "../Handler.hpp"
#include "../String.hpp"
#include "../graphics/graphics.hpp"

BEGIN_INANITY_GRAPHICS

class Presenter;

END_INANITY_GRAPHICS

BEGIN_INANITY_PLATFORM

/// Абстрактный класс окна.
class Window : public Object
{
public:
	/// Указать заголовок окна.
	virtual void SetTitle(const String& title) = 0;
	/// Закрыть окно.
	virtual void Close() = 0;

	/// Запустить оконный цикл.
	/** Цикл завершается, когда окно закрывается. */
	virtual void Run(ptr<Handler> activeHandler) = 0;

	void SetPresenter(Graphics::Presenter* presenter);
	/// Set mouse lock state.
	void SetMouseLock(bool mouseLock);
	/// Set hardware (OS) mouse cursor visibility.
	void SetCursorVisible(bool cursorVisible);
	/// Set position of hardware mouse cursor.
	/** Coordinates are relative to window. */
	virtual void PlaceCursor(int x, int y) = 0;

protected:
	/// Presenter for notifications.
	Graphics::Presenter* presenter;
	/// Mouse lock state.
	/** When mouse lock is enabled, cursor can't get out
	from window. */
	bool mouseLock;
	/// Mouse cursor visibility.
	bool cursorVisible;

	Window();

	virtual void UpdateMouseLock() = 0;
	virtual void UpdateCursorVisible() = 0;
};

END_INANITY_PLATFORM

#endif
