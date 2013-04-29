#ifndef ___INANITY_GRAPHICS_GL_INTERNAL_PROGRAM_CACHE_HPP___
#define ___INANITY_GRAPHICS_GL_INTERNAL_PROGRAM_CACHE_HPP___

#include "graphics.hpp"
#include "opengl.hpp"
#include <unordered_map>

BEGIN_INANITY_GRAPHICS

class GlInternalProgram;
class GlVertexShader;
class GlPixelShader;
class GlShaderBindings;

struct GlInternalProgramKey
{
	GlVertexShader* vertexShader;
	GlPixelShader* pixelShader;
	GlInternalProgramKey(GlVertexShader* vertexShader, GlPixelShader* pixelShader);
	friend bool operator==(const GlInternalProgramKey& a, const GlInternalProgramKey& b);
};

END_INANITY_GRAPHICS

namespace std
{
	template <>
	struct hash<Inanity::Graphics::GlInternalProgramKey>
	{
		size_t operator()(const Inanity::Graphics::GlInternalProgramKey& key) const;
	};
}

BEGIN_INANITY_GRAPHICS

/// Класс кэша программ OpenGL.
class GlInternalProgramCache : public Object
{
private:
	typedef std::unordered_map<GlInternalProgramKey, ptr<GlInternalProgram> > Programs;
	Programs programs;

	static void ApplyPreLinkBindings(GLuint programName, ptr<GlShaderBindings> shaderBindings);
	static void ApplyPostLinkBindings(GLuint programName, ptr<GlShaderBindings> shaderBindings);

public:
	ptr<GlInternalProgram> GetProgram(GlVertexShader* vertexShader, GlPixelShader* pixelShader);
};

END_INANITY_GRAPHICS

#endif
