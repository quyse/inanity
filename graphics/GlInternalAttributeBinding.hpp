#ifndef ___INANITY_GRAPHICS_GL_INTERNAL_ATTRIBUTE_BINDING_HPP___
#define ___INANITY_GRAPHICS_GL_INTERNAL_ATTRIBUTE_BINDING_HPP___

#include "graphics.hpp"
#include "opengl.hpp"
#include <vector>

BEGIN_INANITY_GRAPHICS

/// Класс привязки VertexLayout'а.
class GlInternalAttributeBinding : public Object
{
public:
	/// Элемент разметки.
	struct Element
	{
		GLuint index;
		GLint size;
		GLenum type;
		GLboolean normalized;
		const GLvoid* pointer;
	};

private:
	std::vector<Element> elements;

public:
	std::vector<Element>& GetElements();
};

END_INANITY_GRAPHICS

#endif
