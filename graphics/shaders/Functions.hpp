#ifndef ___INANITY_SHADERS_FUNCTIONS_HPP___
#define ___INANITY_SHADERS_FUNCTIONS_HPP___

#include "shaders.hpp"
#include "Language.hpp"

/* Файл содержит определения функций из HLSL/GLSL, для их лёгкого использования
при написании шейдеров.
*/

/// Макрос для опреде

/// Макрос для определения простой функции 2-х аргументов.
#define IYS_FUN_SIMPLE1(name) \
	template <typename DataType> \
	DataPtr<DataType> name(DataPtr<DataType> a) \
	{ \
		return NEW(Call1Data<DataType, DataType>(#name, a)); \
	}

/// Макрос для определения простой функции 2-х аргументов.
#define IYS_FUN_SIMPLE2(name) \
	template <typename DataType> \
	DataPtr<DataType> name(DataPtr<DataType> a, DataPtr<DataType> b) \
	{ \
		return NEW(Call2Data<DataType, DataType, DataType>(#name, a, b)); \
	}

/// Макрос для определения простой функции 3-х аргументов.
#define IYS_FUN_SIMPLE3(name) \
	template <typename DataType> \
	DataPtr<DataType> name(DataPtr<DataType> a, DataPtr<DataType> b, DataPtr<DataType> c) \
	{ \
		return NEW(Call3Data<DataType, DataType, DataType, DataType>(#name, a, b, c)); \
	}

BEGIN_INANITY_SHADERS

template <int n>
float_ length(DataPtr<vector<n> > a)
{
	return NEW(Call1Data<float, vector<n> >("length", a));
}
template <int n>
float_ distance(DataPtr<vector<n> > a, DataPtr<vector<n> > b)
{
	return NEW(Call2Data<float, vector<n>, vector<n> >("distance", a, b));
}
template <int n>
DataPtr<vector<n> > normalize(DataPtr<vector<n> > a)
{
	return NEW(Call1Data<vector<n>, vector<n> >("normalize", a));
}

IYS_FUN_SIMPLE1(sin)
IYS_FUN_SIMPLE1(cos)
IYS_FUN_SIMPLE1(tan)
IYS_FUN_SIMPLE1(exp)
IYS_FUN_SIMPLE1(exp2)
IYS_FUN_SIMPLE2(pow)

IYS_FUN_SIMPLE1(abs)
IYS_FUN_SIMPLE2(min)
IYS_FUN_SIMPLE2(max)
IYS_FUN_SIMPLE1(floor)
IYS_FUN_SIMPLE1(ceil)
IYS_FUN_SIMPLE3(clamp)
IYS_FUN_SIMPLE3(lerp)

template <int n>
float_ dot(DataPtr<vector<n> > a, DataPtr<vector<n> > b)
{
	return NEW(Call2Data<float, vector<n>, vector<n> >("dot", a, b));
}
float3_ cross(float3_ a, float3_ b)
{
	return NEW(Call2Data<float3, float3, float3>("cross", a, b));
}

END_INANITY_SHADERS

#undef IYS_FUN_SIMPLE2
#undef IYS_FUN_SIMPLE3

#endif
