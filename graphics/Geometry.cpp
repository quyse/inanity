#include "Geometry.hpp"
#include "GeometryFormat.hpp"
#include "../PartFile.hpp"
#include "../FileInputStream.hpp"
#include "../OutputStream.hpp"
#include "../StreamReader.hpp"
#include "../StreamWriter.hpp"
#include "../ResourceLoader.hpp"
#include "../ResourceManager.hpp"
#include "../Exception.hpp"

Geometry::Geometry(ptr<File> vertices, size_t vertexStride, ptr<File> indices, size_t indexStride, ptr<GeometryFormat> format)
: vertices(vertices), vertexStride(vertexStride), indices(indices), indexStride(indexStride), format(format)
{
	if(!indices) Geometry::indexStride = 0;
}

ptr<File> Geometry::GetVertices() const
{
	return vertices;
}

size_t Geometry::GetVertexStride() const
{
	return vertexStride;
}

ptr<File> Geometry::GetIndices() const
{
	return indices;
}

size_t Geometry::GetIndexStride() const
{
	return indexStride;
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

		// считать размеры вершин
		size_t verticesSize = reader->ReadShortly();
		size_t vertexStride = reader->ReadShortly();
		if(verticesSize % vertexStride)
			THROW_PRIMARY_EXCEPTION("Invalid vertex stride");

		size_t indicesSize = reader->ReadShortly();
		size_t indexStride = 0;
		if(indicesSize)
		{
			indexStride = reader->ReadShortly();
			if(indicesSize % indexStride || (indicesSize / indexStride) % 3)
				THROW_PRIMARY_EXCEPTION("Invalid index stride");
		}
		else
		{
			if((verticesSize / vertexStride) % 3)
				THROW_PRIMARY_EXCEPTION("Invalid vertex faces");
		}

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

		// вернуть геометрию
		return NEW(Geometry(vertices, vertexStride, indices, indexStride, format));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't deserialize geometry", exception);
	}
}

void Geometry::Serialize(ptr<OutputStream> outputStream)
{
	ptr<StreamWriter> writer = NEW(StreamWriter(outputStream));

	writer->WriteShortly(vertices->GetSize());
	writer->WriteShortly(vertexStride);
	if(indices)
	{
		writer->WriteShortly(indices->GetSize());
		writer->WriteShortly(indexStride);
	}
	else
		writer->WriteShortly(0);

	format->Serialize(writer);

	writer->Write(vertices->GetData(), vertices->GetSize());
	if(indices)
		writer->Write(indices->GetData(), indices->GetSize());
}
