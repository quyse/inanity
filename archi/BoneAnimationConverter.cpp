#include "BoneAnimationConverter.hpp"
#include <stack>
#include <iostream>

String BoneAnimationConverter::GetCommand() const
{
	return "ba";
}

void BoneAnimationConverter::PrintHelp() const
{
	std::cout << "Converts a bone animation from dronimal-formatted text file to binary Inanity format. Usage:\n";
	std::cout << "skeleton <source .tba> <source .tskeleton> <result .ba>\n";
}

void BoneAnimationConverter::Run(const std::vector<String>& arguments)
{
	if(arguments.size() < 3)
		THROW_PRIMARY_EXCEPTION("Must be at least 3 arguments");

	// считать скелет
	if(!_wfreopen(Strings::UTF82Unicode(arguments[1]).c_str(), L"r", stdin))
		THROW_PRIMARY_EXCEPTION("Can't open source .tskeleton file");
	int bonesCount;
	std::cin >> bonesCount;
	struct Bone
	{
		quaternion originalWorldOrientation;
		float3 originalWorldPosition;
		int parent;
		std::vector<int> children;

		quaternion lastRelativeOrientation;
	};
	std::vector<Bone> bones(bonesCount);
	for(int i = 0; i < bonesCount; ++i)
	{
		int& parent = bones[i].parent;
		quaternion& o = bones[i].originalWorldOrientation;
		float3& p = bones[i].originalWorldPosition;
		std::cin >> parent
			>> o.x >> o.y >> o.z >> o.w
			>> p.x >> p.y >> p.z;
		if(i)
			bones[parent].children.push_back(i);
	}

	// отсортировать кости топологически
	std::vector<int> sortedBones;
	sortedBones.reserve(bones.size());
	{
		std::vector<bool> f(bones.size());
		// DFS без рекурсии
		std::stack<int> s;
		for(int i = 0; i < bonesCount; ++i)
		{
			while(i >= 0 && !f[i])
			{
				f[i] = true;
				s.push(i);
				i = bones[i].parent;
			}

			while(!s.empty())
			{
				sortedBones.push_back(s.top());
				s.pop();
			}
		}
	}

	// открыть файл с текстовой анимацией
	if(!_wfreopen(Strings::UTF82Unicode(arguments[0]).c_str(), L"r", stdin))
		THROW_PRIMARY_EXCEPTION("Can't open source .tba file");

	StreamWriter writer(FolderFileSystem::GetNativeFileSystem()->SaveStream(arguments[2]));

	int bonesCountInAnimation;
	std::cin >> bonesCountInAnimation;
	if(bonesCount != bonesCountInAnimation)
		THROW_PRIMARY_EXCEPTION("Number of bones is different in animation and skeleton");
	writer.WriteShortly(bonesCount);

	const float timeScale = 1.0f / 30;

	// ключи
	struct Key
	{
		float time;
		int boneNumber;
		quaternion orientation;
		float3 rootOffset;
	};
	std::vector<Key> keys;

	// считать кадры
	struct FrameBone
	{
		quaternion worldOrientation;
		quaternion relativeOrientation;
	};
	std::vector<FrameBone> frameBones(bonesCount);
	for(;;)
	{
		// считать номер кадра
		int frameNumber;
		std::cin >> frameNumber;
		if(frameNumber < 0)
			break;

		float keyTime = (frameNumber - 1) * timeScale;

		// считать номера костей в этом кадре
		int hasKeyBonesCount;
		std::cin >> hasKeyBonesCount;
		std::vector<int> hasKeyBones(hasKeyBonesCount);
		for(int i = 0; i < hasKeyBonesCount; ++i)
			std::cin >> hasKeyBones[i];

		// считать все кости
		float3 rootOffset;
		for(int i = 0; i < bonesCount; ++i)
		{
			// считать мировую ориентацию кости
			quaternion& q = frameBones[i].worldOrientation;
			std::cin >> q.x >> q.y >> q.z >> q.w;

			if(!i)
				// считать смещение
				std::cin >> rootOffset.x >> rootOffset.y >> rootOffset.z;
		}

		// получить относительные ориентации костей и скорректировать их
		for(int i = 0; i < bonesCount; ++i)
		{
			int boneNumber = sortedBones[i];
			frameBones[boneNumber].relativeOrientation =
				i ?
				(frameBones[bones[boneNumber].parent].worldOrientation.conjugate() * frameBones[boneNumber].worldOrientation).normalize()
				:
				frameBones[boneNumber].worldOrientation.normalize();
			if(dot(frameBones[boneNumber].relativeOrientation, bones[boneNumber].lastRelativeOrientation) < 0)
			{
				frameBones[boneNumber].relativeOrientation = -frameBones[boneNumber].relativeOrientation;
				frameBones[boneNumber].worldOrientation = -frameBones[boneNumber].worldOrientation;
			}
			bones[boneNumber].lastRelativeOrientation = frameBones[boneNumber].relativeOrientation;
		}

		// добавить ключи
		for(size_t i = 0; i < hasKeyBonesCount; ++i)
		{
			int boneNumber = hasKeyBones[i];
			Key key;
			key.time = keyTime;
			key.boneNumber = boneNumber;
			key.orientation = frameBones[boneNumber].relativeOrientation;
			if(boneNumber == 0)
				key.rootOffset = rootOffset;
			keys.push_back(key);
		}
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
