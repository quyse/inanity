#include "Shape.hpp"
#include "World.hpp"
#include "../scripting_impl.hpp"

BEGIN_INANITY_PHYSICS

SCRIPTABLE_MAP_BEGIN(Shape, Inanity.Physics.Shape);
SCRIPTABLE_MAP_END();

Shape::Shape(ptr<World> world) : world(world) {}

END_INANITY_PHYSICS
