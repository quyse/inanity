#ifndef ___INANITY_DX_SPRITE_DRAWER_HPP___
#define ___INANITY_DX_SPRITE_DRAWER_HPP___

#include "dx.hpp"

BEGIN_INANITY

class ResourceLoader;

END_INANITY

BEGIN_INANITY_DX

class VertexShader;
class PixelShader;
class Geometry;
class ShaderLayout;
class SamplerState;
class ConstantBuffer;
class Context;

/// Класс, выполняющий рендеринг спрайтов.
class SpriteDrawer : public Object
{
private:
	ptr<VertexShader> vertexShader;
	ptr<PixelShader> pixelShader;
	ptr<Geometry> geometry;
	ptr<ShaderLayout> shaderLayout;
	ptr<SamplerState> samplerState;
	ptr<ConstantBuffer> constantBuffer;

public:
	SpriteDrawer(ptr<VertexShader> vertexShader, ptr<PixelShader> pixelShader, ptr<Geometry> geometry, ptr<ShaderLayout> shaderLayout, ptr<SamplerState> samplerState);

	void Draw(Context* context, Texture* texture, const float2& position1, const float2& position2, const float2& texcoord1, const float2& texcoord2);

	static ptr<SpriteDrawer> CreateStaticResource(ResourceLoader* resourceLoader);
};

END_INANITY_DX

#endif
