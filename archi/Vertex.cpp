#include "Vertex.hpp"

bool operator<(const Vertex& a, const Vertex& b)
{
	return a.position < b.position || (a.position == b.position &&
		(a.normal < b.normal || (a.normal == b.normal &&
		(a.texcoord < b.texcoord))));
}

bool operator==(const Vertex& a, const Vertex& b)
{
	return a.position == b.position && a.normal == b.normal && a.texcoord == b.texcoord;
}

bool operator<(const BumpVertex& a, const BumpVertex& b)
{
	return a.position < b.position || (a.position == b.position &&
		(a.transform1 < b.transform1 || (a.transform1 == b.transform1 &&
		(a.transform2 < b.transform2 || (a.transform2 == b.transform2 &&
		(a.transform3 < b.transform3 || (a.transform3 == b.transform3
		&& a.texcoord < b.texcoord)))))));
}

bool operator==(const BumpVertex& a, const BumpVertex& b)
{
	return a.position == b.position && a.transform1 == b.transform1 && a.transform2 == b.transform2 && a.transform3 == b.transform3 && a.texcoord == b.texcoord;
}

bool operator<(const SkinnedVertex& a, const SkinnedVertex& b)
{
	for(int i = 0; i < 4; ++i)
	{
		if(a.boneNumbers[i] < b.boneNumbers[i])
			return true;
		if(a.boneNumbers[i] > b.boneNumbers[i])
			return false;
		if(a.boneWeights[i] < b.boneWeights[i])
			return true;
		if(a.boneWeights[i] > b.boneWeights[i])
			return false;
	}
	return a.position < b.position || (a.position == b.position &&
		(a.normal < b.normal || (a.normal == b.normal &&
		(a.texcoord < b.texcoord// || a.texcoord == b.texcoord &&
		))));
}

bool operator==(const SkinnedVertex& a, const SkinnedVertex& b)
{
	for(int i = 0; i < 4; ++i)
		if(a.boneNumbers[i] != b.boneNumbers[i] || a.boneWeights[i] != b.boneWeights[i])
			return false;
	return a.position == b.position && a.normal == b.normal && a.texcoord == b.texcoord;
}
