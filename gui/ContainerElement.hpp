#ifndef ___INANITY_GUI_CONTAINER_ELEMENT_HPP___
#define ___INANITY_GUI_CONTAINER_ELEMENT_HPP___

#include "Element.hpp"

BEGIN_INANITY_GUI

class Notificator;

/// Base class of GUI element which could be parent.
class ContainerElement : public Element
{
private:
	/// Element in focus.
	/** Could be null if no element in focus. */
	ptr<Element> focusedElement;
	/// Element with mouse over it.
	ptr<Element> mousedElement;
	ptr<Notificator> notificator;

protected:
	ContainerElement(ptr<Visualizer> visualizer);

public:
	void SetNotificator(ptr<Notificator> notificator);
	/// Query focus for child element.
	void QueryFocus(ptr<Element> element);
	/// Receive notification from child element.
	void Notify(ptr<Element> element, int code);

	virtual ptr<Element> TryGetElementByPosition(Position position) const = 0;

	/// Element's methods.
	void EventMouseMove(Position position);
	void EventMouseDown(Position position, MouseEvent::Button button);
	void EventMouseUp(Position position, MouseEvent::Button button);
	void EventMouseLeave();
	void EventSetFocus();
	void EventLostFocus();
	void EventKeyDown(Key key);
	void EventKeyUp(Key key);
	void EventKeyPress(wchar_t key);
};

END_INANITY_GUI

#endif
