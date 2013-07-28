#include "SimpleGeometryCreator.hpp"
#include <iostream>

String SimpleGeometryCreator::GetCommand() const
{
	return "sg";
}

void SimpleGeometryCreator::PrintHelp() const
{
	std::cout << "Creates a geometry file from easy-formatted text file. Using:\n";
	std::cout << "sg <source text file> <output geometry file> <output geometry format>\n";
	std::cout << "Detailed source file format specification can be retrieved with helpsg command.\n";
}

/*
Формат исходного файла:

Сначала идет описание типа вершин. Описание состоит из нескольких строк, каждая строка описывает
одно поле структуры в таком виде:
<тип>[<размер>][x<размер2>][_w|h] [<текстовое описание>]
Тип - одно из следующих: float, int, uint.
Размер может быть цифрой (любой). Он задаёт на самом несколько полей из одного.
w задаёт, что значение должно быть разделено на ширину текстуры.
h задаёт, что значение должно быть разделено на высоту текстуры.
пригодится, если поле является текстурными координатами.
Текстовое описание - просто комментарий к полю.
В описании также может идти:
width <ширина текстуры>
height <высота текстуры>
Описание заканчивается словом end (на отдельной строке).
Затем идут вершины. Каждая вершина - это просто набор значений, разделенных пробельными символами,
и сами вершины разделены пробельными символами, так что не важно, как они идут. InanityArchi только
проверяет, чтобы общее количество полей было кратно размеру вершины, то есть все вершины определены
полностью. В тексте могут идти комментарии стиля C++ (символы комментария должны идти отдельными
токенами, то есть быть отделены пробелами).

Затем могут идти индексы. Начинаются они со слова indexed. Затем они просто идут.
Их количество должно быть кратно 3.

Вот и всё.
*/

void SimpleGeometryCreator::Run(const std::vector<String>& arguments)
{
	if(arguments.size() < 3)
		THROW("Must be 3 auguments for command");

	ptr<FileSystem> fileSystem = FolderFileSystem::GetNativeFileSystem();
	//открыть исходный файл
	_wfreopen(Strings::UTF82Unicode(arguments[0]).c_str(), L"rb", stdin);

	//размеры текстуры
	textureWidth = 1;
	textureHeight = 1;
	//размер вершины
	size_t vertexStride = 0;
	//считать поля
	char str[1024];
	while(gets_s(str))
	{
		Field field;
		//считать тип и размеры
		char typeStr[32];
		size_t i;
		for(i = 0; str[i] && isalpha(str[i]); ++i)
			typeStr[i] = str[i];
		typeStr[i] = 0;
		sscanf(str + i, "%dx%d", &field.size1, &field.size2);
		for(; str[i] && !isspace(str[i]); ++i);
		for(; str[i] && isspace(str[i]); ++i);
		field.divider = str[i];

		//получить тип
		size_t fieldSize;
		if(strcmp(typeStr, "float") == 0)
		{
			field.type = Field::typeFloat;
			fieldSize = sizeof(float);
		}
		else if(strcmp(typeStr, "int") == 0)
		{
			field.type = Field::typeInt;
			fieldSize = sizeof(int);
		}
		else if(strcmp(typeStr, "uint") == 0)
		{
			field.type = Field::typeUInt;
			fieldSize = sizeof(unsigned);
		}
		else if(strcmp(typeStr, "end") == 0)
			break;
		else if(strcmp(typeStr, "width") == 0)
		{
			textureWidth = field.size1;
			continue;
		}
		else if(strcmp(typeStr, "height") == 0)
		{
			textureHeight = field.size1;
			continue;
		}
		else
			THROW(String("Unknown field type: ") + typeStr);

		vertexStride += fieldSize * field.size1 * field.size2;

		//добавить поле
		fields.push_back(field);
	}

	//создать поток вывода в памяти
	stream = new MemoryStream();

	//цикл считывания вершин
	bool stop = false, indexed = false;
	while(!stop)
	{
		//проверить, может там начинается слово indexes
		char c;
		scanf(" %c", &c);
		if(c == 'i')
		{
			indexed = true;
			char s[10];
			scanf("%s", s);
			break;
		}
		else
			ungetc(c, stdin);
		//цикл считывания полей в вершине
		for(size_t i = 0; i < fields.size(); ++i)
			//если поле не считалось
			if(!ReadField(fields[i]))
				//если оно не первое в вершине, то это плохо
				if(i)
					THROW("Can't read field");
				//иначе это нормально, просто завершить цикл
				else
				{
					stop = true;
					break;
				}
	}

	//если индексы, считывать и их
	//индексы могут идти как 
	std::vector<short> indices;
	if(indexed)
	{
		for(;;)
		{
			//считать строку
			if(scanf("%s", str) != 1)
				break;
			//если это треугольник
			if(strcmp(str, "t") == 0)
			{
				//считать три индекса
				int a, b, c;
				if(scanf("%d%d%d", &a, &b, &c) != 3)
					THROW("Unexpected end of file: can't read triangle");
				indices.push_back(a);
				indices.push_back(b);
				indices.push_back(c);
			}
			//иначе если это прямоугольник
			else if(strcmp(str, "r") == 0)
			{
				//считать 4 индекса в порядке по часовой стрелке (как и треугольник)
				int a, b, c, d;
				if(scanf("%d%d%d%d", &a, &b, &c, &d) != 4)
					THROW("Unexpected end of file: can't read rectangle");
				indices.push_back(a);
				indices.push_back(b);
				indices.push_back(c);
				indices.push_back(a);
				indices.push_back(c);
				indices.push_back(d);
			}
			//иначе если это комментарий
			else if(strcmp(str, "/*") == 0)
				//найти закрывающий комментарий
				while(scanf("%s", str) == 1 && strcmp(str, "*/") != 0);
			//иначе если это однострочный комментарий
			else if(strcmp(str, "//") == 0)
				//считать все до конца строки
				gets_s(str);
			//иначе это что-то непонятное
			else
				THROW(String("Unknown type of index: ") + str);
		}
		if(indices.size() % 3)
			THROW("Number of indices must be divisible by 3");
	}

	ptr<File> indicesFile;
	if(indexed)
	{
		indicesFile = new MemoryFile(indices.size() * sizeof(short));
		memcpy(indicesFile->GetData(), &*indices.begin(), indicesFile->GetSize());
	}
	//создать геометрию
	ptr<Graphics::Geometry> geometry = NEW(Graphics::Geometry(stream->ToFile(), vertexStride, indicesFile, indexed ? sizeof(short) : 0, nullptr));
	//сохранить файл
	geometry->Serialize(fileSystem->SaveStream(arguments[1]), arguments[2]);
}

bool SimpleGeometryCreator::ReadField(const Field& field)
{
	//выяснить, сколько данных необходимо считать
	size_t count = field.size1 * field.size2;

	char str[1024];
	//цикл считывания подполей
	for(size_t i = 0; i < count; ++i)
		//цикл считывания подполя
		for(;;)
		{
			//считать токен, а если не считывается
			if(scanf("%s", str) != 1)
				//если он не первый в данном подполе
				if(i)
					//выбросить исключение
					THROW("Unexpected end of file: can't read subfield");
				//иначе он первый, то есть поле еще не началось
				//возможно, все в порядке (если это поле - первое в вершине,
				//но с этим разберется вызывающая функция)
				else
					//просто вернуть неуспех
					return false;

			//если строка - символ комментария
			if(strcmp(str, "/*") == 0)
				//считывать все остальное, пока не найдем конец комментария
				while(scanf("%s", str) == 1 && strcmp(str, "*/") != 0);
			//иначе если строка - символ однострочного комментария
			else if(strcmp(str, "//") == 0)
				//считать все до конца строки
				gets_s(str);
			//иначе это данное
			else
			{
				//выбор по типу
				switch(field.type)
				{
				case Field::typeFloat:
					{
						float data;
						if(sscanf(str, "%f", &data) != 1)
							THROW(String("Can't read float from ") + str);
						switch(field.divider)
						{
						case 'w':
							data /= textureWidth;
							break;
						case 'h':
							data /= textureHeight;
							break;
						}
						stream->Write(&data, sizeof(data));
					}
					break;
				case Field::typeInt:
					{
						int data;
						if(sscanf(str, "%d", &data) != 1)
							THROW(String("Can't read int from ") + str);
						switch(field.divider)
						{
						case 'w':
							data /= textureWidth;
							break;
						case 'h':
							data /= textureHeight;
							break;
						}
						stream->Write(&data, sizeof(data));
					}
					break;
				case Field::typeUInt:
					{
						unsigned data;
						if(sscanf(str, "%u", &data) != 1)
							THROW(String("Can't read uint from ") + str);
						switch(field.divider)
						{
						case 'w':
							data /= textureWidth;
							break;
						case 'h':
							data /= textureHeight;
							break;
						}
						stream->Write(&data, sizeof(data));
					}
					break;
				default:
					THROW("Unknown type of data");
				}
				//считали, так что цикл надо закончить
				break;
			}
		}

	return true;
}
