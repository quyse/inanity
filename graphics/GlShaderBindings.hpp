#ifndef ___INANITY_GRAPHICS_GL_SHADER_BINDINGS_HPP___
#define ___INANITY_GRAPHICS_GL_SHADER_BINDINGS_HPP___

#include "graphics.hpp"
#include "../String.hpp"
#include <vector>

BEGIN_INANITY

class StreamWriter;
class StreamReader;

END_INANITY

BEGIN_INANITY_GRAPHICS

/// Класс, хранящий различные привязки, необходимые для инициализации OpenGL-шейдера.
class GlShaderBindings : public Object
{
public:
	typedef std::vector<std::pair<String, int> > Bindings;

private:
	Bindings uniformBlockBindings;
	Bindings samplerBindings;
	Bindings attributeBindings;
	Bindings targetBindings;

	static void WriteBindings(StreamWriter& writer, const Bindings& bindings);
	static void ReadBindings(StreamReader& reader, Bindings& bindings);

public:
	GlShaderBindings(
		const Bindings& uniformBlockBindings,
		const Bindings& samplerBindings,
		const Bindings& attributeBindings,
		const Bindings& targetBindings
	);

	const Bindings& GetUniformBlockBindings() const;
	const Bindings& GetSamplerBindings() const;
	const Bindings& GetAttributeBindings() const;
	const Bindings& GetTargetBindings() const;

	void Serialize(StreamWriter& writer);
	static ptr<GlShaderBindings> Deserialize(StreamReader& reader);
};

END_INANITY_GRAPHICS

#endif
