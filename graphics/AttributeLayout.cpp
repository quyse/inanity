#include "AttributeLayout.hpp"
#include "VertexLayout.hpp"
#include "VertexLayoutElement.hpp"
#include "AttributeLayoutSlot.hpp"
#include "AttributeLayoutElement.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

AttributeLayout::AttributeLayout() {}

ptr<AttributeLayoutSlot> AttributeLayout::AddSlot(ptr<VertexLayout> vertexLayout, int divisor)
{
	int index = (size_t)slots.size();
	slots.push_back(Slot(vertexLayout, divisor));
	return NEW(AttributeLayoutSlot(this, index));
}

ptr<AttributeLayoutElement> AttributeLayout::AddElement(ptr<AttributeLayoutSlot> slot, ptr<VertexLayoutElement> element)
{
	try
	{
		// проверить, что слот от этой разметки
		if(static_cast<AttributeLayout*>(slot->GetLayout()) != this)
			THROW_PRIMARY_EXCEPTION("Slot is from another attribute layout");
		// проверить, что разметка верная
		int slotIndex = slot->GetIndex();
		if(element->GetLayout() != slots[slotIndex].layout)
			THROW_PRIMARY_EXCEPTION("Wrong vertex layout for this slot");

		int elementIndex = (int)elements.size();
		elements.push_back(Element(slotIndex, element->GetOffset(), element->GetDataType()));
		return NEW(AttributeLayoutElement(this, elementIndex));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't add element to attribute layout", exception);
	}
}

const AttributeLayout::Slots& AttributeLayout::GetSlots() const
{
	return slots;
}

const AttributeLayout::Elements& AttributeLayout::GetElements() const
{
	return elements;
}

END_INANITY_GRAPHICS
