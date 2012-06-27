#include "Language.hpp"
#include "Functions.hpp"
#include <iostream>

struct Input
{
	float3 position;
	float3 normal;
};
struct Output
{
	float4 target;
};

Inanity::Shaders::Composer* Inanity::Shaders::Composer::current = nullptr;

int main()
{
	using namespace Inanity::Shaders;

	{
		struct Input
		{
			float4 position;
			float3 normal;
			float4x4 worldTransform;
		};
		struct Output
		{
			float4 target;
			float depth;
		};
		Composer s;
		float_ a;
		float4x4_ b, c;
		b = b + c;
		c = (b + c) * a;

		float4_ position;
		position = input(&Input::position);
		float3_ normal;
		normal = input(&Input::normal);
		float4x4_ worldTransform;
		worldTransform = input(&Input::worldTransform);

		float4_ target(output(&Output::target));
		float_ depth(output(&Output::depth));

		target = position.Cast<float4>() * worldTransform * length(normal);
		target = lerp(depth.Cast<float4>(), normal.Cast<float4>(), position);
		depth = a;

		std::cout << s.finalize();
	}

#if 2+2==3

	// код так, как он должен быть

	USING_INANITY_SHADERS;// using namespace Inanity::Shaders;

	ShaderComposer composer;
	auto iPosition = composer.InputVariable<float3>();
	auto iNormal = composer.InputVariable<float4>();
	auto iTexcoord = composer.ifloat4(); // то же, что и в предыдущей строке

	auto v2pPosition = composer.VO;

	c = a + b; // действия с переменными создают действия в шейдере


#endif

	return 0;
}
