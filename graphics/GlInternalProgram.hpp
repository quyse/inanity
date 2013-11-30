#ifndef ___INANITY_GRAPHICS_GL_INTERNAL_PROGRAM_HPP___
#define ___INANITY_GRAPHICS_GL_INTERNAL_PROGRAM_HPP___

#include "DataType.hpp"
#include "opengl.hpp"
#include <vector>

BEGIN_INANITY_GRAPHICS

class GlDevice;

/// Класс, инкапсулирующий слинкованную программу OpenGL.
/** Из программ составляется кэш. */
class GlInternalProgram : public Object
{
public:
	/// Non-buffer uniform binding.
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
		/// OpenGL uniform location.
		int location;
	};
	typedef std::vector<UniformBinding> UniformBindings;

private:
	ptr<GlDevice> device;
	GLuint name;

	/// Bindings of non-buffer uniforms.
	std::vector<UniformBinding> uniformBindings;

public:
	GlInternalProgram(ptr<GlDevice> device, GLuint name, const UniformBindings& uniformBindings);
	~GlInternalProgram();

	GLuint GetName() const;
	const UniformBindings& GetUniformBindings() const;
};

END_INANITY_GRAPHICS

#endif
