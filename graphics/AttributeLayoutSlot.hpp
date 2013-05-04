#ifndef ___INANITY_GRAPHICS_ATTRIBUTE_LAYOUT_SLOT_HPP___
#define ___INANITY_GRAPHICS_ATTRIBUTE_LAYOUT_SLOT_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

class AttributeLayout;
struct ContextState;
class VertexBuffer;

/// Класс слота для разметки атрибутов.
class AttributeLayoutSlot : public Object
{
private:
	ptr<AttributeLayout> layout;
	int index;

public:
	AttributeLayoutSlot(ptr<AttributeLayout> layout, int index);

	ptr<AttributeLayout> GetLayout() const;
	int GetIndex() const;

	void Apply(ContextState& contextState, ptr<VertexBuffer> vertexBuffer);
};

END_INANITY_GRAPHICS

#endif
