#include "BlendState.hpp"

String BlendState::Cache::staticResourceName = "/inanity/dx/BlendStateCache";

BlendState::BlendState(ptr<Cache> cache) : CachedState(cache)
{
	//установить настройки по умолчанию
	desc = CD3D11_BLEND_DESC(CD3D11_DEFAULT());
}
