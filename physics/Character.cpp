#include "Character.hpp"
#include "World.hpp"
#include "Shape.hpp"

Character::Character(ptr<World> world, ptr<Shape> shape)
: world(world), shape(shape) {}
