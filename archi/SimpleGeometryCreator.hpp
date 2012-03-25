#ifndef ___INANITY_ARCHI_SIMPLE_GEOMETRY_CREATOR_H___
#define ___INANITY_ARCHI_SIMPLE_GEOMETRY_CREATOR_H___

#include "Processor.hpp"

class SimpleGeometryCreator : public Processor
{
private:
	/// Структура поля.
	struct Field
	{
		/// Тип поля.
		enum Type
		{
			typeFloat,
			typeInt,
			typeUInt
		} type;
		/// Размер 1.
		size_t size1;
		/// Размер 2.
		size_t size2;
		/// Делитель текстуры.
		char divider;

		Field() : size1(1), size2(1), divider(0) {}
	};
	/// Вектор полей вершины.
	std::vector<Field> fields;

	/// Выходной поток данных.
	ptr<MemoryStream> stream;
	
	/// Ширина и высота текстуры.
	size_t textureWidth, textureHeight;

	/// Считать поле и записать его в выходной поток.
	bool ReadField(const Field& field);

public:
	String GetCommand() const;
	void PrintHelp() const;
	void Run(const std::vector<String>& arguments);
};

#endif
