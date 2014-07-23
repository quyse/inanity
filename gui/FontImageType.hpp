#ifndef ___INANITY_GUI_FONT_IMAGE_TYPE_HPP___
#define ___INANITY_GUI_FONT_IMAGE_TYPE_HPP___

#include "gui.hpp"

BEGIN_INANITY_GUI

struct FontImageTypes
{
	enum _
	{
		grayscale,
		distanceField
	};
};
typedef FontImageTypes::_ FontImageType;

END_INANITY_GUI

#endif
