#ifndef ___INANITY_GRAPHICS_SHADER_SEMANTIC_HPP___
#define ___INANITY_GRAPHICS_SHADER_SEMANTIC_HPP___

#include "shaders.hpp"

BEGIN_INANITY_SHADERS

/// Поддерживаемые семантики.
struct Semantics
{
	enum _
	{
		/// Нет специальной семантики.
		None,
		/// Номер инстанса.
		Instance,
		/// Координаты вершины из вершинного шейдера для растеризации.
		VertexPosition,
		/// Цвет результирующего пиксела.
		PixelColor,
		/// Глубина результирующего пиксела.
		TargetDepth,
		_Count
	};
};
typedef Semantics::_ Semantic;

END_INANITY_SHADERS

#endif
