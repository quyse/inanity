#include "GlSamplerState.hpp"
#include "GlDevice.hpp"
#include "GlSystem.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

GlSamplerState::GlSamplerState(ptr<GlDevice> device, GLuint samplerName)
: device(device), samplerName(samplerName)
{}

GlSamplerState::~GlSamplerState()
{
	glDeleteSamplers(1, &samplerName);
}

GLint GlSamplerState::ConvertWrap(Wrap wrap)
{
	switch(wrap)
	{
	case wrapRepeat:
		return GL_REPEAT;
	case wrapRepeatMirror:
		return GL_MIRRORED_REPEAT;
	case wrapClamp:
		return GL_CLAMP_TO_EDGE;
	case wrapBorder:
		return GL_CLAMP_TO_BORDER;
	}
	THROW("Invalid wrap mode");
}

GLuint GlSamplerState::GetName()
{
	Update();
	return samplerName;
}

void GlSamplerState::Update()
{
	if(!dirty)
		return;

	try
	{
		// разобраться с фильтрацией уменьшения
		{
			GLint filterParam = -1;
			if(mipMapping)
				switch(minFilter)
				{
				case filterPoint:
					switch(mipFilter)
					{
					case filterPoint:
						filterParam = GL_NEAREST_MIPMAP_NEAREST;
						break;
					case filterLinear:
						filterParam = GL_NEAREST_MIPMAP_LINEAR;
						break;
					default:
						break;
					}
					break;
				case filterLinear:
					switch(mipFilter)
					{
					case filterPoint:
						filterParam = GL_LINEAR_MIPMAP_NEAREST;
						break;
					case filterLinear:
						filterParam = GL_LINEAR_MIPMAP_LINEAR;
						break;
					default:
						break;
					}
					break;
				default:
					break;
				}
			else
				switch(minFilter)
				{
				case filterPoint:
					filterParam = GL_NEAREST;
					break;
				case filterLinear:
					filterParam = GL_LINEAR;
					break;
				default:
					break;
				}
			if(filterParam == -1)
				THROW("Invalid min or mip filter param");
			glSamplerParameteri(samplerName, GL_TEXTURE_MIN_FILTER, filterParam);
			GlSystem::CheckErrors("Can't set texture min filter");
		}

		// разобраться с фильтрацией увеличения
		{
			GLint filterParam = -1;
			switch(magFilter)
			{
			case filterPoint:
				filterParam = GL_NEAREST;
				break;
			case filterLinear:
				filterParam = GL_LINEAR;
				break;
			default:
				break;
			}
			if(filterParam == -1)
				THROW("Invalid mag filter param");
			glSamplerParameteri(samplerName, GL_TEXTURE_MAG_FILTER, filterParam);
			GlSystem::CheckErrors("Can't set texture mag filter");
		}

		// установить режимы свёртки
		glSamplerParameteri(samplerName, GL_TEXTURE_WRAP_S, ConvertWrap(wrapU));
		GlSystem::CheckErrors("Can't set texture wrap U");
		glSamplerParameteri(samplerName, GL_TEXTURE_WRAP_T, ConvertWrap(wrapV));
		GlSystem::CheckErrors("Can't set texture wrap V");
		glSamplerParameteri(samplerName, GL_TEXTURE_WRAP_R, ConvertWrap(wrapW));
		GlSystem::CheckErrors("Can't set texture wrap W");

		// установить минимальный и максимальный LOD.
		glSamplerParameterf(samplerName, GL_TEXTURE_MIN_LOD, minLOD);
		GlSystem::CheckErrors("Can't set min LOD");
		glSamplerParameterf(samplerName, GL_TEXTURE_MAX_LOD, maxLOD);
		GlSystem::CheckErrors("Can't set max LOD");

		// установить цвет границы
		glSamplerParameterfv(samplerName, GL_TEXTURE_BORDER_COLOR, borderColor);
		GlSystem::CheckErrors("Can't set border color");

		dirty = false;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't update OpenGL sampler", exception);
	}
}

END_INANITY_GRAPHICS
