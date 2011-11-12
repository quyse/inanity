#ifndef ___INANITY_RENDER_STAGE_HPP___
#define ___INANITY_RENDER_STAGE_HPP___

#include "dx.hpp"
#include "d3d.hpp"

BEGIN_INANITY_DX

class Context;
class RenderBuffer;
class DepthStencilBuffer;
/// Класс стадии рендеринга.
/** Хранит список рендербуферов, и буфер глубины/трафарета, а также может их очищать. */
class RenderStage : public Object
{
public:
	static const unsigned maxRenderTargetsCount = D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT;

private:
	ptr<RenderBuffer> renderTargets[maxRenderTargetsCount];
	bool clearTargets[maxRenderTargetsCount];
	float clearTargetColors[maxRenderTargetsCount][4];
	ptr<DepthStencilBuffer> depthStencilBuffer;
	bool clearDepth;
	bool clearStencil;
	float clearDepthValue;
	unsigned clearStencilValue;

	unsigned renderTargetsCount;

public:
	RenderStage();

	void ResetRenderTargets();
	void SetRenderTarget(unsigned slot, ptr<RenderBuffer> renderBuffer);
	void SetRenderTargetClearing(unsigned slot, bool clear, const float* color = 0);
	void SetDepthStencilBuffer(ptr<DepthStencilBuffer> depthStencilBuffer);
	void SetDepthClearing(bool clear, float depth = 1.0f);
	void SetStencilClearing(bool clear, unsigned stencil = 0);

	void Apply(Context* context);
};

END_INANITY_DX

#endif
