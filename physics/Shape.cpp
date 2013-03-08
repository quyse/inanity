#include "Shape.hpp"
#include "World.hpp"
#include "../scripting_impl.hpp"

SCRIPTABLE_MAP_BEGIN(Shape, Inanity.Physics.Shape);
SCRIPTABLE_MAP_END();

Shape::Shape(ptr<World> world) : world(world) {}
