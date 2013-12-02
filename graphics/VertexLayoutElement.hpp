#ifndef ___INANITY_GRAPHICS_VERTEX_LAYOUT_ELEMENT_HPP___
#define ___INANITY_GRAPHICS_VERTEX_LAYOUT_ELEMENT_HPP___

#include "DataType.hpp"
#include "LayoutDataType.hpp"

BEGIN_INANITY_GRAPHICS

class VertexLayout;

/// Element of vertex layout.
/** Just pointer + index. */
class VertexLayoutElement : public Object
{
private:
	ptr<VertexLayout> layout;
	int index;

public:
	VertexLayoutElement(ptr<VertexLayout> layout, int index);

	ptr<VertexLayout> GetLayout() const;
	DataType GetDataType() const;
	LayoutDataType GetLayoutDataType() const;
	int GetOffset() const;
};

END_INANITY_GRAPHICS

#endif
