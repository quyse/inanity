#include "GlAttributeLayout.hpp"

BEGIN_INANITY_GRAPHICS

GlAttributeLayout::GlAttributeLayout(GLuint vertexArrayName)
: vertexArrayName(vertexArrayName) {}

GlAttributeLayout::~GlAttributeLayout()
{
	glDeleteVertexArrays(1, &vertexArrayName);
}

GLuint GlAttributeLayout::GetVertexArrayName() const
{
	return vertexArrayName;
}

END_INANITY_GRAPHICS
