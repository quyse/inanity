#include "Shape.hpp"
#include "World.hpp"
#include "../meta/impl.hpp"

BEGIN_INANITY_PHYSICS

META_CLASS(Shape, Inanity.Physics.Shape);
META_CLASS_END();

Shape::Shape(ptr<World> world) : world(world) {}

END_INANITY_PHYSICS
