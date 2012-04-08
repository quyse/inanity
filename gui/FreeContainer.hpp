#ifndef ___INANITY_GUI_FREE_CONTAINER_HPP___
#define ___INANITY_GUI_FREE_CONTAINER_HPP___

#include "ContainerElement.hpp"
#include <vector>

BEGIN_INANITY_GUI

class Visualizer;

/// Класс "свободного" контейнера элементов.
/** Позволяет размещать на себе элементы свободно, без всяких ограничений. */
class FreeContainer : public ContainerElement
{
private:
	/// Список элементов.
	std::vector<ptr<Element> > elements;

public:
	FreeContainer(ptr<Visualizer> visualizer);

	/// Добавить элемент.
	void AddElement(ptr<Element> element);
	/// Удалить элемент.
	void RemoveElement(ptr<Element> element);

	void Draw(Position offset);

	bool IsPositionInto(Position position) const;

	ptr<Element> GetElementByPosition(Position position) const;
};

END_INANITY_GUI

#endif
