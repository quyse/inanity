#ifndef ___INANITY_GRAPHICS_SHADER_SOURCE_HPP___
#define ___INANITY_GRAPHICS_SHADER_SOURCE_HPP___

#include "graphics.hpp"

BEGIN_INANITY

class OutputStream;

END_INANITY

BEGIN_INANITY_GRAPHICS

/// Абстракнтый класс исходного кода шейдера.
class ShaderSource : public Object
{
public:
	virtual void Serialize(ptr<OutputStream> outputStream) = 0;
};

END_INANITY_GRAPHICS

#endif
