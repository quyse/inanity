#ifndef ___INANITY_GUI_ELEMENT_HPP___
#define ___INANITY_GUI_ELEMENT_HPP___

#include "gui.hpp"
#include "types.hpp"

BEGIN_INANITY_GUI

class Visualizer;
class ContainerElement;
class Canvas;

/// Base class of GUI element.
class Element : public Object
{
protected:
	/// Visualizer.
	ptr<Visualizer> visualizer;
	/// Parent element.
	ContainerElement* parent;
	/// Current position relative to parent.
	Position position;
	/// Current size.
	Size size;
	/// Is element in focus.
	bool focused;
	/// Is mouse over element.
	bool moused;

	/// Send notification.
	void Notify(int code);

public:
	Element(ptr<Visualizer> visualizer);

	ContainerElement* GetParent() const;
	void SetParent(ContainerElement* parent);

	/// Get current position (relative to parent).
	Position GetPosition() const;
	/// Get current size (relative to parent).
	Size GetSize() const;

	//*** Layout methods. Should not be called explicitly.
	virtual void SetPosition(Position position);
	virtual void SetSize(Size size);

	bool IsFocused() const;
	void SetFocus();

	virtual bool IsPositionInto(Position position) const;

	virtual void Draw(Canvas* canvas, Position offset = Position(0, 0)) = 0;

	//*** Event methods.
	virtual void EventMouseMove(Position position);
	virtual void EventMouseDown(Position position, MouseEvent::Button button);
	virtual void EventMouseUp(Position position, MouseEvent::Button button);
	virtual void EventMouseEnter();
	virtual void EventMouseLeave();
	virtual void EventMouseWheel(Distance wheel);
	virtual void EventSetFocus();
	virtual void EventLostFocus();
	virtual void EventKeyDown(Key key);
	virtual void EventKeyUp(Key key);
	virtual void EventKeyPress(wchar_t key);
};

END_INANITY_GUI

#endif
