#include "BtShape.hpp"
#include "BtWorld.hpp"

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
