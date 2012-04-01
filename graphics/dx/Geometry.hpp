#ifndef ___INANITY_DX_GEOMETRY_HPP___
#define ___INANITY_DX_GEOMETRY_HPP___

#include "dx.hpp"
#include "d3d.hpp"
#include "../../String.hpp"

BEGIN_INANITY

class File;
class OutputStream;
class ResourceLoader;

END_INANITY

BEGIN_INANITY_GRAPHICS

class GeometryFormat;

END_INANITY_GRAPHICS

BEGIN_INANITY_DX

class Buffer;

/// Класс геометрии.
class Geometry : public Object
{
private:
	ptr<Buffer> vertexBuffer;
	size_t vertexStride;
	ptr<Buffer> indexBuffer;
	DXGI_FORMAT indexFormat;
	size_t indicesCount;
	ptr<GeometryFormat> format;

public:
	Geometry(ptr<Buffer> vertexBuffer, unsigned vertexStride, ptr<Buffer> indexBuffer, DXGI_FORMAT indexFormat, size_t indicesCount, ptr<GeometryFormat> format = nullptr);

	ptr<Buffer> GetVertexBuffer() const;
	size_t GetVertexStride() const;
	ptr<Buffer> GetIndexBuffer() const;
	DXGI_FORMAT GetIndexFormat() const;
	/// Получить количество индексов.
	/** Если геометрия не индексирована, то это количество вершин. */
	unsigned GetIndicesCount() const;
	bool IsIndexed() const;
	ptr<GeometryFormat> GetFormat() const;

	static ptr<Geometry> Deserialize(ptr<File> file, ResourceLoader* resourceLoader);
	static ptr<Geometry> Deserialize(ptr<File> file, ptr<GeometryFormat> geometryFormat);
};

END_INANITY_DX

#endif
