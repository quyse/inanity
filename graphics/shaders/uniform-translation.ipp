#ifndef ___INANITY_GRAPHICS_SHADERS_UNIFORM_TRANSLATION_IPP___
#define ___INANITY_GRAPHICS_SHADERS_UNIFORM_TRANSLATION_IPP___

#include "shaders.hpp"

/* Функции для передачи данных в GPU. */

BEGIN_INANITY_SHADERS

/// Скопировать данные переменной.
template <typename ValueType>
inline void CopyUniformData(ValueType& data, const ValueType& v)
{
	data = v;
}
// Для матриц требуется транспонирование.
template <>
inline void CopyUniformData(float4x4& data, const float4x4& v)
{
	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			data.t[i][j] = v.t[j][i];
}

END_INANITY_SHADERS

#endif
