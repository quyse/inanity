#ifndef ___INANITY_GRAPHICS_GEOMETRY_SEMANTIC_HPP___
#define ___INANITY_GRAPHICS_GEOMETRY_SEMANTIC_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

/// Структура, задающая привязку (семантику) одного элемента геометрии.
/** Несколько семантик могут быть привязаны к разным элементам. */
struct GeometrySemantic
{
	/// Тип данных элемента.
	enum Type
	{
		typeFloat,
		typeInt,
		typeUint
	};
	unsigned char type;
	/// Количество элементов.
	unsigned char count;
	/// Смещение от начала данных вершины до элемента.
	unsigned short offset;

	GeometrySemantic() {}
	GeometrySemantic(unsigned char type, unsigned char count, unsigned short offset) : type(type), count(count), offset(offset) {}
};

END_INANITY_GRAPHICS

#endif
