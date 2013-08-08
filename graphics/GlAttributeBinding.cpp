#include "GlAttributeBinding.hpp"
#include "GlDevice.hpp"
#include "VertexLayout.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

GlAttributeBinding::GlAttributeBinding(ptr<GlDevice> device, GLuint vertexArrayName)
: device(device), vertexArrayName(vertexArrayName) {}

GlAttributeBinding::~GlAttributeBinding()
{
	if(vertexArrayName)
		glDeleteVertexArrays(1, &vertexArrayName);
}

GLuint GlAttributeBinding::GetVertexArrayName() const
{
	return vertexArrayName;
}

GlAttributeBinding::Slots& GlAttributeBinding::GetSlots()
{
	return slots;
}

END_INANITY_GRAPHICS
