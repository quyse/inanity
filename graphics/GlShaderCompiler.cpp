#include "GlShaderCompiler.hpp"
#include "GlslSource.hpp"
#include "../MemoryStream.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

ptr<File> GlShaderCompiler::Compile(ptr<ShaderSource> shaderSource)
{
	try
	{
		// получить код на GLSL
		ptr<GlslSource> glslSource = shaderSource.FastCast<GlslSource>();
		if(!glslSource)
			THROW("Shader source is not GLSL");

		// сериализуем исходник шейдера в файл
		ptr<MemoryStream> stream = NEW(MemoryStream());
		glslSource->Serialize(stream);
		return stream->ToFile();
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't compile GLSL shader source", exception);
	}
}

END_INANITY_GRAPHICS
