#ifndef ___INANITY_GRAPHICS_GL_ATTRIBUTE_LAYOUT_HPP___
#define ___INANITY_GRAPHICS_GL_ATTRIBUTE_LAYOUT_HPP___

#include "AttributeLayout.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

class GlDevice;

/// Класс разметки атрибутов для OpenGL.
class GlAttributeLayout : public AttributeLayout
{
private:
	ptr<GlDevice> device;
	/// Vertex Array Object.
	GLuint vertexArrayName;

public:
	GlAttributeLayout(ptr<GlDevice> device);
	~GlAttributeLayout();

	GLuint GetVertexArrayName() const;

	void Finalize();
};

END_INANITY_GRAPHICS

#endif
