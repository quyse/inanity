#ifndef ___INANITY_GRAPHICS_GL_GEOMETRY_HPP___
#define ___INANITY_GRAPHICS_GL_GEOMETRY_HPP___

#include "Geometry.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

class GlVertexBuffer;
class GlIndexBuffer;

class GlGeometry : public Geometry
{
private:
	/// Вершинный буфер.
	ptr<GlVertexBuffer> vertexBuffer;
	/// Индексный буфер.
	ptr<GlIndexBuffer> indexBuffer;
	/// Vertex Array Object.
	GLuint vertexArrayName;

	int GetIndicesType() const;

public:
	GlGeometry(ptr<GlVertexBuffer> vertexBuffer, ptr<GlIndexBuffer> indexBuffer, GLuint vertexArrayName);
	~GlGeometry();

	GLuint GetVertexArrayName() const;

	void IssueDraw();
	void IssueDrawInstanced(int instancesCount);
};

END_INANITY_GRAPHICS

#endif
