#ifndef ___INANITY_EDITABLE_GEOMETRY_HPP___
#define ___INANITY_EDITABLE_GEOMETRY_HPP___

#include "graphics.hpp"
#include "../String.hpp"
#include "../math/basic.hpp"
#include <map>

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_GRAPHICS

using namespace Inanity::Math;

enum VertexMapping
{
	vertexMappingPosition,
	vertexMappingNormal,
	vertexMappingTexcoord,
	vertexMappingTangent,
	vertexMappingBinormal
};

struct ShadowVertex
{
	vec3 position;
	vec3 normal;
};

struct TangentVertex
{
	vec3 position;
	vec2 texcoord;
	vec3 transform1;
	vec3 transform2;
	vec3 transform3;
};

/// Класс редактируемой геометрии.
class EditableGeometry : public Object
{
private:
	ptr<File> vertices;
	size_t vertexStride;
	ptr<File> indices;
	size_t indexStride;
	std::map<VertexMapping, ptrdiff_t> mappings;
	String format;

public:
	EditableGeometry(ptr<File> vertices, size_t vertexStride, ptr<File> indices, size_t indexStride, const String& format);

	ptr<File> GetVertices() const;
	size_t GetVertexStride() const;
	ptr<File> GetIndices() const;
	size_t GetIndexStride() const;
	String GetFormat() const;

	/// Получить подгеометрию.
	ptr<EditableGeometry> Subgeometry(size_t startIndex, size_t indicesCount) const;

	void AddVertexMapping(VertexMapping mapping, ptrdiff_t offset);
	/// Сократить индексы.
	/** Сокращает размер индекса, если это возможно, и если он уже не короткий.
	Допускается сокращение только от unsigned до short. Если сократить нельзя,
	возвращает копию существующей геометрии. */
	ptr<EditableGeometry> ShortIndices() const;

	ptr<EditableGeometry> CreateShadowGeometry() const;
	ptr<EditableGeometry> CreateTangentGeometry(bool smooth, const String& format) const;
};

END_INANITY_GRAPHICS

#endif
