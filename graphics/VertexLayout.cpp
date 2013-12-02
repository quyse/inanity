#include "VertexLayout.hpp"
#include "VertexLayoutElement.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

VertexLayout::Element::Element(DataType dataType, LayoutDataType layoutDataType, int offset)
: dataType(dataType), layoutDataType(layoutDataType), offset(offset) {}

VertexLayout::VertexLayout(int stride)
: stride(stride) {}

ptr<VertexLayoutElement> VertexLayout::AddElement(DataType dataType, LayoutDataType layoutDataType, int offset)
{
	int index = (int)elements.size();
	elements.push_back(Element(dataType, layoutDataType, offset));
	return NEW(VertexLayoutElement(this, index));
}

ptr<VertexLayoutElement> VertexLayout::AddElement(DataType dataType, int offset)
{
	LayoutDataType layoutDataType;
	switch(dataType)
	{
		case DataTypes::_float:
		case DataTypes::_vec2:
		case DataTypes::_vec3:
		case DataTypes::_vec4:
		case DataTypes::_mat3x3:
		case DataTypes::_mat4x4:
			layoutDataType = LayoutDataTypes::Float32;
			break;
		case DataTypes::_uint:
		case DataTypes::_uvec2:
		case DataTypes::_uvec3:
		case DataTypes::_uvec4:
			layoutDataType = LayoutDataTypes::Uint32;
			break;
		case DataTypes::_int:
		case DataTypes::_ivec2:
		case DataTypes::_ivec3:
		case DataTypes::_ivec4:
			layoutDataType = LayoutDataTypes::Int32;
			break;
		default:
			THROW("Unknown data type");
	}
	return AddElement(dataType, layoutDataType, offset);
}

const VertexLayout::Elements& VertexLayout::GetElements() const
{
	return elements;
}

int VertexLayout::GetStride() const
{
	return stride;
}

END_INANITY_GRAPHICS
