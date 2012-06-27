#ifndef ___INANITY_GEOMETRY_LAYOUT_HPP___
#define ___INANITY_GEOMETRY_LAYOUT_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс разметки геометрии.
/** Указывает, какие данные и где хранятся в геометрии. */
class GeometryLayout : public Object
{
private:
	struct Element
	{
		unsigned int code;
	};

	std::vector<Element> elements;

public:
	GeometryLayout();
};

END_INANITY_GRAPHICS

#endif
