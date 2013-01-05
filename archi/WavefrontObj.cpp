#include "WavefrontObj.hpp"
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

bool operator<(const Vertex& a, const Vertex& b)
{
	return a.position < b.position || a.position == b.position && (a.normal < b.normal || a.normal == b.normal && a.texcoord < b.texcoord);
}

bool operator == (const Vertex& a, const Vertex& b)
{
	return a.position == b.position && a.normal == b.normal && a.texcoord == b.texcoord;
}

bool operator<(const NormalVertex& a, const NormalVertex& b)
{
	return a.position < b.position || a.position == b.position &&
		(a.transform1 < b.transform1 || a.transform1 == b.transform1 &&
		(a.transform2 < b.transform2 || a.transform2 == b.transform2 &&
		(a.transform3 < b.transform3 || a.transform3 == b.transform3
		&& a.texcoord < b.texcoord)));
}

bool operator == (const NormalVertex& a, const NormalVertex& b)
{
	return a.position == b.position && a.transform1 == b.transform1 && a.transform2 == b.transform2 && a.transform3 == b.transform3 && a.texcoord == b.texcoord;
}

String WavefrontObj::GetCommand() const
{
	return "wobj";
}

void WavefrontObj::PrintHelp() const
{
	std::cout << "Converts a Wavefront OBJ formatted geometry to Inanity geometry. Usage:\n";
	std::cout << "wobj <source .obj> <result .geo> <format> [<format file name>]\n";
}

void WavefrontObj::Run(const std::vector<String>& arguments)
{
	if(arguments.size() < 3)
		THROW_PRIMARY_EXCEPTION("Must be at least 3 arguments for command");

	if(!_wfreopen(Strings::UTF82Unicode(arguments[0]).c_str(), L"r", stdin))
		THROW_PRIMARY_EXCEPTION("Can't open source file");

	ptr<Graphics::EditableGeometry<Vertex> > geometry = WavefrontObj::Convert();

	ptr<FileSystem> fileSystem = FolderFileSystem::GetNativeFileSystem();

	fileSystem->SaveFile(geometry->SerializeVertices(), arguments[1] + ".vertices");
	fileSystem->SaveFile(geometry->SerializeIndices(), arguments[1] + ".indices");

#if 0
	ptr<EditableGeometry> eGeometry;
	wstring format = arguments[2];
	if(format == "pnt")
		eGeometry = geometry->Untype(argumentsCount >= 4 ? arguments[3] : format);
	else if(format == "pbt")
		eGeometry = geometry->CreateBumpTransformations<NormalVertex>()->SmoothBump<&NormalVertex::transform1, &NormalVertex::transform2, &NormalVertex::transform3>()->Optimize()->Untype(argumentsCount >= 4 ? arguments[3] : format);
//		eGeometry = geometry->CreateBumpTransformations<NormalVertex>()/*->Optimize()*/->Untype(argumentsCount >= 4 ? arguments[3] : format);
/*	{
		eGeometry = geometry->Untype(argumentsCount >= 4 ? arguments[3] : format);
		eGeometry->AddVertexMapping(VertexMapping::Position, 0);
		eGeometry->AddVertexMapping(VertexMapping::Normal, 12);
		eGeometry->AddVertexMapping(VertexMapping::Texcoord, 24);
		eGeometry = eGeometry->CreateTangentGeometry(true, argumentsCount >= 4 ? arguments[3] : format);
	}*/
	else
		THROW_PRIMARY_EXCEPTION("Unknown geometry format");

	MakePointer(new FolderFileSystem(""))->SaveFile(GeometryFileFormatter::Save(eGeometry), arguments[1]);
#endif
}

ptr<EditableGeometry<Vertex> > WavefrontObj::Convert()
{
	std::vector<float3> points;
	std::vector<float3> normals;
	std::vector<float2> texPoints;
	std::string geometryName;
	std::vector<Vertex> vertices;

	//цикл по строкам файла
	char ss[1000];
	while(gets(ss))
	{
		//если пустая, пропустить
		if(!ss[0]) continue;
		//если комментарий, пропустить
		if(isspace(ss[0]) || ss[0] == '#') continue;

		//считать первое слово (команду)
		std::stringstream s(ss);
		std::string command;
		s >> command;

		//вершина
		if(command == "v")
		{
			float3 v;
			s >> v.x >> v.y >> v.z;
			points.push_back(v);
		}
		//нормаль к вершине
		else if(command == "vn")
		{
			float3 v;
			s >> v.x >> v.y >> v.z;
			normals.push_back(v);
		}
		//текстурные координаты вершины
		else if(command == "vt")
		{
			float3 v;
			s >> v.x >> v.y >> v.z;
			//развернуть ось Y, так как в максе начало координат текстуры внизу
			v.y = 1.0f - v.y;
			texPoints.push_back(float2(v.x, v.y));
		}
		//название геометрии (хз зачем)
		else if(command == "g")
			s >> geometryName;
		//треугольник
		else if(command == "f")
		{
			char c;
			unsigned pointNumber, texPointNumber, normalNumber;
			unsigned i;
			for(i = 0; i < 3; ++i)
			{
				s >> pointNumber >> c >> texPointNumber >> c >> normalNumber;
				Vertex vertex;
				vertex.position = points[pointNumber - 1];
				vertex.normal = normals[normalNumber - 1];
				vertex.texcoord = texPoints[texPointNumber - 1];
				vertices.push_back(vertex);
			}
			//поменять порядок вершин (надо!)
			std::swap(vertices[vertices.size() - 1], vertices[vertices.size() - 3]);
		}
	}

	//создать модель, оптимизировать и вернуть
	return MakePointer(NEW(EditableGeometry<Vertex, unsigned>(vertices)))->Optimize()->CastIndices<unsigned short>();
}
