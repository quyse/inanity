#ifndef ___INANITY_ARCHI_VERTEX_HPP___
#define ___INANITY_ARCHI_VERTEX_HPP___

#include "general.hpp"

struct Vertex
{
	vec3 position;
	vec3 normal;
	vec2 texcoord;
};

struct BumpVertex
{
	vec3 position;
	vec3 transform1;
	vec3 transform2;
	vec3 transform3;
	vec2 texcoord;
};

struct SkinnedVertex
{
	vec3 position;
	vec3 normal;
	vec2 texcoord;
	unsigned char boneNumbers[4];
	float boneWeights[4];
};

bool operator<(const Vertex& a, const Vertex& b);
bool operator==(const Vertex& a, const Vertex& b);
bool operator<(const BumpVertex& a, const BumpVertex& b);
bool operator==(const BumpVertex& a, const BumpVertex& b);
bool operator<(const SkinnedVertex& a, const SkinnedVertex& b);
bool operator==(const SkinnedVertex& a, const SkinnedVertex& b);

#endif
