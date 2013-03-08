#ifndef ___INANITY_GRAPHICS_GLSL_SOURCE_HPP___
#define ___INANITY_GRAPHICS_GLSL_SOURCE_HPP___

#include "ShaderSource.hpp"
#include "../File.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_GRAPHICS

/// Класс исходного кода шейдера на языке GLSL.
class GlslSource : public ShaderSource
{
private:
	ptr<File> code;

public:
	GlslSource(ptr<File> code);

	void Serialize(ptr<OutputStream> outputStream);

	ptr<File> GetCode() const;
};

END_INANITY_GRAPHICS

#endif
