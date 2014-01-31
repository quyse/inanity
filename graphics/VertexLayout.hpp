#ifndef ___INANITY_GRAPHICS_VERTEX_LAYOUT_HPP___
#define ___INANITY_GRAPHICS_VERTEX_LAYOUT_HPP___

#include "DataType.hpp"
#include "LayoutDataType.hpp"
#include <vector>

BEGIN_INANITY_GRAPHICS

class VertexLayoutElement;

/// Layout of vertex data in vertex buffer.
class VertexLayout : public Object
{
public:
	struct Element
	{
		DataType dataType;
		LayoutDataType layoutDataType;
		int offset;

		Element(DataType dataType, LayoutDataType layoutDataType, int offset);
	};
	typedef std::vector<Element> Elements;

protected:
	Elements elements;
	int stride;

public:
	VertexLayout(int stride);

	/// Add new element into layout.
	ptr<VertexLayoutElement> AddElement(DataType dataType, LayoutDataType layoutDataType, int offset);
	/// Add new element with default layout data type.
	ptr<VertexLayoutElement> AddElement(DataType dataType, int offset);
	/// Add struct's field as an element (helper method).
	template <typename Struct, typename ValueType>
	ptr<VertexLayoutElement> AddElement(ValueType Struct::*field)
	{
		return AddElement(DataTypeOf<ValueType>(), (int)(size_t)&(((Struct*)0)->*field));
	}
	const Elements& GetElements() const;

	int GetStride() const;
};

END_INANITY_GRAPHICS

#endif
