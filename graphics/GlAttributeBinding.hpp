#ifndef ___INANITY_GRAPHICS_GL_ATTRIBUTE_BINDING_HPP___
#define ___INANITY_GRAPHICS_GL_ATTRIBUTE_BINDING_HPP___

#include "AttributeBinding.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

class GlDevice;

/// Класс привязки атрибутов для OpenGL.
class GlAttributeBinding : public AttributeBinding
{
private:
	ptr<GlDevice> device;
	/// Vertex Array Object.
	GLuint vertexArrayName;

public:
	GlAttributeBinding(ptr<GlDevice> device, GLuint vertexArrayName);
	~GlAttributeBinding();

	GLuint GetVertexArrayName() const;
};

END_INANITY_GRAPHICS

#endif
