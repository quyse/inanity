#ifndef ___INANITY_GRAPHICS_SHADERS_SAMPLER_TYPE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_SAMPLER_TYPE_HPP___

#include "shaders.hpp"

BEGIN_INANITY_SHADERS

/// Тип семплера.
struct SamplerTypes
{
	enum _
	{
		_None,
		_1D,
		_2D,
		_3D,
		_Cube
	};
};
typedef SamplerTypes::_ SamplerType;

END_INANITY_SHADERS

#endif
