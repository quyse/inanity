#include "SpriteDrawer.hpp"

SpriteDrawer::SpriteDrawer(ptr<VertexShader> vertexShader, ptr<PixelShader> pixelShader, ptr<Geometry> geometry, ptr<ShaderLayout> shaderLayout, ptr<SamplerState> samplerState, ptr<ConstantBuffer> constantBuffer)
: vertexShader(vertexShader), pixelShader(pixelShader), geometry(geometry), shaderLayout(shaderLayout), samplerState(samplerState), constantBuffer(constantBuffer)
{
}

static struct DataForShader
{
	float2 position1;
	float2 position2;
	float2 texcoord1;
	float2 texcoord2;
};

void SpriteDrawer::Draw(Context* context, Texture* texture, const float2& position1, const float2& position2, const float2& texcoord1, const float2& texcoord2)
{
	context->ApplyGeometry(geometry);
	context->ApplyShaderLayout(shaderLayout);
	context->ApplyShaders(vertexShader, pixelShader);
	context->SetConstantBuffer(0, Context::stageVertex, constantBuffer);
	context->SetSamplerState(0, Context::stagePixel, samplerState);
}

ptr<SpriteDrawer> SpriteDrawer::CreateStaticResource(ResourceLoader* resourceLoader)
{
}
