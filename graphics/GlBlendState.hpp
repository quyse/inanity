#ifndef ___INANITY_GRAPHICS_GL_BLEND_STATE_HPP___
#define ___INANITY_GRAPHICS_GL_BLEND_STATE_HPP___

#include "BlendState.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс настроек смешивания OpenGL.
class GlBlendState : public BlendState
{
private:
	static GLenum ConvertColorSource(ColorSource colorSource);
	static GLenum ConvertAlphaSource(AlphaSource alphaSource);
	static GLenum ConvertOperation(Operation operation);
public:
	/// Применить настройки.
	void Apply();
};

END_INANITY_GRAPHICS

#endif
