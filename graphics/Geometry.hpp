#ifndef ___INANITY_GRAPHICS_GEOMETRY_HPP___
#define ___INANITY_GRAPHICS_GEOMETRY_HPP___

#include "graphics.hpp"
#include "../scripting_decl.hpp"

BEGIN_INANITY_GRAPHICS

/// Абстрактный класс геометрии.
/** Хранит данные геометрии, необходимые для рисования. */
class Geometry : public Object
{
public:

	SCRIPTABLE_CLASS(Geometry);
};

END_INANITY_GRAPHICS

#endif
