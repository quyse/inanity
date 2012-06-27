#ifndef ___INANITY_DX_CONTEXT_STATE_HPP___
#define ___INANITY_DX_CONTEXT_STATE_HPP___

#include "dx.hpp"

BEGIN_INANITY_DX

/// Класс состояния контекста.
/** Позволяет сохранить несколько различных аспектов состояния контекста,
и применять их разом к контексту. */
class ContextState : public Object
{
private:
	ptr<VertexShader> vertexShader;
	ptr<PixelShader> pixelShader;
	struct Stage
	{
		ptr<SamplerState> samplerStates[8];
	};
	static const int stagesCount = 2;
	Stage stages[stagesCount];
	ptr<DepthStencilState> depthStencilState;
	ptr<BlendState> blendState;
	ptr<RasterizerState> rasterizerState;

public:
	ContextState();

	/// Применить состояние к контексту.
	void Apply(Context* context);
};

END_INANITY_DX

#endif
