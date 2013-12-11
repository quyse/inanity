#ifndef ___INANITY_GRAPHICS_CONTEXT_HPP___
#define ___INANITY_GRAPHICS_CONTEXT_HPP___

#include "graphics.hpp"
#include "shaders/shaders.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_SHADERS

class SamplerBase;
class UniformGroup;

END_INANITY_SHADERS

BEGIN_INANITY_GRAPHICS

class FrameBuffer;
class Texture;
class SamplerState;
class UniformBuffer;
class VertexShader;
class PixelShader;
class AttributeBinding;
class VertexBuffer;
class IndexBuffer;
class BlendState;
class AttributeLayoutSlot;

/// Abstract graphics context class.
class Context : public Object
{
protected:
	struct Cell;

public:
	/// Element of state stack.
	/** Element can be immediate or deferred.
	Immediate element is initialized when created.
	Deferred element could be initialized after creation.
	*/
	struct Let
	{
		/// Cell of let.
		Cell* cell;
		/// Next let in cell stack.
		Let* next;

		Let();
		~Let();

	protected:
		void Init(Cell* cell);
	};

protected:
	/// Class stores a stack of Lets for one render state.
	struct Cell
	{
		Let* top;
		Let* current;

		Cell();

		/// Get if that state is actual.
		bool IsActual() const;
		/// Set that state is actual.
		void Actual();
		/// Reset state so it should be updated next time.
		void Reset();
	};

public:
	//******* Elements.

	/// Framebuffer element.
	struct LetFrameBuffer : public Let
	{
		ptr<FrameBuffer> frameBuffer;
		LetFrameBuffer();
		LetFrameBuffer(Context* context, ptr<FrameBuffer> frameBuffer);
		void operator()(Context* context, ptr<FrameBuffer> frameBuffer);
	};

	/// Sampler element.
	struct LetSampler : public Let
	{
		ptr<Texture> texture;
		ptr<SamplerState> samplerState;
		LetSampler();
		LetSampler(Context* context, int i, ptr<Texture> texture, ptr<SamplerState> samplerState = 0);
		LetSampler(Context* context, const Shaders::SamplerBase& sampler, ptr<Texture> texture, ptr<SamplerState> samplerState = 0);
		void operator()(Context* context, int i, ptr<Texture> texture, ptr<SamplerState> samplerState = 0);
		void operator()(Context* context, const Shaders::SamplerBase& sampler, ptr<Texture> texture, ptr<SamplerState> samplerState = 0);
	};

	/// Uniform buffer element.
	struct LetUniformBuffer : public Let
	{
		ptr<UniformBuffer> uniformBuffer;
		LetUniformBuffer();
		LetUniformBuffer(Context* context, int i, ptr<UniformBuffer> uniformBuffer);
		LetUniformBuffer(Context* context, Shaders::UniformGroup* uniformGroup);
		void operator()(Context* context, int i, ptr<UniformBuffer> uniformBuffer);
		void operator()(Context* context, Shaders::UniformGroup* uniformGroup);
	};

	//*** Shader elements.

	/// Vertex shader element.
	struct LetVertexShader : public Let
	{
		ptr<VertexShader> vertexShader;
		LetVertexShader();
		LetVertexShader(Context* context, ptr<VertexShader> vertexShader);
		void operator()(Context* context, ptr<VertexShader> vertexShader);
	};

	/// Pixel shader element.
	struct LetPixelShader : public Let
	{
		ptr<PixelShader> pixelShader;
		LetPixelShader();
		LetPixelShader(Context* context, ptr<PixelShader> pixelShader);
		void operator()(Context* context, ptr<PixelShader> pixelShader);
	};

	//*** Geometry elements.

	/// Attribute binding element.
	struct LetAttributeBinding : public Let
	{
		ptr<AttributeBinding> attributeBinding;
		LetAttributeBinding();
		LetAttributeBinding(Context* context, ptr<AttributeBinding> attributeBinding);
		void operator()(Context* context, ptr<AttributeBinding> attributeBinding);
	};

	/// Vertex buffer element.
	struct LetVertexBuffer : public Let
	{
		ptr<VertexBuffer> vertexBuffer;
		LetVertexBuffer();
		LetVertexBuffer(Context* context, int i, ptr<VertexBuffer> vertexBuffer);
		LetVertexBuffer(Context* context, AttributeLayoutSlot* slot, ptr<VertexBuffer> vertexBuffer);
		void operator()(Context* context, int i, ptr<VertexBuffer> vertexBuffer);
		void operator()(Context* context, AttributeLayoutSlot* slot, ptr<VertexBuffer> vertexBuffer);
	};

	/// Index buffer element.
	struct LetIndexBuffer : public Let
	{
		ptr<IndexBuffer> indexBuffer;
		LetIndexBuffer();
		LetIndexBuffer(Context* context, ptr<IndexBuffer> indexBuffer);
		void operator()(Context* context, ptr<IndexBuffer> indexBuffer);
	};

	//*** Rasterizer states.

	// Fill mode.
	enum FillMode
	{
		fillModeWireframe,
		fillModeSolid
	};
	struct LetFillMode : public Let
	{
		FillMode fillMode;
		LetFillMode();
		LetFillMode(Context* context, FillMode fillMode);
		void operator()(Context* context, FillMode fillMode);
	};

	// Cull mode.
	enum CullMode
	{
		cullModeNone,
		cullModeBack,
		cullModeFront
	};
	struct LetCullMode : public Let
	{
		CullMode cullMode;
		LetCullMode();
		LetCullMode(Context* context, CullMode cullMode);
		void operator()(Context* context, CullMode cullMode);
	};

	/// Viewport element.
	struct LetViewport : public Let
	{
		int viewportWidth, viewportHeight;
		LetViewport();
		LetViewport(Context* context, int viewportWidth, int viewportHeight);
		void operator()(Context* context, int viewportWidth, int viewportHeight);
	};

	// Depth test function.
	/** To disable depth test simply set this function to 'always'. */
	enum DepthTestFunc
	{
		depthTestFuncNever,
		depthTestFuncLess,
		depthTestFuncLessOrEqual,
		depthTestFuncEqual,
		depthTestFuncNonEqual,
		depthTestFuncGreaterOrEqual,
		depthTestFuncGreater,
		depthTestFuncAlways
	};
	struct LetDepthTestFunc : public Let
	{
		DepthTestFunc depthTestFunc;
		LetDepthTestFunc();
		LetDepthTestFunc(Context* context, DepthTestFunc depthTestFunc);
		void operator()(Context* context, DepthTestFunc depthTestFunc);
	};

	/// Depth write element.
	struct LetDepthWrite : public Let
	{
		bool depthWrite;
		LetDepthWrite();
		LetDepthWrite(Context* context, bool depthWrite);
		void operator()(Context* context, bool depthWrite);
	};

	/// Blend state element.
	struct LetBlendState : public Let
	{
		ptr<BlendState> blendState;
		LetBlendState();
		LetBlendState(Context* context, ptr<BlendState> blendState);
		void operator()(Context* context, ptr<BlendState> blendState);
	};

protected:
	//*** Cells.
	Cell cellFrameBuffer;
	static const int samplersCount = 16;
	Cell cellSamplers[samplersCount];
	static const int uniformBuffersCount = 8;
	Cell cellUniformBuffers[uniformBuffersCount];
	Cell cellVertexShader;
	Cell cellPixelShader;
	Cell cellAttributeBinding;
	static const int vertexBuffersCount = 2;
	Cell cellVertexBuffers[vertexBuffersCount];
	Cell cellIndexBuffer;
	Cell cellFillMode;
	Cell cellCullMode;
	Cell cellViewport;
	Cell cellDepthTestFunc;
	Cell cellDepthWrite;
	Cell cellBlendState;

	//*** Default lets for necessary cells.
	LetVertexShader letDefaultVertexShader;
	LetPixelShader letDefaultPixelShader;
	LetIndexBuffer letDefaultIndexBuffer;
	LetFillMode letDefaultFillMode;
	LetCullMode letDefaultCullMode;
	LetDepthTestFunc letDefaultDepthTestFunc;
	LetDepthWrite letDefaultDepthWrite;
	LetBlendState letDefaultBlendState;

protected:
	Context();

public:
	//******* Informational methods.
	int GetViewportWidth() const;
	int GetViewportHeight() const;

	//******* Methods to clean color/depth/stencil buffers of bound framebuffer.
	/* These are immediate methods. */
	/// Clear color buffer.
	virtual void ClearColor(int colorBufferIndex, const float* color) = 0;
	/// Clear depth in depth-stencil buffer.
	virtual void ClearDepth(float depth) = 0;
	/// Clear stencil in depth-stencil buffer.
	virtual void ClearStencil(unsigned stencil) = 0;
	/// Clear depth & stencil in depth-stencil buffer.
	virtual void ClearDepthStencil(float depth, unsigned stencil) = 0;

	//******* Methods to upload data.
	/* Immediate methods. */
	/// Upload data to uniform buffer.
	/** Size of data should be equal to buffer size. */
	virtual void UploadUniformBufferData(UniformBuffer* buffer, const void* data, int size) = 0;
	/// Upload data to dynamic vertex buffer.
	virtual void UploadVertexBufferData(VertexBuffer* buffer, const void* data, int size) = 0;

	//******* Draw methods.
	/* Immediate methods. */
	/// Do draw.
	virtual void Draw(int count = -1) = 0;
	/// Do instanced draw.
	virtual void DrawInstanced(int instancesCount, int count = -1) = 0;
};

END_INANITY_GRAPHICS

#endif
