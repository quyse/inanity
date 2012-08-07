#include "GlInternalProgramCache.hpp"
#include "GlInternalProgram.hpp"
#include "GlVertexShader.hpp"
#include "GlPixelShader.hpp"
#include "GlSystem.hpp"
#include "../Exception.hpp"
#include "opengl.hpp"

GlInternalProgramCache::ProgramKey::ProgramKey(GlVertexShader* vertexShader, GlPixelShader* pixelShader)
: vertexShader(vertexShader), pixelShader(pixelShader) {}

GlInternalProgramCache::ProgramKey::operator size_t() const
{
	return size_t(vertexShader) ^ size_t(pixelShader);
}

ptr<GlInternalProgram> GlInternalProgramCache::GetProgram(GlVertexShader* vertexShader, GlPixelShader* pixelShader)
{
	ProgramKey key(vertexShader, pixelShader);
	Programs::const_iterator i = programs.find(key);
	if(i != programs.end())
		return i->second;

	// создать программу
	GLuint programName = glCreateProgram();
	GlSystem::CheckErrors("Can't create program");
	// присоединить шейдеры
	if(vertexShader)
	{
		glAttachShader(programName, vertexShader->GetShaderName());
		GlSystem::CheckErrors("Can't attach vertex shader");
	}
	if(pixelShader)
	{
		glAttachShader(programName, pixelShader->GetShaderName());
		GlSystem::CheckErrors("Can't attach pixel shader");
	}
	// слинковать программу
	glLinkProgram(programName);
	{
		GLint status;
		glGetProgramiv(programName, GL_LINK_STATUS, &status);
		if(status != GL_TRUE)
		{
			// получить лог ошибок
			GLint logLength;
			glGetProgramiv(programName, GL_INFO_LOG_LENGTH, &logLength);
			String log(logLength, ' ');
			glGetProgramInfoLog(programName, logLength, &logLength, &*log.begin());
			log.resize(logLength);

			// очистить ошибки, на всякий случай
			GlSystem::ClearErrors();

			// выбросить ошибку
			THROW_PRIMARY_EXCEPTION("Can't link program:\n" + log);
		}
	}

	// записать программу
	ptr<GlInternalProgram> program = NEW(GlInternalProgram(programName));
	programs[key] = program;

	return program;
}
