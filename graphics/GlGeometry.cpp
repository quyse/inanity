#include "GlGeometry.hpp"
#include "GlVertexBuffer.hpp"
#include "GlIndexBuffer.hpp"

GlGeometry::GlGeometry(ptr<GlVertexBuffer> vertexBuffer, ptr<GlIndexBuffer> indexBuffer, GLuint vertexArrayName)
: vertexBuffer(vertexBuffer), indexBuffer(indexBuffer), vertexArrayName(vertexArrayName) {}

GlGeometry::~GlGeometry()
{
	glDeleteVertexArrays(1, &vertexArrayName);
}

int GlGeometry::GetIndicesType() const
{
	switch(indexBuffer->GetIndexSize())
	{
	case 1:
		return GL_UNSIGNED_BYTE;
	case 2:
		return GL_UNSIGNED_SHORT;
	case 4:
		return GL_UNSIGNED_INT;
	default:
		return 0;
	}
}

GLuint GlGeometry::GetVertexArrayName() const
{
	return vertexArrayName;
}

void GlGeometry::IssueDraw()
{
	if(indexBuffer)
		glDrawElements(GL_TRIANGLES, indexBuffer->GetIndicesCount(), GetIndicesType(), (void*)0);
	else
		glDrawArrays(GL_TRIANGLES, 0, vertexBuffer->GetVerticesCount());
}

void GlGeometry::IssueDrawInstanced(int instancesCount)
{
	if(indexBuffer)
		glDrawElementsInstanced(GL_TRIANGLES, indexBuffer->GetIndicesCount(), GetIndicesType(), (void*)0, instancesCount);
	else
		glDrawArraysInstanced(GL_TRIANGLES, 0, vertexBuffer->GetVerticesCount(), instancesCount);
}
