#ifndef ___INANITY_GRAPHICS_SHADERS_FUNCTIONS_HPP___
#define ___INANITY_GRAPHICS_SHADERS_FUNCTIONS_HPP___

#include "Language.hpp"

/* Файл содержит определения функций, которые можно
использовать при написании шейдеров.
Функции должны иметь реализации в соответствующих
языковых генераторах.
*/

BEGIN_INANITY_SHADERS

namespace F
{
	//******* Макросы для объявлений функций.
#define FUN1(name) \
	Expression name(Expression a) { \
		return NEW(Call1ExpressionObject(#name, a)); }
#define FUN2(name) \
	Expression name(Expression a, Expression b) { \
		return NEW(Call2ExpressionObject(#name, a, b)); }
#define FUN3(name) \
	Expression name(Expression a, Expression b, Expression c) { \
		return NEW(Call3ExpressionObject(#name, a, b, c)); }
#define FUN4(name) \
	Expression name(Expression a, Expression b, Expression c, Expression d) { \
		return NEW(Call4ExpressionObject(#name, a, b, c, d)); }

	//******* Собственно функции.

	FUN1(length);
	FUN2(distance);
	FUN1(normalize);
	FUN1(sin);
	FUN1(cos);
	FUN1(tan);
	FUN1(exp);
	FUN1(exp2);
	FUN2(pow);

	FUN1(abs);
	FUN2(min);
	FUN2(max);
	FUN1(floor);
	FUN1(ceil);
	FUN3(clamp);
	FUN3(lerp);

	FUN2(dot);
	FUN2(cross);

	/// Умножение матриц на векторы.
	FUN2(mul);

	/// Получить семпл из семплера.
	/** Параметры - семплер и координаты. */
	FUN2(sample);

	// конструкторы векторов
	FUN4(float4);
	FUN2(float4_xyz_w);
	FUN3(float4_xy_z_w);
	FUN3(fLoat3);
	FUN2(float2);
	// преобразование к матрице
	FUN1(cast4x4to3x3)

	//******* отменяем макросы
#undef FUN1
#undef FUN2
#undef FUN3
#undef FUN4
};

END_INANITY_SHADERS

#endif
