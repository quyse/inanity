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

		//******* Специальные семантики.

		_SpecialBegin,

		/// Номер инстанса.
		Instance = _SpecialBegin,
		/// Координаты вершины из вершинного шейдера для растеризации.
		VertexPosition,
		/// Цвет результирующего пиксела (для первого рендербуфера).
		_TargetColorBegin,
		TargetColor0 = _TargetColorBegin,
		// ...
		_TargetColorEnd = _TargetColorBegin + 8,
		/// Глубина результирующего пиксела (для первого рендербуфера).
		_TargetDepthBegin = _TargetColorEnd,
		TargetDepth0 = _TargetDepthBegin,
		// ...
		_TargetDepthEnd = _TargetDepthBegin + 8,

		_SpecialEnd = _TargetDepthEnd,

		//******* Пользовательские семантики.
		/* Для атрибутов: Custom0 + номер атрибутной семантики. */

		_CustomBegin = _SpecialEnd,

		Custom0 = _CustomBegin,

		/// Положение вершины.
		CustomPosition = Custom0,
		/// Нормаль вершины.
		CustomNormal,
		/// Бинормаль.
		CustomBinormal,
		/// Тангента,
		CustomTangent,
		/// Текстурные координаты вершины
		CustomTexcoord0,
		// ...
		/// Коэффициенты локальной трансформации (переводящей из пространства модели в пространство
		CustomLocalTransform = CustomTexcoord0 + 8,

		_CustomEnd = _CustomBegin + 64
	};
};
typedef Semantics::_ Semantic;

END_INANITY_SHADERS

#endif
