#include "SkeletonConverter.hpp"
#include <iostream>

String SkeletonConverter::GetCommand() const
{
	return "skeleton";
}

void SkeletonConverter::PrintHelp() const
{
	std::cout << "Converts a skeleton from dronimal-formatted text file to binary Inanity format. Usage:\n";
	std::cout << "skeleton <source .tskeleton> <result .skeleton>\n";
}

void SkeletonConverter::Run(const std::vector<String>& arguments)
{
	if(arguments.size() < 2)
		THROW_PRIMARY_EXCEPTION("Must be at least 2 arguments");

	if(!_wfreopen(Strings::UTF82Unicode(arguments[0]).c_str(), L"r", stdin))
		THROW_PRIMARY_EXCEPTION("Can't open source file");

	StreamWriter writer(FolderFileSystem::GetNativeFileSystem()->SaveFileAsStream(arguments[1]));

	size_t bonesCount;
	std::cin >> bonesCount;

	struct Bone
	{
		int parent;
		quaternion worldOrientation;
		float3 worldOffset;
	};

	std::vector<Bone> bones(bonesCount);

	for(size_t i = 0; i < bonesCount; ++i)
	{
		Bone& bone = bones[i];
		std::cin >> bone.parent
			>> bone.worldOrientation.x >> bone.worldOrientation.y >> bone.worldOrientation.z >> bone.worldOrientation.w
			>> bone.worldOffset.x >> bone.worldOffset.y >> bone.worldOffset.z;
	}

	writer.WriteShortly(bonesCount);

	// преобразовать из мировых координат
	for(size_t i = 0; i < bonesCount; ++i)
	{
		Bone& bone = bones[i];

		// оригинальные ориентация и смещение для кости
		quaternion orientation;
		float3 offset;

		if(i == 0)
		{
			orientation = bone.worldOrientation;
			offset = float3(0, 0, 0);
		}
		else
		{
			quaternion inverseParentWorldOrientation = bones[bone.parent].worldOrientation.conjugate();

			// относительная ориентация
			orientation = inverseParentWorldOrientation * bone.worldOrientation;
			// относительный сдвиг
			offset = (bone.worldOffset - bones[bone.parent].worldOffset) * inverseParentWorldOrientation;
		}

		// записать
		writer.WriteShortly(bone.parent);
		writer.Write(orientation);
		writer.Write(offset);
	}
}
