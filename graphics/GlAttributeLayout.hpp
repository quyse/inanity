#ifndef ___INANITY_GRAPHICS_GL_ATTRIBUTE_LAYOUT_HPP___
#define ___INANITY_GRAPHICS_GL_ATTRIBUTE_LAYOUT_HPP___

#include "AttributeLayout.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

class GlAttributeLayout : public AttributeLayout
{
private:
	/// Vertex Array Object.
	GLuint vertexArrayName;

public:
	GlAttributeLayout(GLuint vertexArrayName);
	~GlAttributeLayout();

	GLuint GetVertexArrayName() const;
};

END_INANITY_GRAPHICS

#endif
