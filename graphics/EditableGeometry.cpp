#include "EditableGeometry.hpp"
#include "../File.hpp"
#include "../MemoryFile.hpp"
#include "../Exception.hpp"
#include <vector>
#include <set>
#include <algorithm>

EditableGeometry::EditableGeometry(ptr<File> vertices, size_t vertexStride, ptr<File> indices, size_t indexStride, const String& format)
: vertices(vertices), vertexStride(vertexStride), indices(indices), indexStride(indexStride), format(format)
{
	if(!indices) EditableGeometry::indexStride = 0;
}

ptr<File> EditableGeometry::GetVertices() const
{
	return vertices;
}

size_t EditableGeometry::GetVertexStride() const
{
	return vertexStride;
}

ptr<File> EditableGeometry::GetIndices() const
{
	return indices;
}

size_t EditableGeometry::GetIndexStride() const
{
	return indexStride;
}

String EditableGeometry::GetFormat() const
{
	return format;
}

ptr<EditableGeometry> EditableGeometry::Subgeometry(size_t startIndex, size_t indicesCount) const
{
	try
	{
		ptr<File> verticesFile = NEW(MemoryFile(indicesCount * vertexStride));
		char* verticesData = (char*)vertices->GetData();
		char* newVertices = (char*)verticesFile->GetData();
		if(indices)
		{
			if(indexStride == 4)
			{
				unsigned* indicesData = (unsigned*)indices->GetData();
				for(size_t i = 0; i < indicesCount; ++i)
					memcpy(newVertices + (startIndex + i) * vertexStride, verticesData + indicesData[startIndex + i] * vertexStride, vertexStride);
			}
			else if(indexStride == 2)
			{
				unsigned short* indicesData = (unsigned short*)indices->GetData();
				for(size_t i = 0; i < indicesCount; ++i)
					memcpy(newVertices + (startIndex + i) * vertexStride, verticesData + indicesData[startIndex + i] * vertexStride, vertexStride);
			}
			else
				THROW_PRIMARY_EXCEPTION("Unsupported index format");
		}
		else
			memcpy(newVertices + startIndex * vertexStride, verticesData + startIndex * vertexStride, indicesCount * vertexStride);

		return NEW(EditableGeometry(verticesFile, vertexStride, 0, 0, format));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create subgeometry", exception);
	}
}

void EditableGeometry::AddVertexMapping(VertexMapping mapping, ptrdiff_t offset)
{
	mappings[mapping] = offset;
}

ptr<EditableGeometry> EditableGeometry::ShortIndices() const
{
	try
	{
		//получить количество индексов
		size_t indicesCount = indices->GetSize() / indexStride;
		//если индексы некуда сокращать, закончить
		if(indexStride != 4 || indicesCount >= 0x10000)
			return NEW(EditableGeometry(vertices, vertexStride, indices, indexStride, format));
		//иначе сократить
		ptr<File> newIndices = NEW(MemoryFile(indicesCount * sizeof(unsigned short)));
		unsigned short* newIndicesPtr = (unsigned short*)newIndices->GetData();
		unsigned* indicesPtr = (unsigned*)indices->GetData();
		std::copy(indicesPtr, indicesPtr + indicesCount, newIndicesPtr);
		return NEW(EditableGeometry(vertices, vertexStride, newIndices, sizeof(unsigned short), format));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't short indices", exception);
	}
}

ptr<EditableGeometry> EditableGeometry::CreateShadowGeometry() const
{
	try
	{
		if(indexStride != 2)
			THROW_PRIMARY_EXCEPTION("Only 16-bit indices supported");
		size_t positionMapping;
		{
			std::map<VertexMapping, ptrdiff_t>::const_iterator it = mappings.find(vertexMappingPosition);
			if(it == mappings.end())
				THROW_PRIMARY_EXCEPTION("Position mapping is not specified");
			positionMapping = it->second;
		}

		struct Face
		{
			size_t vertices[3];
		};
		struct Edge
		{
			std::vector<size_t> faces;
			std::vector<std::pair<size_t, size_t> > vertices;

			static inline std::pair<size_t, size_t> GetNumber(size_t i, size_t j)
			{
				if(i > j) std::swap(i, j);
				return std::make_pair(i, j);
			}
		};

		char* vertices = (char*)EditableGeometry::vertices->GetData();
		size_t verticesCount = EditableGeometry::vertices->GetSize() / vertexStride;
		unsigned short* indices = (unsigned short*)EditableGeometry::indices->GetData();
		size_t indicesCount = EditableGeometry::indices->GetSize() / indexStride;

		std::vector<ShadowVertex> resultVertices;
		std::vector<size_t> resultIndices;

		size_t resultIndicesCount = 0;

		size_t i, j;
		std::vector<Face> faces;
		std::map<std::pair<size_t, size_t>, Edge> edges;
		for(i = 0; i < indicesCount; i += 3)
		{
			Face face;
			for(j = 0; j < 3; ++j)
				face.vertices[j] = indices[i + j];
			faces.push_back(face);

			{
				Edge& edge = edges[Edge::GetNumber(face.vertices[0], face.vertices[1])];
				edge.faces.push_back((unsigned)faces.size() - 1);
				edge.vertices.push_back(std::make_pair(0, 1));
			}
			{
				Edge& edge = edges[Edge::GetNumber(face.vertices[1], face.vertices[2])];
				edge.faces.push_back((unsigned)faces.size() - 1);
				edge.vertices.push_back(std::make_pair(1, 2));
			}
			{
				Edge& edge = edges[Edge::GetNumber(face.vertices[2], face.vertices[0])];
				edge.faces.push_back((unsigned)faces.size() - 1);
				edge.vertices.push_back(std::make_pair(2, 0));
			}
		}

		for(i = 0; i < faces.size(); ++i)
		{
			Face& face = faces[i];

			vec3 normal = Math::normalize(Math::normal(
				*(vec3*)(vertices + face.vertices[0] * vertexStride + positionMapping),
				*(vec3*)(vertices + face.vertices[1] * vertexStride + positionMapping),
				*(vec3*)(vertices + face.vertices[2] * vertexStride + positionMapping)));

			for(j = 0; j < 3; ++j)
			{
				ShadowVertex vertex;
				vertex.position = *(vec3*)(vertices + face.vertices[j] * vertexStride + positionMapping);
				vertex.normal = normal;
				face.vertices[j] = resultVertices.size();
				resultIndices.push_back(face.vertices[j]);
				resultVertices.push_back(vertex);
			}
		}

		for(std::map<std::pair<size_t, size_t>, Edge>::iterator it = edges.begin(); it != edges.end(); ++it)
		{
			Edge& edge = it->second;

			if(edge.faces.size() == 1)
			{
				const Face& face = faces[edge.faces[0]];
				size_t v1 = face.vertices[edge.vertices[0].first];
				size_t v2 = face.vertices[edge.vertices[0].second];

				ShadowVertex vertex;

				vertex.position = resultVertices[v1].position;
				vertex.normal = vec3(0, 0, 0);
				size_t av1 = resultVertices.size();
				resultVertices.push_back(vertex);

				vertex.position = resultVertices[v2].position;
				vertex.normal = vec3(0, 0, 0);
				size_t av2 = resultVertices.size();
				resultVertices.push_back(vertex);

				resultIndices.push_back(v1);
				resultIndices.push_back(v2);
				resultIndices.push_back(av2);
				resultIndices.push_back(v1);
				resultIndices.push_back(av2);
				resultIndices.push_back(av1);
			}
			else
			{
				const Face& face1 = faces[edge.faces[0]];
				const Face& face2 = faces[edge.faces[1]];

				size_t a1 = face1.vertices[edge.vertices[0].first];
				size_t a2 = face1.vertices[edge.vertices[0].second];
				size_t b1 = face2.vertices[edge.vertices[1].first];
				size_t b2 = face2.vertices[edge.vertices[1].second];

				resultIndices.push_back(a1);
				resultIndices.push_back(a2);
				resultIndices.push_back(b1);
				resultIndices.push_back(a1);
				resultIndices.push_back(b1);
				resultIndices.push_back(b2);
			}
		}

		ShadowVertex* memoryVertices = new ShadowVertex[resultVertices.size()];
		size_t verticesSize = resultVertices.size() * sizeof(ShadowVertex);
		std::copy(resultVertices.begin(), resultVertices.end(), memoryVertices);
		void* memoryIndices;
		size_t indexStride;
		if(resultIndices.size() < 0x10000)
		{
			memoryIndices = new unsigned short[resultIndices.size()];
			indexStride = 2;
			std::copy(resultIndices.begin(), resultIndices.end(), (unsigned short*)memoryIndices);
		}
		else
		{
			memoryIndices = new unsigned[resultIndices.size()];
			indexStride = 4;
			std::copy(resultIndices.begin(), resultIndices.end(), (unsigned*)memoryIndices);
		}

		return NEW(EditableGeometry(NEW(MemoryFile(memoryVertices, verticesSize)), sizeof(ShadowVertex), NEW(MemoryFile(memoryIndices, resultIndices.size() * indexStride)), indexStride, "shadow"));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create shadow geometry", exception);
	}
}

ptr<EditableGeometry> EditableGeometry::CreateTangentGeometry(bool smooth, const String& format) const
{
	try
	{
		//проверить, что индексы 16 или 32-х битные
		if(indexStride != 2 && indexStride != 4)
			THROW_PRIMARY_EXCEPTION("Only 16-bit or 32-bit indices are supported.");

		//получить привязки для положения точки, нормали, текстурных координат
		size_t positionMapping;
		{
			std::map<VertexMapping, ptrdiff_t>::const_iterator it = mappings.find(vertexMappingPosition);
			if(it == mappings.end())
				THROW_PRIMARY_EXCEPTION("Position mapping is not specified");
			positionMapping = it->second;
		}
		size_t normalMapping;
		{
			std::map<VertexMapping, ptrdiff_t>::const_iterator it = mappings.find(vertexMappingNormal);
			if(it == mappings.end())
				THROW_PRIMARY_EXCEPTION("Normal mapping is not specified");
			normalMapping = it->second;
		}
		size_t texcoordMapping;
		{
			std::map<VertexMapping, ptrdiff_t>::const_iterator it = mappings.find(vertexMappingTexcoord);
			if(it == mappings.end())
				THROW_PRIMARY_EXCEPTION("Texcoord mapping is not specified");
			texcoordMapping = it->second;
		}

		//если сглаживание включено
		size_t vertexCount = vertices->GetSize() / vertexStride;
		void* verticesData = vertices->GetData();
		size_t indicesCount = indices->GetSize() / indexStride;
		//вычислить количество результирующих вершин
		size_t newVertexCount = smooth ? vertices->GetSize() / vertexStride : indicesCount;
		void* indicesData = indices->GetData();
		//выделить память для результирующих вершин
		ptr<File> resultVerticesFile = NEW(MemoryFile(sizeof(TangentVertex) * newVertexCount));
		TangentVertex* resultVertices = (TangentVertex*)resultVerticesFile->GetData();

		//обнулить все поля в результирующих вершинах
		std::fill((char*)resultVertices, (char*)resultVertices + resultVerticesFile->GetSize(), 0);
		static const float eps = 1e-8f;
		//сравниватель векторов; используется только для того, чтобы отделить различные векторы
		struct VectorComparer
		{
			bool operator()(const vec3& a, const vec3& b) const
			{
				if(a.x < b.x - eps) return true;
				if(b.x < a.x - eps) return false;
				if(a.y < b.y - eps) return true;
				if(b.y < a.y - eps) return false;
				return a.z < b.z;
			}
		};
		//структура из трех векторов
		struct ThreeVectors
		{
			vec3 transform1;
			vec3 transform2;
			vec3 transform3;
			size_t count;

			ThreeVectors() : transform1(0, 0, 0), transform2(0, 0, 0), transform3(0, 0, 0), count(0)
			{
			}

			ThreeVectors(vec3 transform1, vec3 transform2, vec3 transform3)
				: transform1(transform1), transform2(transform2), transform3(transform3), count(1)
			{
			}

			ThreeVectors& operator+=(const ThreeVectors& v)
			{
				transform1 = transform1 + v.transform1;
				transform2 = transform2 + v.transform2;
				transform3 = transform3 + v.transform3;
				count++;
				return *this;
			}
		};
		//сравниватель троек векторов; используется только для разделения различных
		struct ThreeVectorsComparer
		{
			VectorComparer comparer;

			bool operator()(const ThreeVectors& a, const ThreeVectors& b) const
			{
				if(comparer(a.transform1, b.transform1)) return true;
				if(comparer(b.transform1, a.transform1)) return false;
				if(comparer(a.transform2, b.transform2)) return true;
				if(comparer(b.transform2, a.transform2)) return false;
				return comparer(a.transform3, b.transform3);
			}
		};
		//карта по векторам, содержащая на каждый вектор сумму векторов - трансформаций
		std::map<vec3, ThreeVectors, VectorComparer> divisors;

		//структура для упрощения получения данных вершины по привязкам
		struct Vertex
		{
			vec3 position;
			vec3 normal;
			vec2 texcoord;

			Vertex()
			{
			}

			Vertex(void* data, unsigned positionMapping, unsigned normalMapping, unsigned texcoordMapping) :
				position(*(vec3*)((unsigned char*)data + positionMapping)),
				normal(*(vec3*)((unsigned char*)data + normalMapping)),
				texcoord(*(vec2*)((unsigned char*)data + texcoordMapping))
			{
			}
		};

		//цикл по треугольникам
		for(size_t i = 0; i < indicesCount; i += 3)
		{
			//вершины треугольника
			Vertex v[3];
			//цикл по вершинам в треугольнике
			for(size_t k = 0; k < 3; ++k)
			{
				//получить индекс вершины
				size_t index = indexStride == 4 ? ((unsigned*)indicesData)[i + k] : ((unsigned short*)indicesData)[i + k];
				//получить вершину
				v[k] = Vertex((char*)verticesData + vertexStride * index, positionMapping, normalMapping, texcoordMapping);
				//сразу записать в результирующие вершины положение и текстурные координаты
				size_t p = smooth ? index : (i + k);
				resultVertices[p].position = v[k].position;
				resultVertices[p].texcoord = v[k].texcoord;
			}

			//** далее идет вычисление матрицы преобразования

			// ЗДЕСЬ БЫЛО СТАРОЕ ВЫЧИСЛЕНИЕ; ТЕПЕРЬ ОНО ЗАПИХАНО В КОНЕЦ ФАЙЛА В КОММЕНТАРИЙ

			const vec3 a21 = Math::normalize(v[1].position - v[0].position);
			const vec3 a31 = Math::normalize(v[2].position - v[0].position);

			vec2 t21 = Math::normalize(v[1].texcoord - v[0].texcoord);
			vec2 t31 = Math::normalize(v[2].texcoord - v[0].texcoord);

			vec3 n = Math::normalize(Math::cross(a31, a21));
			float d = 1.0f / (t21.x * t31.y - t21.y * t31.x);
			vec3 transform1((a21.x * t31.y - a31.x * t21.y) * d, (a31.x * t21.x - a21.x * t31.x) * d, n.x);
			vec3 transform2((a21.y * t31.y - a31.y * t21.y) * d, (a31.y * t21.x - a21.y * t31.x) * d, n.y);
			vec3 transform3((a21.z * t31.y - a31.z * t21.y) * d, (a31.z * t21.x - a21.z * t31.x) * d, n.z);

			if(smooth)
			{
				ThreeVectors q(transform1, transform2, transform3);
				divisors[v[0].position] += q;
				divisors[v[1].position] += q;
				divisors[v[2].position] += q;
			}
			else
				for(unsigned k = 0; k < 3; ++k)
				{
					resultVertices[i + k].transform1 = transform1;
					resultVertices[i + k].transform2 = transform2;
					resultVertices[i + k].transform3 = transform3;
				}
		}

		if(smooth)
		{
			//цикл по делителям - определяем окончательные трансформации
			for(std::map<vec3, ThreeVectors, VectorComparer>::iterator i = divisors.begin(); i != divisors.end(); ++i)
			{
				scalar count = (scalar)i->second.count;
				i->second.transform1 = i->second.transform1 / count;
				i->second.transform2 = i->second.transform2 / count;
				i->second.transform3 = i->second.transform3 / count;
			}
			//цикл по вершинам - для распределения трансформаций
			for(size_t i = 0; i < newVertexCount; ++i)
			{
				ThreeVectors& transform = divisors[resultVertices[i].position];
				resultVertices[i].transform1 = transform.transform1;
				resultVertices[i].transform2 = transform.transform2;
				resultVertices[i].transform3 = transform.transform3;
			}
		}

		if(smooth)
			return NEW(EditableGeometry(resultVerticesFile, sizeof(TangentVertex), indices, indexStride, format));
		else
			return NEW(EditableGeometry(resultVerticesFile, sizeof(TangentVertex), 0, 0, format));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create tangent geometry", exception);
	}
}
