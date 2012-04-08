#ifndef ___INANITY_GUI_CONTAINER_ELEMENT_HPP___
#define ___INANITY_GUI_CONTAINER_ELEMENT_HPP___

#include "Element.hpp"

BEGIN_INANITY_GUI

class Visualizer;
class Notificator;

/// Базовый класс контейнерного элемента.
class ContainerElement : public Element
{
private:
	/// Элемент, находящийся в фокусе.
	/** Значение null зависит от реализации конкретного контейнерного элемента,
	и может обозначать фокус самого контейнера. */
	ptr<Element> focusedElement;
	/// Элемент, над которым находится мышь.
	ptr<Element> mousedElement;
	/// Приёмник уведомлений.
	ptr<Notificator> notificator;

public:
	ContainerElement(ptr<Visualizer> visualizer);

	/// Указать приёмник уведомлений.
	void SetNotificator(ptr<Notificator> notificator);
	/// Запросить фокус элемента.
	/** Обычно элементы запрашивают фокус для себя, после щелчка мышью, к примеру. */
	void QueryFocus(ptr<Element> element);
	/// Принять уведомление от дочернего элемента.
	void Notify(ptr<Element> element, int code);

	/// Получить дочерний элемент по точке.
	/** Если элемента нет, должен возвращаться 0. */
	virtual ptr<Element> GetElementByPosition(Position position) const = 0;

	void EventMouseMove(Position position);
	void EventMouseDown(Position position, MouseButton button);
	void EventMouseUp(Position position, MouseButton button);
	void EventMouseLeave();
	void EventSetFocus();
	void EventLostFocus();
	void EventKeyDown(Key key);
	void EventKeyUp(Key key);
	void EventKeyPress(wchar_t key);
};

END_INANITY_GUI

#endif
