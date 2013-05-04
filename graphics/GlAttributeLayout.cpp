#include "GlAttributeLayout.hpp"
#include "GlDevice.hpp"
#include "GlSystem.hpp"
#include "VertexLayout.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

GlAttributeLayout::GlAttributeLayout(ptr<GlDevice> device)
: device(device), vertexArrayName(0) {}

GlAttributeLayout::~GlAttributeLayout()
{
	if(vertexArrayName)
		glDeleteVertexArrays(1, &vertexArrayName);
}

GLuint GlAttributeLayout::GetVertexArrayName() const
{
	return vertexArrayName;
}

void GlAttributeLayout::Finalize()
{
	try
	{
		if(vertexArrayName)
			THROW_PRIMARY_EXCEPTION("Already finalized");

		// создать Vertex Array Object
		glGenVertexArrays(1, &vertexArrayName);
		GlSystem::CheckErrors("Can't gen vertex array");

		// задать все настройки для VAO

		// привязать VAO
		glBindVertexArray(vertexArrayName);
		GlSystem::CheckErrors("Can't bind vertex array");

		// задать элементы в VAO
		for(size_t i = 0; i < elements.size(); ++i)
		{
			const Element& element = elements[i];

			glEnableVertexAttribArray((GLuint)i);
			GlSystem::CheckErrors("Can't enable vertex attribute array");

			// выбрать формат и размер
			GLint size;
			GLenum type;
			switch(element.dataType)
			{
			case DataTypes::Float:
				size = 1;
				type = GL_FLOAT;
				break;
			case DataTypes::Float2:
				size = 2;
				type = GL_FLOAT;
				break;
			case DataTypes::Float3:
				size = 3;
				type = GL_FLOAT;
				break;
			case DataTypes::Float4:
				size = 4;
				type = GL_FLOAT;
				break;
			case DataTypes::Float4x4:
				THROW_PRIMARY_EXCEPTION("Matrices can't be used in attributes");
			case DataTypes::UInt:
				size = 1;
				type = GL_UNSIGNED_INT;
				break;
			case DataTypes::UInt2:
				size = 2;
				type = GL_UNSIGNED_INT;
				break;
			case DataTypes::UInt3:
				size = 3;
				type = GL_UNSIGNED_INT;
				break;
			case DataTypes::UInt4:
				size = 4;
				type = GL_UNSIGNED_INT;
				break;
			default:
				THROW_PRIMARY_EXCEPTION("Unknown attribute element type");
			}
			glVertexAttribFormat((GLuint)i, size, type, false, element.offset);
			GlSystem::CheckErrors("Can't set vertex attribute format");

			// указать слот для элемента
			glVertexAttribBinding((GLuint)i, element.slot);
		}

		// задать разделители для слотов
		for(size_t i = 0; i < slots.size(); ++i)
		{
			glVertexBindingDivisor((GLuint)i, slots[i].divisor);
			GlSystem::CheckErrors("Can't set vertex binding divisor");
		}

		// отвязать VAO
		glBindVertexArray(0);
		GlSystem::CheckErrors("Can't unbind vertex array");
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't finalize GL attribute layout", exception);
	}
}

END_INANITY_GRAPHICS
