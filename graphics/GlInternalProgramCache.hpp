#ifndef ___INANITY_GRAPHICS_GL_INTERNAL_PROGRAM_CACHE_HPP___
#define ___INANITY_GRAPHICS_GL_INTERNAL_PROGRAM_CACHE_HPP___

#include "graphics.hpp"
#include <unordered_map>

BEGIN_INANITY_GRAPHICS

class GlInternalProgram;
class GlVertexShader;
class GlPixelShader;

/// Класс кэша программ OpenGL.
class GlInternalProgramCache : public Object
{
private:
	struct ProgramKey
	{
		GlVertexShader* vertexShader;
		GlPixelShader* pixelShader;
		ProgramKey(GlVertexShader* vertexShader, GlPixelShader* pixelShader);
		operator size_t() const;
	};
	typedef std::unordered_map<ProgramKey, ptr<GlInternalProgram> > Programs;
	Programs programs;

public:
	ptr<GlInternalProgram> GetProgram(GlVertexShader* vertexShader, GlPixelShader* pixelShader);
};

END_INANITY_GRAPHICS

#endif
