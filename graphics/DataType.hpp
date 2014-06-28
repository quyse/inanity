#ifndef ___INANITY_GRAPHICS_DATA_TYPE_HPP___
#define ___INANITY_GRAPHICS_DATA_TYPE_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

/// Типы данных в шейдерах.
struct DataTypes
{
	enum _
	{
		_float,
		_vec2,
		_vec3,
		_vec4,
		_mat3x3,
		_mat4x4,
		_uint,
		_uvec2,
		_uvec3,
		_uvec4,
		_int,
		_ivec2,
		_ivec3,
		_ivec4,
		_bool,
		_bvec2,
		_bvec3,
		_bvec4
	};
};

typedef DataTypes::_ DataType;

/// Получить константу типа на этапе компиляции.
template <typename T>
inline DataType DataTypeOf();
#define DTO(t) template <> inline DataType DataTypeOf<t>() { return DataTypes::_##t; }
DTO(float)
DTO(vec2)
DTO(vec3)
DTO(vec4)
DTO(mat3x3)
DTO(mat4x4)
DTO(uint)
DTO(uvec2)
DTO(uvec3)
DTO(uvec4)
DTO(int)
DTO(ivec2)
DTO(ivec3)
DTO(ivec4)
DTO(bool)
DTO(bvec2)
DTO(bvec3)
DTO(bvec4)
#undef DTO

int GetDataTypeSize(DataType dataType);
DataType GetBaseDataType(DataType dataType);
DataType GetVectorDataType(DataType baseDataType, int dimensions);
int GetVectorDataTypeDimensions(DataType vectorDataType);

END_INANITY_GRAPHICS

#endif
