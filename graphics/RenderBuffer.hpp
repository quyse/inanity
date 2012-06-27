#ifndef ___INANITY_GRAPHICS_RENDER_BUFFER_HPP___
#define ___INANITY_GRAPHICS_RENDER_BUFFER_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

/// Абстрактный класс рендербуфера.
class RenderBuffer : public Object
{
public:
	/// Получить текстуру, соответствующую рендербуферу.
	/** Может возвращать 0, если рендербуфер не имеет
	возможности быть источником данных. */
	virtual ptr<Texture> GetAsTexture() = 0;
};

END_INANITY_GRAPHICS

#endif
