#ifndef ___INANITY_GRAPHICS_DEPTH_STENCIL_STATE_HPP___
#define ___INANITY_GRAPHICS_DEPTH_STENCIL_STATE_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

class DepthStencilState : public Object
{
public:
	enum TestFunc
	{
		testFuncNever,
		testFuncLess,
		testFuncLessOrEqual,
		testFuncEqual,
		testFuncNonEqual,
		testFuncGreaterOrEqual,
		testFuncGreater,
		testFuncAlways
	};

	enum StencilOp
	{
		stencilOpKeep,
		stencilOpZero,
		stencilOpReplace,
		stencilOpIncrementClamp,
		stencilOpDecrementClamp,
		stencilOpIncrementWrap,
		stencilOpDecrementWrap,
		stencilOpInvert
	};

	struct StencilFace
	{
		TestFunc testFunc;
		StencilOp failOp;
		StencilOp depthFailOp;
		StencilOp passOp;

		StencilFace(TestFunc testFunc = testFuncAlways, StencilOp failOp = stencilOpKeep, StencilOp depthFailOp = stencilOpKeep, StencilOp passOp = stencilOpKeep);
	};

protected:
	TestFunc depthTestFunc;
	bool depthWrite;
	bool stencilEnable;
	uint8_t stencilMask;
	uint8_t stencilRef;
	StencilFace stencilFrontFace;
	StencilFace stencilBackFace;

	bool dirty;

	DepthStencilState();

public:

	void SetDepthTest(TestFunc depthTestFunc = testFuncLess, bool depthWrite = true);
	void SetStencil(bool stencilEnable, uint8_t stencilMask = 0xFF, uint8_t stencilRef = 0);
	void SetStencilFrontFace(const StencilFace& stencilFrontFace);
	void SetStencilBackFace(const StencilFace& stencilBackFace);
};

END_INANITY_GRAPHICS

#endif
