#include "RigidBody.hpp"
#include "World.hpp"
#include "Shape.hpp"
#include "../meta/impl.hpp"

BEGIN_INANITY_PHYSICS

META_CLASS(RigidBody, Inanity.Physics.RigidBody);
META_CLASS_END();

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
