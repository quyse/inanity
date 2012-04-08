#ifndef ___INANITY_GUI_CONTENT_CONTAINER_HPP___
#define ___INANITY_GUI_CONTENT_CONTAINER_HPP___

#include "ContainerElement.hpp"
#include <vector>

BEGIN_INANITY_GUI

class Visualizer;

/// Класс "контентного" контейнера элементов.
/** Позволяет размещать на себе всего один дочерний элемент. */
class ContentContainer : public ContainerElement
{
private:
	/// Единственный дочерний элемент.
	ptr<Element> contentElement;

protected:
	Rect 

public:
	ContentContainer(ptr<Visualizer> visualizer);

	ptr<Element> GetContentElement() const;
	void SetContentElement(ptr<Element> contentElement);

	void Draw(Position offset);

	bool IsPositionInto(Position position) const;

	ptr<Element> GetElementByPosition(Position position) const;
};

END_INANITY_GUI

#endif
