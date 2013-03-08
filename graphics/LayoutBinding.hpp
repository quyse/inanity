#ifndef ___INANITY_GRAPHICS_LAYOUT_BINDING_HPP___
#define ___INANITY_GRAPHICS_LAYOUT_BINDING_HPP___

#include "graphics.hpp"
#include <vector>

BEGIN_INANITY_GRAPHICS

class Layout;

/// Класс привязки - связь между двумя разметками.
/** Описывает перестановку данных между разметками. */
class LayoutBinding : public Object
{
public:
	/// Элемент привязки.
	struct Element
	{
		int sourceOffset;
		int destOffset;
		int size;

		Element(int sourceOffset = -1, int destOffset = -1, int size = 0);

		friend bool operator<(const Element& a, const Element& b);
	};

private:
	std::vector<Element> elements;

public:
	LayoutBinding(const std::vector<Element>& elements);

	const std::vector<Element>& GetElements() const;

	void Apply(const void* sourceData, void* destData);

	static ptr<LayoutBinding> Create(Layout* sourceLayout, Layout* destLayout);
};

END_INANITY_GRAPHICS

#endif
