#include "DataType.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

int GetDataTypeSize(DataType dataType)
{
	switch(dataType)
	{
#define DTS(t) case DataTypes::_##t: return sizeof(t);
	DTS(float)
	DTS(vec2)
	DTS(vec3)
	DTS(vec4)
	DTS(mat3x3)
	DTS(mat4x4)
	DTS(uint)
	DTS(uvec2)
	DTS(uvec3)
	DTS(uvec4)
	DTS(int)
	DTS(ivec2)
	DTS(ivec3)
	DTS(ivec4)
#undef DTS
	default:
		THROW("Invalid data type");
	}
}

DataType GetBaseDataType(DataType dataType)
{
	switch(dataType)
	{
	case DataTypes::_float:
	case DataTypes::_vec2:
	case DataTypes::_vec3:
	case DataTypes::_vec4:
	case DataTypes::_mat3x3:
	case DataTypes::_mat4x4:
		return DataTypes::_float;
	case DataTypes::_uint:
	case DataTypes::_uvec2:
	case DataTypes::_uvec3:
	case DataTypes::_uvec4:
		return DataTypes::_uint;
	case DataTypes::_int:
	case DataTypes::_ivec2:
	case DataTypes::_ivec3:
	case DataTypes::_ivec4:
		return DataTypes::_int;
	default:
		THROW("Invalid base data type");
	}
}

DataType GetVectorDataType(DataType baseDataType, int dimensions)
{
	if(dimensions <= 0 || dimensions > 4)
		THROW("Invalid vector dimensions");
	--dimensions;
	switch(baseDataType)
	{
	case DataTypes::_float:
	case DataTypes::_uint:
	case DataTypes::_int:
		return (DataType)(baseDataType + dimensions);
	default:
		THROW("Invalid base data type");
	}
}

int GetVectorDataTypeDimensions(DataType vectorDataType)
{
	switch(vectorDataType)
	{
	case DataTypes::_float:
	case DataTypes::_uint:
	case DataTypes::_int:
		return 1;
	case DataTypes::_vec2:
	case DataTypes::_uvec2:
	case DataTypes::_ivec2:
		return 2;
	case DataTypes::_vec3:
	case DataTypes::_uvec3:
	case DataTypes::_ivec3:
		return 3;
	case DataTypes::_vec4:
	case DataTypes::_uvec4:
	case DataTypes::_ivec4:
		return 4;
	default:
		THROW("Invalid vector data type");
	}
}

END_INANITY_GRAPHICS
