#ifndef ___INANITY_GRAPHICS_EDITABLE_GEOMETRY_H___
#define ___INANITY_GRAPHICS_EDITABLE_GEOMETRY_H___

#include "Geometry.hpp"
#include "GeometryFormat.hpp"
#include <vector>
#include <map>
#include <set>
#include <algorithm>

BEGIN_INANITY_GRAPHICS

/// Класс типизированной редактируемой геометрии.
/**
Различные процедуры выдвигают различные требования к шаблонным параметрам.
Они написаны в комментариях.
*/
template <typename Vertex, typename Index = unsigned short>
class EditableGeometry : public Object
{
private:
	std::vector<Vertex> vertices;
	std::vector<Index> indices;

public:
	/// Создать типизированную геометрию.
	EditableGeometry(const std::vector<Vertex>& vertices, const std::vector<Index>& indices)
		: vertices(vertices), indices(indices)
	{
	}

	/// Создать типизированную геометрию из неиндексированных вершин.
	EditableGeometry(const std::vector<Vertex>& vertices) : vertices(vertices)
	{
		indices.resize(vertices.size());
		for(size_t i = 0; i < indices.size(); ++i)
			indices[i] = i;
	}

	/// Сериализовать геометрию.
	void Serialize(ptr<OutputStream> outputStream, const String& formatFileName)
	{
		// проверить, являются ли индексы последовательными числами
		// тогда геометрию можно будет сериализовать без индексов
		bool needIndices = false;
		for(size_t i = 0; i < indices.size(); ++i)
			if(indices[i] != i)
			{
				needIndices = true;
				break;
			}

		ptr<StreamWriter> writer = NEW(StreamWriter(outputStream));

		writer->WriteShortly(vertices.size());
		writer->WriteShortly(sizeof(Vertex));
		writer->WriteShortly(needIndices ? indices.size() : 0);
		if(needIndices)
			writer->WriteShortly(sizeof(Index));
		writer->WriteString(formatFileName);
		writer->Write(&*vertices.begin(), vertices.size() * sizeof(Vertex));
		if(needIndices)
			writer->Write(&*indices.begin(), indices.size() * sizeof(Index));
	}

	ptr<File> SerializeVertices() const
	{
		ptr<File> file = NEW(MemoryFile(vertices.size() * sizeof(Vertex)));
		memcpy(file->GetData(), &*vertices.begin(), vertices.size() * sizeof(Vertex));
		return file;
	}

	ptr<File> SerializeIndices() const
	{
		ptr<File> file = NEW(MemoryFile(indices.size() * sizeof(Index)));
		memcpy(file->GetData(), &*indices.begin(), indices.size() * sizeof(Index));
		return file;
	}

	/// Оптимизировать геометрию
	/** Выполняется объединение одинаковых вершин, с заданием соответствующих индексов.
	Для типа Vertex требуется наличие operator< и operator==.
	*/
	ptr<EditableGeometry> Optimize() const
	{
		//скопировать вершины в новый массив с их индексами
		std::vector<std::pair<Vertex, size_t> > newVertices(vertices.size());
		for(size_t i = 0; i < vertices.size(); ++i)
			newVertices[i] = std::make_pair(vertices[i], i);
		//отсортировать их
		std::sort(newVertices.begin(), newVertices.end());
		//выполнить объединение одинаковых вершин
		//заодно построить массив преобразования индексов
		std::vector<Vertex> resultVertices;
		std::vector<size_t> indexMap(vertices.size());
		resultVertices.reserve(vertices.size());
		for(size_t i = 0; i < newVertices.size(); )
		{
			size_t j;
			for(j = i + 1; j < newVertices.size() && newVertices[i].first == newVertices[j].first; ++j);
			size_t newIndex = resultVertices.size();
			resultVertices.push_back(newVertices[i].first);
			for(; i < j; ++i)
				indexMap[newVertices[i].second] = newIndex;
		}
		//скопировать индексы в новый массив
		std::vector<Index> resultIndices(indices.size());
		//и преобразовать их
		for(size_t i = 0; i < resultIndices.size(); ++i)
			resultIndices[i] = indexMap[indices[i]];
		return NEW(EditableGeometry(resultVertices, resultIndices));
	}

	/// Добавить в геометрию тангенту и бинормаль.
	template <typename OutputVertex>
	ptr<EditableGeometry<OutputVertex, Index> > CreateTangentAndBinormal() const
	{
		//ликвидировать индексы
		std::vector<OutputVertex> outputVertices(indices.size());
		for(size_t i = 0; i < outputVertices.size(); ++i)
		{
			const Vertex& vertex = vertices[indices[i]];
			OutputVertex& outputVertex = outputVertices[i];
			outputVertex.position = vertex.position;
			outputVertex.texcoord = vertex.texcoord;
		}

		//обработать треугольники и вычислить для каждого нормали
		for(size_t i = 0; i < outputVertices.size(); i += 3)
		{
			//получить указатель на три вершины
			OutputVertex* v = &*(outputVertices.begin() + i);

			//вычислить нормаль
			float3 temp;
			float3 normal;
			D3DXVec3Cross(&temp, &(v[2].position - v[0].position), &(v[1].position - v[0].position));
			D3DXVec3Normalize(&normal, &temp);

			//вычислить касательную и бинормаль
			float3 tangent, binormal;
			{
				//для этого делаем матрицу
				float4x4 m, m1;
				m._11 = v[1].position.x - v[0].position.x;
				m._12 = v[1].position.y - v[0].position.y;
				m._13 = v[1].position.z - v[0].position.z;
				m._14 = 0;
				m._21 = v[2].position.x - v[0].position.x;
				m._22 = v[2].position.y - v[0].position.y;
				m._23 = v[2].position.z - v[0].position.z;
				m._24 = 0;
				m._31 = normal.x;
				m._32 = normal.y;
				m._33 = normal.z;
				m._34 = 0;
				m._41 = 0;
				m._42 = 0;
				m._43 = 0;
				m._44 = 1;
				//обращаем её
				D3DXMatrixInverse(&m1, NULL, &m);
				//и умножаем её сначала на один вектор (для касательной)
				D3DXVec3TransformCoord(&tangent, &float3(v[1].texcoord.x - v[0].texcoord.x, v[2].texcoord.x - v[0].texcoord.x, 0), &m1);
				//а затем на другой (для бинормали)
				D3DXVec3TransformCoord(&binormal, &float3(v[1].texcoord.y - v[0].texcoord.y, v[2].texcoord.y - v[0].texcoord.y, 0), &m1);
			}

			//записать вектора в вершины
			for(size_t j = 0; j < 3; ++j)
			{
				v[j].normal = normal;
				v[j].tangent = tangent;
				v[j].binormal = binormal;
			}
		}

		//вернуть геометрию
		return NEW((EditableGeometry<OutputVertex, Index>(outputVertices)));
	}

	/// Добавить в геометрию трансформации для прямого bump'а.
	template <typename OutputVertex>
	ptr<EditableGeometry<OutputVertex, Index> > CreateBumpTransformations() const
	{
		//ликвидировать индексы
		std::vector<OutputVertex> outputVertices(indices.size());
		for(size_t i = 0; i < outputVertices.size(); ++i)
		{
			const Vertex& vertex = vertices[indices[i]];
			OutputVertex& outputVertex = outputVertices[i];
			outputVertex.position = vertex.position;
			outputVertex.texcoord = vertex.texcoord;
		}

		//обработать треугольники
		for(size_t i = 0; i < outputVertices.size(); i += 3)
		{
			//получить указатель на три вершины
			OutputVertex* v = &*(outputVertices.begin() + i);

			//вычислить нормаль
			float3 temp;
			float3 normal;
			D3DXVec3Cross(&temp, &(v[2].position - v[0].position), &(v[1].position - v[0].position));
			D3DXVec3Normalize(&normal, &temp);

			//вычислить два базовых мировых вектора
			float3 Ba, Ca;
			temp = v[1].position - v[0].position;
			D3DXVec3Normalize(&Ba, &temp);
			temp = v[2].position - v[0].position;
			D3DXVec3Normalize(&Ca, &temp);
			//и два базовых нормальных вектора
			float2 ba, ca;
			float2 temp2;
			temp2 = v[1].texcoord - v[0].texcoord;
			D3DXVec2Normalize(&ba, &temp2);
			temp2 = v[2].texcoord - v[0].texcoord;
			D3DXVec2Normalize(&ca, &temp2);
			float3 transform1, transform2, transform3;
			{
				//для этого делаем матрицу
				float4x4 m, m1;
				m._11 = ba.x;
				m._12 = ba.y;
				m._13 = 0;
				m._14 = 0;
				m._21 = ca.x;
				m._22 = ca.y;
				m._23 = 0;
				m._24 = 0;
				m._31 = 0;
				m._32 = 0;
				m._33 = 1;
				m._34 = 0;
				m._41 = 0;
				m._42 = 0;
				m._43 = 0;
				m._44 = 1;
				//обращаем её
				D3DXMatrixInverse(&m1, NULL, &m);
				//делаем другую матрицу
				float4x4 m2;
				m2._11 = Ba.x;
				m2._12 = Ba.y;
				m2._13 = Ba.z;
				m2._14 = 0;
				m2._21 = Ca.x;
				m2._22 = Ca.y;
				m2._23 = Ca.z;
				m2._24 = 0;
				m2._31 = normal.x;
				m2._32 = normal.y;
				m2._33 = normal.z;
				m2._34 = 0;
				m2._41 = 0;
				m2._42 = 0;
				m2._43 = 0;
				m2._44 = 1;
				//умножаем матрицы
				m = m1 * m2;

				//получаем трансформации
				transform1 = float3(m._11, m._21, m._31);
				transform2 = float3(m._12, m._22, m._32);
				transform3 = float3(m._13, m._23, m._33);
			}

			//записать вектора в вершины
			for(size_t j = 0; j < 3; ++j)
			{
				v[j].transform1 = transform1;
				v[j].transform2 = transform2;
				v[j].transform3 = transform3;
			}
		}

		//вернуть геометрию (макрос NEW никак не хочет компилироваться)
		return new EditableGeometry<OutputVertex, Index>(outputVertices);
	}

	//структура из трех векторов
	struct ThreeVectors
	{
		float3 a;
		float3 b;
		float3 c;

		ThreeVectors(const float3& a = float3(0, 0, 0), const float3& b = float3(0, 0, 0), const float3& c = float3(0, 0, 0))
			: a(a), b(b), c(c)
		{
		}

		void operator+=(const ThreeVectors& v)
		{
			a += v.a;
			b += v.b;
			c += v.c;
		}

		void operator/=(const float v)
		{
			a /= v;
			b /= v;
			c /= v;
		}
	};
	//сравниватель векторов
	struct Float3Comparer
	{
		bool operator()(const float3& a, const float3& b) const
		{
			if(a.x < b.x - GraphicsMath::eps) return true;
			if(b.x < a.x - GraphicsMath::eps) return false;
			if(a.y < b.y - GraphicsMath::eps) return true;
			if(b.y < a.y - GraphicsMath::eps) return false;
			return a.z < b.z - GraphicsMath::eps;
		}
	};
	//сравниватель троек векторов; используется только для разделения различных
	struct ThreeVectorsComparer
	{
		Float3Comparer comparer;
		bool operator()(const ThreeVectors& a, const ThreeVectors& b) const
		{
			if(comparer(a.a, b.a)) return true;
			if(comparer(b.a, a.a)) return false;
			if(comparer(a.b, b.b)) return true;
			if(comparer(b.b, a.b)) return false;
			return comparer(a.c, b.c);
		}
	};

	/// Сгладить геометрию с bump'ом.
	/** Выполняется усреднение нормалей, тангент и бинормалей. */
	template <
		float3 Vertex::*transform1,
		float3 Vertex::*transform2,
		float3 Vertex::*transform3
	>
	ptr<EditableGeometry> SmoothBump() const
	{
		/* Алгоритм заключается в том, чтобы объединить вершины,
		находящиеся в одной точке, и усреднить в этих вершинах
		компоненты. При этом при усреднении несколько одинаковых векторов
		считаются за один, чтобы не допускать искажение.
		*/
		//карта по векторам, содержащая на каждый вектор сумму векторов - трансформаций
		std::map<float3, std::set<ThreeVectors, ThreeVectorsComparer>, Float3Comparer> vectors;

		//собрать информацию
		for(size_t i = 0; i < indices.size(); ++i)
		{
			size_t j = indices[i];
			vectors[vertices[j].position].insert(ThreeVectors(vertices[j].*transform1, vertices[j].*transform2, vertices[j].*transform3));
		}

		//вычислить результирующие вектора
		std::map<float3, ThreeVectors, Float3Comparer> results;
		for(auto i = vectors.begin(); i != vectors.end(); ++i)
		{
			const std::set<ThreeVectors, ThreeVectorsComparer>& vectorSet = i->second;
			ThreeVectors r;
			for(std::set<ThreeVectors, ThreeVectorsComparer>::const_iterator j = vectorSet.begin(); j != vectorSet.end(); ++j)
				r += *j;
			r /= (float)vectorSet.size();
			results[i->first] = r;
		}

		//сформировать новые вершины
		std::vector<Vertex> outputVertices(indices.size());
		for(size_t i = 0; i < outputVertices.size(); ++i)
		{
			outputVertices[i] = vertices[indices[i]];
			const ThreeVectors& t = results[outputVertices[i].position];
			outputVertices[i].*transform1 = t.a;
			outputVertices[i].*transform2 = t.b;
			outputVertices[i].*transform3 = t.c;
		}

		//создать геометрию
		return NEW(EditableGeometry(outputVertices));
	}
};

END_INANITY_GRAPHICS

#endif
