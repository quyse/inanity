#ifndef ___INANITY_GRAPHICS_GL_ATTRIBUTE_BINDING_HPP___
#define ___INANITY_GRAPHICS_GL_ATTRIBUTE_BINDING_HPP___

#include "AttributeBinding.hpp"
#include "opengl.hpp"
#include <vector>

BEGIN_INANITY_GRAPHICS

/// Класс привязки атрибутов для OpenGL.
/** Привязка работает через заранее сделанный VertexArrayObject, если
поддерживается ARB_vertex_attrib_binding. Иначе все выставляется каждый
раз вручную. */
class GlAttributeBinding : public AttributeBinding
{
public:
	/// Элемент ручной разметки.
	struct Element
	{
		GLuint index;
		GLint size;
		GLenum type;
		GLboolean normalized;
		const GLvoid* pointer;
		bool integer;
	};
	typedef std::vector<Element> Elements;
	/// Слот ручной разметки.
	struct Slot
	{
		Elements elements;
		GLuint divisor;
	};
	typedef std::vector<Slot> Slots;

private:
	/// Vertex Array Object.
	/** Если используется ручная разметка, то 0. */
	GLuint vertexArrayName;
	/// "Ручная" разметка.
	std::vector<Slot> slots;

public:
	GlAttributeBinding(GLuint vertexArrayName);
	~GlAttributeBinding();

	GLuint GetVertexArrayName() const;
	Slots& GetSlots();
};

END_INANITY_GRAPHICS

#endif
