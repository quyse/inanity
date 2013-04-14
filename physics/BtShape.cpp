#include "BtShape.hpp"
#include "BtWorld.hpp"

BEGIN_INANITY_PHYSICS

BtShape::BtShape(ptr<BtWorld> world, btCollisionShape* collisionShape)
: Shape(world), collisionShape(collisionShape) {}

BtShape::~BtShape()
{
	delete collisionShape;
}

btCollisionShape* BtShape::GetInternalObject() const
{
	return collisionShape;
}

END_INANITY_PHYSICS
