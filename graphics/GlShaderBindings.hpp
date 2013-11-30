#ifndef ___INANITY_GRAPHICS_GL_SHADER_BINDINGS_HPP___
#define ___INANITY_GRAPHICS_GL_SHADER_BINDINGS_HPP___

#include "graphics.hpp"
#include "DataType.hpp"
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
	struct UniformBinding
	{
		/// Type of uniform.
		DataType dataType;
		/// Size of an array (1 if not an array).
		int count;
		/// Number of (virtual) uniform buffer slot.
		int slot;
		/// Offset into uniform buffer.
		int offset;
		/// Name of the uniform.
		String name;
	};
	typedef std::vector<UniformBinding> UniformBindings;
	typedef std::vector<std::pair<String, int> > Bindings;

private:
	UniformBindings uniformBindings;
	Bindings uniformBlockBindings;
	Bindings samplerBindings;
	Bindings attributeBindings;
	Bindings targetBindings;

	static void WriteBindings(StreamWriter& writer, const Bindings& bindings);
	static void ReadBindings(StreamReader& reader, Bindings& bindings);

public:
	GlShaderBindings(
		const UniformBindings& uniformBindings,
		const Bindings& uniformBlockBindings,
		const Bindings& samplerBindings,
		const Bindings& attributeBindings,
		const Bindings& targetBindings
	);

	const UniformBindings& GetUniformBindings() const;
	const Bindings& GetUniformBlockBindings() const;
	const Bindings& GetSamplerBindings() const;
	const Bindings& GetAttributeBindings() const;
	const Bindings& GetTargetBindings() const;

	void Serialize(StreamWriter& writer);
	static ptr<GlShaderBindings> Deserialize(StreamReader& reader);
};

END_INANITY_GRAPHICS

#endif
