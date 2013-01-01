#ifndef ___INANITY_GRAPHICS_DATA_TYPE_HPP___
#define ___INANITY_GRAPHICS_DATA_TYPE_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

/// Типы данных в шейдерах.
struct DataTypes
{
	enum _
	{
		Float,
		Float2,
		Float3,
		Float4,
		Float3x3,
		Float4x4,
		UInt,
		UInt2,
		UInt3,
		UInt4,
		Int,
		Int2,
		Int3,
		Int4,
		_Count
	};
};
typedef DataTypes::_ DataType;

/// Размеры данных.
inline int GetDataTypeSize(DataType dataType)
{
	switch(dataType)
	{
#define DT(name, size) case DataTypes::name: return size
		DT(Float, 4);
		DT(Float2, 8);
		DT(Float3, 12);
		DT(Float4, 16);
		DT(Float3x3, 36);
		DT(Float4x4, 64);
		DT(UInt, 4);
		DT(UInt2, 8);
		DT(UInt3, 12);
		DT(UInt4, 16);
		DT(Int, 4);
		DT(Int2, 8);
		DT(Int3, 12);
		DT(Int4, 16);
#undef DT
	default:
		return 0x7fffffff;
	}
}

/// Перевести тип на этапе компиляции в константу типа.
template <typename T>
DataType GetDataType();
template <>
inline DataType GetDataType<float>() { return DataTypes::Float; }
template <>
inline DataType GetDataType<float2>() { return DataTypes::Float2; }
template <>
inline DataType GetDataType<float3>() { return DataTypes::Float3; }
template <>
inline DataType GetDataType<float4>() { return DataTypes::Float4; }
template <>
inline DataType GetDataType<float3x3>() { return DataTypes::Float3x3; }
template <>
inline DataType GetDataType<float4x4>() { return DataTypes::Float4x4; }
template <>
inline DataType GetDataType<unsigned int>() { return DataTypes::UInt; }

END_INANITY_GRAPHICS

#endif
