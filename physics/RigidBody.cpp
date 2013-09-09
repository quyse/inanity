#include "RigidBody.hpp"
#include "World.hpp"
#include "Shape.hpp"

BEGIN_INANITY_PHYSICS

RigidBody::RigidBody(ptr<World> world, ptr<Shape> shape)
: world(world), shape(shape) {}

ptr<World> RigidBody::GetWorld() const
{
	return world;
}

ptr<Shape> RigidBody::GetShape() const
{
	return shape;
}

END_INANITY_PHYSICS
