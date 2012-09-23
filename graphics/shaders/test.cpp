#include "Language.hpp"
#include "HlslGenerator.hpp"
#include "HlslGeneratorInstance.hpp"
#include <iostream>
#include <fstream>
using namespace Inanity::Shaders;

void run()
{
	struct Vertex
	{
		float3 position;
		float dummy1;
		float3 normal;
		float dummy2;
		float2 texcoord;
	};
	struct VertexShaderTemp
	{
		float4 position;
		float2 normal;
	};
	struct Vertex2Pixel
	{
		float4 position;
		float3 normal;
		float dummy2;
		float2 texcoord;
	};
	struct PixelShaderTemp
	{
	};
	struct Pixel
	{
		float4 target;
	};
	struct CameraUniforms
	{
		float4x4 viewTransform;
	};
	struct LightUniforms
	{
		float3 lightPosition;
		float dummy;
		float3 lightDirection;
	};
	struct ObjectUniforms
	{
		float4x4 worldTransform;
		float4x4 worldViewTransform;
	};

	struct Uniforms
	{
		CameraUniforms camera;
		LightUniforms light;
		ObjectUniforms object;
	};

	HlslGenerator gen;

	{
		SHADER_BEGIN(Vertex, Vertex2Pixel, Uniforms, VertexShaderTemp)
			SHADER_UNIFORM_BUFFER(0, camera);
			SHADER_UNIFORM_BUFFER(1, light);
			SHADER_UNIFORM_BUFFER(2, object);
			OUTPUT_SEMANTIC(position, VertexPosition);

			SHADER_CODE_BEGIN()
				OUT(position) = F::mul(F::float4_xyz_w(SWIZZLE(IN(position), xyz), CFLOAT(1.0f)), UNIFORM(2, ObjectUniforms::worldViewTransform)),
				OUT(normal) = F::mul(IN(normal), F::cast4x4to3x3(UNIFORM(2, ObjectUniforms::worldTransform))),
				OUT(texcoord) = IN(texcoord)
			SHADER_CODE_END();
			std::fstream("test\\vs.hlsl", std::ios::out) << gen.Generate(shaderBuilder);
		SHADER_END();
	}

	{
		SHADER_BEGIN(Vertex2Pixel, Pixel, Uniforms, PixelShaderTemp)
			SHADER_UNIFORM_BUFFER(2, object);
			INPUT_SEMANTIC(position, VertexPosition);
			OUTPUT_SEMANTIC(target, TargetColor0);

			SHADER_CODE_BEGIN()
				OUT(target) = F::float4_xy_z_w(F::sample(SAMPLER(0), SWIZZLE(IN(texcoord), xy)), CFLOAT(0), CFLOAT(1))
			SHADER_CODE_END();
			std::fstream("test\\ps.hlsl", std::ios::out) << gen.Generate(shaderBuilder);
		SHADER_END();
	}
}

int main()
{
#define CATCHING
#ifdef CATCHING
	try
	{
#endif
		run();
#ifdef CATCHING
	}
	catch(Exception* exception)
	{
		std::ostringstream s;
		MakePointer(exception)->PrintStack(s);
		std::cout << s.str();
	}
#endif
	return 0;
}
