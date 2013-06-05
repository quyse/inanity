#ifndef ___INANITY_INANITY_GRAPHICS_HPP___
#define ___INANITY_INANITY_GRAPHICS_HPP___

/* Общая графика.
 */

#include "Object.hpp"

// В Windows используется подсистема DX, использующая DirectX 11
#ifdef ___INANITY_WINDOWS
#define ___INANITY_GRAPHICS_DX
#endif
// В Linux используется подсистема GL, использующая OpenGL
#ifdef ___INANITY_LINUX
#define ___INANITY_GRAPHICS_GL
#endif

#include "graphics/graphics.hpp"
#include "graphics/Adapter.hpp"
#include "graphics/AttributeBinding.hpp"
#include "graphics/AttributeLayout.hpp"
#include "graphics/AttributeLayoutElement.hpp"
#include "graphics/AttributeLayoutSlot.hpp"
#include "graphics/BlendState.hpp"
#include "graphics/BmpImageLoader.hpp"
#include "graphics/Context.hpp"
#include "graphics/ContextState.hpp"
#include "graphics/DataType.hpp"
#include "graphics/DepthStencilBuffer.hpp"
#include "graphics/Device.hpp"
#include "graphics/EditableFont.hpp"
#include "graphics/EditableGeometry.hpp"
#include "graphics/Font.hpp"
#include "graphics/FontAlignment.hpp"
#include "graphics/FontChar.hpp"
#include "graphics/FontManager.hpp"
#include "graphics/Image2DData.hpp"
#include "graphics/ImageLoader.hpp"
#include "graphics/IndexBuffer.hpp"
#include "graphics/Monitor.hpp"
#include "graphics/MonitorMode.hpp"
#include "graphics/Output.hpp"
#include "graphics/PixelFormat.hpp"
#include "graphics/PixelShader.hpp"
#include "graphics/PngImageLoader.hpp"
#include "graphics/Presenter.hpp"
#include "graphics/PresentMode.hpp"
#include "graphics/RenderBuffer.hpp"
#include "graphics/RenderStage.hpp"
#include "graphics/SamplerState.hpp"
#include "graphics/ShaderCache.hpp"
#include "graphics/ShaderSource.hpp"
#include "graphics/System.hpp"
#include "graphics/SystemSettings.hpp"
#include "graphics/TextDrawer.hpp"
#include "graphics/Texture.hpp"
#include "graphics/TextureManager.hpp"
#include "graphics/UniformBuffer.hpp"
#include "graphics/VertexBuffer.hpp"
#include "graphics/VertexLayout.hpp"
#include "graphics/VertexLayoutElement.hpp"
#include "graphics/VertexShader.hpp"

#ifdef ___INANITY_WINDOWS
#include "Win32Window.hpp"
#endif

#endif
