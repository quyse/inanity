#ifndef ___INANITY_GRAPHICS_EDITABLE_GEOMETRY_H___
#define ___INANITY_GRAPHICS_EDITABLE_GEOMETRY_H___

#include "graphics.hpp"
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
		: vertices(vertices), indices(indices) {}

	/// Создать типизированную геометрию из неиндексированных вершин.
	EditableGeometry(const std::vector<Vertex>& vertices) : vertices(vertices)
	{
		indices.resize(vertices.size());
		for(size_t i = 0; i < indices.size(); ++i)
			indices[i] = i;
	}

	/// Получить вершины.
	std::vector<Vertex>& GetVertices()
	{
		return vertices;
	}

	/// Получить индексы.
	std::vector<Index>& GetIndices()
	{
		return indices;
	}

	/// Преобразовать тип индексов.
	template <typename ToIndex>
	ptr<EditableGeometry<Vertex, ToIndex> > CastIndices() const
	{
		return NEW(EditableGeometry<Vertex, ToIndex>(vertices, std::vector<ToIndex>(indices.begin(), indices.end())));
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
};

END_INANITY_GRAPHICS

#endif
