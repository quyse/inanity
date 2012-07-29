#ifndef ___INANITY_DEPTH_STENCIL_BUFFER_HPP___
#define ___INANITY_DEPTH_STENCIL_BUFFER_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

class Texture;

/// Абстрактный класс буфера глубины-трафарета.
class DepthStencilBuffer : public Object
{
public:
	/// Получить текстуру, соответствующую буферу.
	/** Может возвращать 0, если буфер не имеет
	возможности быть источником данных. */
	virtual ptr<Texture> GetTexture() = 0;
};

END_INANITY_GRAPHICS

#endif
