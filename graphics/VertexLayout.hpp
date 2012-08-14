#ifndef ___INANITY_GRAPHICS_VERTEX_LAYOUT_HPP___
#define ___INANITY_GRAPHICS_VERTEX_LAYOUT_HPP___

#include "graphics.hpp"
#include <vector>

BEGIN_INANITY_GRAPHICS

/// Класс разметки вершинных данных.
/** Описывает, что и где валяется в вершинном буфере. */
class VertexLayout : public Object
{
public:
	/// Типы данных.
	enum Type
	{
		typeFloat,
		typeFloat2,
		typeFloat3,
		typeFloat4,
		typeFloat4x4,
		typeUInt,
		typeUInt2,
		typeUInt3,
		typeUInt4
	};
	/// Структура элемента вершины.
	struct Element
	{
		/// Тип данных элемента.
		Type type;
		/// Смещение от начала вершины.
		int offset;
		/// Семантика элемента.
		/** Это просто число, которое имеет смысл положения, текстурных координат
		или нормали. */
		int semantic;
	};

private:
	int stride;
	std::vector<Element> elements;

public:
	VertexLayout(int stride);

	std::vector<Element>& GetElements();
	int GetStride() const;
};

END_INANITY_GRAPHICS

#endif
