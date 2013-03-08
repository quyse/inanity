#include "GlShaderCompiler.hpp"
#include "GlslSource.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

ptr<File> GlShaderCompiler::Compile(ptr<ShaderSource> shaderSource)
{
	try
	{
		// получить код на GLSL
		ptr<GlslSource> glslSource = shaderSource.FastCast<GlslSource>();
		if(!glslSource)
			THROW_PRIMARY_EXCEPTION("Shader source is not GLSL");

		// бинарные шейдеры OpenGL привязаны к архитектуре
		// пока ничего не компилируем, просто возвращаем код
		return glslSource->GetCode();
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't compile GLSL shader source", exception);
	}
}
