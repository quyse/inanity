#ifndef ___INANITY_GRAPHICS_GLSL_SOURCE_HPP___
#define ___INANITY_GRAPHICS_GLSL_SOURCE_HPP___

#include "ShaderSource.hpp"
#include "../File.hpp"
#include "../String.hpp"
#include <vector>

BEGIN_INANITY

class File;
class InputStream;

END_INANITY

BEGIN_INANITY_GRAPHICS

class GlShaderBindings;

/// Класс исходного кода шейдера на языке GLSL.
class GlslSource : public ShaderSource
{
private:
	/// Исходный код на GLSL.
	ptr<File> code;
	/// Привязки для шейдера.
	ptr<GlShaderBindings> shaderBindings;

public:
	GlslSource(ptr<File> code, ptr<GlShaderBindings> shaderBindings);

	void Serialize(ptr<OutputStream> outputStream);
	static ptr<GlslSource> Deserialize(ptr<InputStream> inputStream);

	ptr<File> GetCode() const;
	ptr<GlShaderBindings> GetShaderBindings() const;
};

END_INANITY_GRAPHICS

#endif
