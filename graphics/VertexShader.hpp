#ifndef ___INANITY_GRAPHICS_VERTEX_SHADER_HPP___
#define ___INANITY_GRAPHICS_VERTEX_SHADER_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

/// Абстрактный класс вершинного шейдера.
class VertexShader : public Object
{
public:
	/// Получить разметку атрибутов.
	virtual ptr<Layout> GetAttributesLayout() = 0;
};

END_INANITY_GRAPHICS

#endif
