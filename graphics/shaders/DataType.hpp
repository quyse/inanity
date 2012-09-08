#ifndef ___INANITY_GRAPHICS_SHADERS_DATA_TYPE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_DATA_TYPE_HPP___

#include "shaders.hpp"

BEGIN_INANITY_SHADERS

/// Типы данных в шейдерах.
struct DataTypes
{
	enum _
	{
		Float,
		Float2,
		Float3,
		Float4,
		Float4x4,
		UInt,
		UInt2,
		UInt3,
		UInt4,
		_Count
	};
};
typedef DataTypes::_ DataType;

/// Размеры данных.
extern int dataTypeSizes[DataTypes::_Count];
inline int GetDataTypeSize(DataType dataType)
{
	return dataTypeSizes[dataType];
}

/// Перевести тип на этапе компиляции в константу типа.
template <typename T>
static DataType GetDataType();
template <>
static DataType GetDataType<float>() { return DataTypes::Float; }
template <>
static DataType GetDataType<float2>() { return DataTypes::Float2; }
template <>
static DataType GetDataType<float3>() { return DataTypes::Float3; }
template <>
static DataType GetDataType<float4>() { return DataTypes::Float4; }
template <>
static DataType GetDataType<float4x4>() { return DataTypes::Float4x4; }
template <>
static DataType GetDataType<unsigned int>() { return DataTypes::UInt; }

END_INANITY_SHADERS

#endif
