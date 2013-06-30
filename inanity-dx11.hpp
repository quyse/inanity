#ifndef ___INANITY_INANITY_DX11_HPP___
#define ___INANITY_INANITY_DX11_HPP___

/* Подсистема DX11.
 */

#include "Object.hpp"

// Если это не Windows, то капец.
#ifndef ___INANITY_WINDOWS
#error DirectX 11 works only on Windows.
#endif

#include "graphics/D3D10BlobFile.hpp"
#include "graphics/Dx11AttributeBinding.hpp"
#include "graphics/Dx11BlendState.hpp"
#include "graphics/Dx11CompiledShader.hpp"
#include "graphics/Dx11Context.hpp"
#include "graphics/Dx11DepthStencilBuffer.hpp"
#include "graphics/Dx11Device.hpp"
#include "graphics/Dx11IndexBuffer.hpp"
#include "graphics/Dx11PixelShader.hpp"
#include "graphics/Dx11Presenter.hpp"
#include "graphics/Dx11RenderBuffer.hpp"
#include "graphics/Dx11SamplerState.hpp"
#include "graphics/Dx11ShaderCompiler.hpp"
#include "graphics/Dx11ShaderResources.hpp"
#include "graphics/Dx11System.hpp"
#include "graphics/Dx11Texture.hpp"
#include "graphics/Dx11UniformBuffer.hpp"
#include "graphics/Dx11VertexBuffer.hpp"
#include "graphics/Dx11VertexShader.hpp"
#include "graphics/Hlsl11Source.hpp"

#endif
