#ifndef ___INANITY_GRAPHICS_RENDER_STAGE_HPP___
#define ___INANITY_GRAPHICS_RENDER_STAGE_HPP___

#include "graphics.hpp"
#include "Context.hpp"
#include "RenderBuffer.hpp"
#include "DepthStencilBuffer.hpp"
#include <vector>

BEGIN_INANITY_GRAPHICS

/// Класс стадии рендеринга.
/** Хранит список рендербуферов, и буфер глубины/трафарета, а также может их очищать. */
class RenderStage : public Object
{
private:
	struct RenderTarget
	{
		ptr<RenderBuffer> renderBuffer;
		bool clear;
		float clearColor[4];

		RenderTarget();
	};

	std::vector<RenderTarget> renderTargets;
	std::vector<ptr<RenderBuffer> > renderBuffers;
	ptr<DepthStencilBuffer> depthStencilBuffer;
	bool clearDepth;
	bool clearStencil;
	float clearDepthValue;
	unsigned clearStencilValue;

public:
	RenderStage();

	void ResetRenderTargets();
	void SetRenderTarget(size_t slot, ptr<RenderBuffer> renderBuffer, bool clear = false, const float* color = 0);
	void SetDepthStencilBuffer(ptr<DepthStencilBuffer> depthStencilBuffer);
	void SetDepthClearing(bool clear, float depth = 1.0f);
	void SetStencilClearing(bool clear, unsigned stencil = 0);

	void Apply(Context* context);
};

END_INANITY_GRAPHICS

#endif
