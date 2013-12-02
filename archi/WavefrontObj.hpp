#ifndef ___INANITY_ARCHI_WAVEFRONT_OBJ_H___
#define ___INANITY_ARCHI_WAVEFRONT_OBJ_H___

#include "Processor.hpp"

struct Vertex
{
	vec3 position;
	vec3 normal;
	vec2 texcoord;
};

struct NormalVertex
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
