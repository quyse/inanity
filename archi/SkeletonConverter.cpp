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

void dfs(const std::vector<std::vector<int> >& h, int i)
{
	static int level = 0;
	for(int j = 0; j < level; ++j)
		printf("  ");
	printf("%d\n", i);
	++level;
	for(int j = 0; j < (int)h[i].size(); ++j)
		dfs(h, h[i][j]);
	--level;
}

void SkeletonConverter::Run(const std::vector<String>& arguments)
{
	if(arguments.size() < 2)
		THROW_PRIMARY_EXCEPTION("Must be at least 2 arguments");

	if(!_wfreopen(Strings::UTF82Unicode(arguments[0]).c_str(), L"r", stdin))
		THROW_PRIMARY_EXCEPTION("Can't open source file");

	StreamWriter writer(FolderFileSystem::GetNativeFileSystem()->SaveStream(arguments[1]));

	size_t bonesCount;
	std::cin >> bonesCount;
	writer.WriteShortly(bonesCount);

	std::vector<std::vector<int> > h(bonesCount);
	for(size_t i = 0; i < bonesCount; ++i)
	{
		int parent;
		quaternion worldOrientation;
		float3 worldOffset;
		std::cin >> parent
			>> worldOrientation.x >> worldOrientation.y >> worldOrientation.z >> worldOrientation.w
			>> worldOffset.x >> worldOffset.y >> worldOffset.z;
		if(i)
			h[parent].push_back(i);
		writer.WriteShortly(parent);
		writer.Write(worldOrientation);
		writer.Write(worldOffset);
	}

	dfs(h, 0);
}
