#include "WavefrontObj.hpp"
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

std::ostream& operator<<(std::ostream& s, const vec3& p)
{
	return s << p.x << ' ' << p.y << ' ' << p.z;
}

String WavefrontObj::GetCommand() const
{
	return "wobj";
}

void WavefrontObj::PrintHelp() const
{
	std::cout << "Converts a Wavefront OBJ formatted geometry to Inanity geometry.\n";
	std::cout << "Optional with skin dronimal-formatted text file. Usage:\n";
	std::cout << "wobj <source .obj> <result .geo> [<skin text file>]\n";
}

void WavefrontObj::Run(const std::vector<String>& arguments)
{
	if(arguments.size() < 2)
		THROW("Must be at least 2 arguments for command");

	if(!_wfreopen(Strings::UTF82Unicode(arguments[0]).c_str(), L"r", stdin))
		THROW("Can't open source file");

	std::vector<int> positionNumbers;
	ptr<Graphics::EditableGeometry<Vertex, unsigned> > geometry = WavefrontObj::Convert(positionNumbers);

	ptr<FileSystem> fileSystem = Platform::FileSystem::GetNativeFileSystem();

	if(arguments.size() >= 3)
	{
		std::vector<Vertex>& vertices = geometry->GetVertices();
		std::vector<SkinnedVertex> skinnedVertices(vertices.size());

		if(!_wfreopen(Strings::UTF82Unicode(arguments[2]).c_str(), L"r", stdin))
			THROW("Can't open skin file");

		// считать коэффициенты для кожи
		size_t skinVerticesCount;
		std::cin >> skinVerticesCount;

		struct SkinCoef
		{
			uint boneNumbers[4];
			float boneWeights[4];
			vec3 position;
		};
		std::vector<SkinCoef> skinCoefs(skinVerticesCount);

		std::vector<std::pair<float, int> > bones;
		for(size_t i = 0; i < skinVerticesCount; ++i)
		{
			// считать веса и номера костей
			size_t bonesCount;
			std::cin >> bonesCount;
			bones.resize(bonesCount);
			for(size_t j = 0; j < bonesCount; ++j)
				std::cin >> bones[j].second >> bones[j].first;
			std::sort(bones.begin(), bones.end());
			// если костей слишком много
			if(bones.size() > 4)
				// выбрать самые важные
				bones.erase(bones.begin(), bones.end() - 4);
			// если костей слишком мало, дополнить нулями
			while(bones.size() < 4)
				bones.push_back(std::make_pair(0.0f, 0));
			// нормализовать веса
			float weightsSum = 0;
			for(int j = 0; j < 4; ++j)
				weightsSum += bones[j].first;
			if(weightsSum <= 1e-8f)
				THROW("Bone with zero sum weight");
			float weightCoef = 1.0f / weightsSum;
			for(int j = 0; j < 4; ++j)
				bones[j].first *= weightCoef;

			// считать позицию для проверки
			vec3 position;
			std::cin >> position.x >> position.y >> position.z;

			// записать
			for(int j = 0; j < 4; ++j)
			{
				skinCoefs[i].boneNumbers[j] = bones[j].second;
				skinCoefs[i].boneWeights[j] = bones[j].first;
			}
			skinCoefs[i].position = position;
		}

		// сформировать вершины
		for(size_t i = 0; i < vertices.size(); ++i)
		{
			const Vertex& v = vertices[i];
			SkinnedVertex& sv = skinnedVertices[i];

			sv.position = v.position;
			sv.normal = v.normal;
			sv.texcoord = v.texcoord;

			int skinNumber = positionNumbers[i];
			const SkinCoef& skinCoef = skinCoefs[skinNumber];
			// проверить, что позиция совпадает
			if(fabs(length(sv.position - skinCoef.position)) > 1e-5f)
			{
				printf("Vertex %d: position: %f %f %f, skin position: %f %f %f\n", i, sv.position.x, sv.position.y, sv.position.z,
					skinCoef.position.x, skinCoef.position.y, skinCoef.position.z);
			}
			for(int j = 0; j < 4; ++j)
			{
				sv.boneNumbers[j] = skinCoef.boneNumbers[j];
				sv.boneWeights[j] = skinCoef.boneWeights[j];
			}
		}

		ptr<Graphics::EditableGeometry<SkinnedVertex, unsigned> > skinnedGeometry = NEW(Graphics::EditableGeometry<SkinnedVertex, unsigned>(skinnedVertices, geometry->GetIndices()));

		ptr<Graphics::EditableGeometry<SkinnedVertex> > optimizedSkinnedGeometry = skinnedGeometry->Optimize()->CastIndices<unsigned short>();

		if(0)
		{
			const std::vector<SkinnedVertex>& vs = optimizedSkinnedGeometry->GetVertices();
			const std::vector<unsigned short>& is = optimizedSkinnedGeometry->GetIndices();
			for(size_t i = 0; i < vs.size(); ++i)
			{
				std::cout << vs[i].position;
				for(size_t j = 0; j < 4; ++j)
					std::cout << " (" << vs[i].boneNumbers[j] << ' ' << vs[i].boneWeights[j] << ")";
				std::cout << '\n';
			}
		}

		fileSystem->SaveFile(optimizedSkinnedGeometry->SerializeVertices(), arguments[1] + ".vertices");
		fileSystem->SaveFile(optimizedSkinnedGeometry->SerializeIndices(), arguments[1] + ".indices");
	}
	else
	{
		ptr<Graphics::EditableGeometry<Vertex> > optimizedGeometry = geometry->Optimize()->CastIndices<unsigned short>();

		fileSystem->SaveFile(optimizedGeometry->SerializeVertices(), arguments[1] + ".vertices");
		fileSystem->SaveFile(optimizedGeometry->SerializeIndices(), arguments[1] + ".indices");
	}
}

ptr<Graphics::EditableGeometry<Vertex, unsigned> > WavefrontObj::Convert(std::vector<int>& positionNumbers)
{
	std::vector<vec3> points;
	std::vector<vec3> normals;
	std::vector<vec2> texPoints;
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
			vec3 v;
			s >> v.x >> v.y >> v.z;
			points.push_back(v);
		}
		//нормаль к вершине
		else if(command == "vn")
		{
			vec3 v;
			s >> v.x >> v.y >> v.z;
			normals.push_back(v);
		}
		//текстурные координаты вершины
		else if(command == "vt")
		{
			vec3 v;
			s >> v.x >> v.y >> v.z;
			//развернуть ось Y, так как в максе начало координат текстуры внизу
			v.y = 1.0f - v.y;
			texPoints.push_back(vec2(v.x, v.y));
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
				positionNumbers.push_back(pointNumber - 1);
			}
			//поменять порядок вершин (надо!)
			std::swap(vertices[vertices.size() - 1], vertices[vertices.size() - 3]);
			std::swap(positionNumbers[vertices.size() - 1], positionNumbers[vertices.size() - 3]);
		}
	}

	//создать модель, оптимизировать и вернуть
	return MakePointer(NEW(Graphics::EditableGeometry<Vertex, unsigned>(vertices)));
}
