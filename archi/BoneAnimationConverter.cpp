#include "BoneAnimationConverter.hpp"
#include <iostream>

String BoneAnimationConverter::GetCommand() const
{
	return "ba";
}

void BoneAnimationConverter::PrintHelp() const
{
	std::cout << "Converts a bone animation from dronimal-formatted text file to binary Inanity format. Usage:\n";
	std::cout << "skeleton <source .tba> <result .ba>\n";
}

void BoneAnimationConverter::Run(const std::vector<String>& arguments)
{
	if(arguments.size() < 2)
		THROW_PRIMARY_EXCEPTION("Must be at least 2 arguments");

	if(!_wfreopen(Strings::UTF82Unicode(arguments[0]).c_str(), L"r", stdin))
		THROW_PRIMARY_EXCEPTION("Can't open source file");

	StreamWriter writer(FolderFileSystem::GetNativeFileSystem()->SaveStream(arguments[1]));

	size_t bonesCount;
	std::cin >> bonesCount;
	writer.WriteShortly(bonesCount);

	const float timeScale = 1.0f / 30;

	// цикл по ключам
	struct Key
	{
		float time;
		int boneNumber;
		quaternion orientation;
		float3 rootOffset;
	};
	std::vector<Key> keys;

	for(;;)
	{
		int boneNumber;
		std::cin >> boneNumber;
		if(boneNumber < 0)
			break;

		Key key;
		key.boneNumber = boneNumber;

		int frameNumber;
		std::cin >> frameNumber;
		key.time = (frameNumber - 1) * timeScale;

		quaternion worldOrientation;
		std::cin >> worldOrientation.x >> worldOrientation.y >> worldOrientation.z >> worldOrientation.w;
		if(boneNumber)
		{
			quaternion parentWorldOrientation;
			std::cin >> parentWorldOrientation.x >> parentWorldOrientation.y >> parentWorldOrientation.z >> parentWorldOrientation.w;

			key.orientation = (worldOrientation * parentWorldOrientation.conjugate()).normalize();
		}
		else
		{
			key.orientation = worldOrientation.normalize();
			std::cin >> key.rootOffset.x >> key.rootOffset.y >> key.rootOffset.z;
		}

		keys.push_back(key);
	}

	// записать ключи
	writer.WriteShortly(keys.size());
	for(size_t i = 0; i < keys.size(); ++i)
	{
		Key key = keys[i];
		writer.Write(key.time);
		writer.WriteShortly(key.boneNumber);
		writer.Write(key.orientation);
		if(key.boneNumber == 0)
			writer.Write(key.rootOffset);
	}
}
