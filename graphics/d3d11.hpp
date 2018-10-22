#ifndef ___INANITY_D3D11_HPP___
#define ___INANITY_D3D11_HPP___

#include "../platform/windows.hpp"
#include <InitGuid.h>

#if defined(___INANITY_PLATFORM_XBOX)

#include <d3d11_x.h>
#include <d3dcompiler_x.h>

#else

#include <d3d11.h>
#include <d3dcompiler.h>

#endif

#endif
