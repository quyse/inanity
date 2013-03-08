#ifndef ___INANITY_PHYSICS_SHAPE_HPP___
#define ___INANITY_PHYSICS_SHAPE_HPP___

#include "physics.hpp"
#include "../scripting_decl.hpp"

BEGIN_INANITY_PHYSICS

class World;

/// Абстрактный класс формы физического объекта.
class Shape : public Object
{
protected:
	ptr<World> world;

public:
	Shape(ptr<World> world);

	SCRIPTABLE_CLASS(Shape);
};

END_INANITY_PHYSICS

#endif
