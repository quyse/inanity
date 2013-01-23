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
#include "graphics/BlendState.hpp"
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
#include "graphics/FontDrawer.hpp"
#include "graphics/Geometry.hpp"
#include "graphics/GeometryFormat.hpp"
#include "graphics/GeometrySemantic.hpp"
#include "graphics/IndexBuffer.hpp"
#include "graphics/Layout.hpp"
#include "graphics/LayoutBinding.hpp"
#include "graphics/Output.hpp"
#include "graphics/PixelFormat.hpp"
#include "graphics/PixelShader.hpp"
#include "graphics/Presenter.hpp"
#include "graphics/PresentMode.hpp"
#include "graphics/RenderBuffer.hpp"
#include "graphics/RenderStage.hpp"
#include "graphics/SamplerState.hpp"
#include "graphics/ShaderCache.hpp"
#include "graphics/ShaderSource.hpp"
#include "graphics/System.hpp"
#include "graphics/SystemSettings.hpp"
#include "graphics/Texture.hpp"
#include "graphics/UniformBuffer.hpp"
#include "graphics/VertexBuffer.hpp"
#include "graphics/VertexShader.hpp"

#ifdef ___INANITY_WINDOWS
#include "Win32Window.hpp"
#endif

#endif
