#ifndef ___INANITY_GRAPHICS_LAYOUT_DATA_TYPE_HPP___
#define ___INANITY_GRAPHICS_LAYOUT_DATA_TYPE_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

/// Format of data in layouts.
struct LayoutDataTypes
{
	enum _
	{
		Float32,
		Uint32,
		Uint16,
		Uint8,
		Int32,
		Int16,
		Int8
	};
};

typedef LayoutDataTypes::_ LayoutDataType;

END_INANITY_GRAPHICS

#endif
