#ifndef ___INANITY_DX_SPRITE_DRAWER_HPP___
#define ___INANITY_DX_SPRITE_DRAWER_HPP___

#include "dx.hpp"

BEGIN_INANITY_DX

/// Класс, выполняющий рендеринг 
class SpriteDrawer : public Object
{
private:
	ptr<ShaderData> shaderData;
	ptr<Texture> texture;

public:
	SpriteDrawer();
	
	void StartDrawing(ptr<Texture> texture);
	void PushSprites(const float2& position, const float2& size, const float2& texcoord, const float2& texcoord2);
	void FinishDrawing();
};

END_INANITY_DX

#endif
