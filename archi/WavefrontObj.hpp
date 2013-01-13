#ifndef ___INANITY_ARCHI_WAVEFRONT_OBJ_H___
#define ___INANITY_ARCHI_WAVEFRONT_OBJ_H___

#include "Processor.hpp"

struct Vertex
{
	float3 position;
	float3 normal;
	float2 texcoord;
};

struct NormalVertex
{
	float3 position;
	float3 transform1;
	float3 transform2;
	float3 transform3;
	float2 texcoord;
};

struct SkinnedVertex
{
	float3 position;
	float3 normal;
	float2 texcoord;
	uint boneNumbers[4];
	float boneWeights[4];
};

class WavefrontObj : public Processor
{
private:
	//конвертировать файл из входного потока данных в геометрию
	ptr<Graphics::EditableGeometry<Vertex, unsigned> > Convert(std::vector<int>& positionNumbers);

public:
	String GetCommand() const;
	void PrintHelp() const;
	void Run(const std::vector<String>& arguments);
};

#endif
