#include "RigidBody.hpp"
#include "World.hpp"
#include "Shape.hpp"
#include "../scripting_impl.hpp"

BEGIN_INANITY_PHYSICS

SCRIPTABLE_MAP_BEGIN(RigidBody, Inanity.Physics.RigidBody);
SCRIPTABLE_MAP_END();

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
