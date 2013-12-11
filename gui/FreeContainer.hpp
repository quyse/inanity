#ifndef ___INANITY_GUI_FREE_CONTAINER_HPP___
#define ___INANITY_GUI_FREE_CONTAINER_HPP___

#include "ContainerElement.hpp"
#include <vector>

BEGIN_INANITY_GUI

/// Класс "свободного" контейнера элементов.
/** Позволяет размещать на себе элементы свободно, без всяких ограничений. */
class FreeContainer : public ContainerElement
{
private:
	/// Список элементов.
	std::vector<ptr<Element> > elements;

public:
	/// Добавить элемент.
	void AddElement(ptr<Element> element);
	/// Удалить элемент.
	void RemoveElement(ptr<Element> element);

	void Draw(Visualizer* visualizer, Position offset);

	bool IsPositionInto(Position position) const;

	ptr<Element> GetElementByPosition(Position position) const;
};

END_INANITY_GUI

#endif
