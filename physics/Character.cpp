#include "Character.hpp"
#include "World.hpp"
#include "Shape.hpp"

BEGIN_INANITY_PHYSICS

Character::Character(ptr<World> world, ptr<Shape> shape)
: world(world), shape(shape) {}

END_INANITY_PHYSICS
