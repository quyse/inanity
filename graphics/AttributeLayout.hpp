#ifndef ___INANITY_GRAPHICS_ATTRIBUTE_LAYOUT_HPP___
#define ___INANITY_GRAPHICS_ATTRIBUTE_LAYOUT_HPP___

#include "DataType.hpp"
#include "LayoutDataType.hpp"
#include <vector>

BEGIN_INANITY_GRAPHICS

class VertexLayoutElement;
class AttributeLayoutSlot;
class AttributeLayoutElement;

/// Attribute layout class for binding to vertex pipeline.
class AttributeLayout : public Object
{
public:
	/// Slot of vertex buffer.
	struct Slot
	{
		/// Instancing divisor.
		/** For indexed elements = 0. For per-instance elements >= 1. */
		int divisor;

		Slot(int divisor);
	};
	typedef std::vector<Slot> Slots;

	struct Element
	{
		int slot;
		int offset;
		DataType dataType;
		LayoutDataType layoutDataType;

		Element(int slot, int offset, DataType dataType, LayoutDataType layoutDataType);
	};
	typedef std::vector<Element> Elements;

private:
	Slots slots;
	Elements elements;

public:
	ptr<AttributeLayoutSlot> AddSlot(int divisor = 0);
	ptr<AttributeLayoutElement> AddElement(ptr<AttributeLayoutSlot> slot, ptr<VertexLayoutElement> element);

	const Slots& GetSlots() const;
	const Elements& GetElements() const;
};

END_INANITY_GRAPHICS

#endif
