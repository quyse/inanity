#include "Geometry.hpp"
#include "../GeometryFormat.hpp"
#include "Buffer.hpp"
#include "System.hpp"
#include "../../PartFile.hpp"
#include "../../FileInputStream.hpp"
#include "../../OutputStream.hpp"
#include "../../StreamReader.hpp"
#include "../../StreamWriter.hpp"
#include "../../ResourceLoader.hpp"
#include "../../ResourceManager.hpp"
#include "../../Exception.hpp"

Geometry::Geometry(ptr<Buffer> vertexBuffer, unsigned vertexStride, ptr<Buffer> indexBuffer, DXGI_FORMAT indexFormat, size_t indicesCount, ptr<GeometryFormat> format)
: vertexBuffer(vertexBuffer), vertexStride(vertexStride), indexBuffer(indexBuffer), indexFormat(indexFormat), indicesCount(indicesCount), format(format)
{
}

ptr<Buffer> Geometry::GetVertexBuffer() const
{
	return vertexBuffer;
}

size_t Geometry::GetVertexStride() const
{
	return vertexStride;
}

ptr<Buffer> Geometry::GetIndexBuffer() const
{
	return indexBuffer;
}

DXGI_FORMAT Geometry::GetIndexFormat() const
{
	return indexFormat;
}

unsigned Geometry::GetIndicesCount() const
{
	return indicesCount;
}

bool Geometry::IsIndexed() const
{
	return indexBuffer;
}

ptr<GeometryFormat> Geometry::GetFormat() const
{
	return format;
}

ptr<Geometry> Geometry::Deserialize(ptr<File> file, ResourceLoader* resourceLoader)
{
	try
	{
		ptr<StreamReader> reader = NEW(StreamReader(NEW(FileInputStream(file))));

		size_t verticesCount = reader->ReadShortly();
		size_t vertexStride = reader->ReadShortly();
		size_t verticesSize = verticesCount * vertexStride;

		size_t indicesCount = reader->ReadShortly();
		size_t indexStride = 0;
		if(indicesCount)
		{
			if(indicesCount % 3)
				THROW_PRIMARY_EXCEPTION("Invalid index stride");
			indexStride = reader->ReadShortly();
		}
		else
		{
			if(verticesCount % 3)
				THROW_PRIMARY_EXCEPTION("Invalid vertex faces");
		}
		size_t indicesSize = indicesCount * indexStride;

		// считать формат геометрии
		ptr<GeometryFormat> format = resourceLoader->LoadResource<GeometryFormat>(reader->ReadString());

		char* fileData = (char*)file->GetData();
		// считать вершины
		size_t offset = reader->GetReadSize();
		size_t size = file->GetSize() - offset;
		if(verticesSize > size)
			THROW_PRIMARY_EXCEPTION("Can't read vertices");
		ptr<File> vertices = NEW(PartFile(file, fileData + offset, verticesSize));

		// считать индексы
		ptr<File> indices = 0;
		if(indicesSize)
		{
			offset += verticesSize;
			size -= verticesSize;
			if(indicesSize > size)
				THROW_PRIMARY_EXCEPTION("Can't read indices");
			indices = NEW(PartFile(file, fileData + offset, indicesSize));
		}

		DXGI_FORMAT indexFormat;
		if(indices)
			switch(indexStride)
			{
			case 0:
				indexFormat = DXGI_FORMAT_UNKNOWN;
				break;
			case 2:
				indexFormat = DXGI_FORMAT_R16_UINT;
				break;
			case 4:
				indexFormat = DXGI_FORMAT_R32_UINT;
				break;
			default:
				THROW_PRIMARY_EXCEPTION("Unsupported index stride (format)");
			}
		else
			indexFormat = DXGI_FORMAT_UNKNOWN;

		ptr<System> system = resourceLoader->GetStaticResource<System>();

		ptr<Buffer> vertexBuffer;
		{
			D3D11_BUFFER_DESC desc;
			desc.ByteWidth = (UINT)vertices->GetSize();
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA data;
			data.pSysMem = vertices->GetData();

			ID3D11Buffer* vertexBufferInterface;
			if(FAILED(system->GetDevice()->CreateBuffer(&desc, &data, &vertexBufferInterface)))
				THROW_PRIMARY_EXCEPTION("Can't create vertex buffer");

			vertexBuffer = NEW(Buffer(vertexBufferInterface, vertices->GetSize()));
		}

		ptr<Buffer> indexBuffer;
		if(indexFormat != DXGI_FORMAT_UNKNOWN)
		{
			D3D11_BUFFER_DESC desc;
			desc.ByteWidth = (UINT)indices->GetSize();
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA data;
			data.pSysMem = indices->GetData();

			ID3D11Buffer* indexBufferInterface;
			if(FAILED(system->GetDevice()->CreateBuffer(&desc, &data, &indexBufferInterface)))
				THROW_PRIMARY_EXCEPTION("Can't create index buffer");

			indexBuffer = NEW(Buffer(indexBufferInterface, indices->GetSize()));
		}
		else
			indexBuffer = 0;

		return NEW(Geometry(vertexBuffer, vertexStride, indexBuffer, indexFormat,
			(unsigned)(indexFormat == DXGI_FORMAT_UNKNOWN ? verticesCount : indicesCount),
			format));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't load geometry", exception);
	}
}
