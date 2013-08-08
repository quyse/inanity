#include "BtRigidBody.hpp"
#include "BtWorld.hpp"
#include "BtShape.hpp"

BEGIN_INANITY_PHYSICS

BtRigidBody::BtRigidBody(ptr<BtWorld> world, ptr<BtShape> shape, const btTransform& startTransform)
: RigidBody(world, shape), rigidBody(0), transform(startTransform)
{
}

BtRigidBody::~BtRigidBody()
{
	if(rigidBody)
	{
		world.FastCast<BtWorld>()->GetInternalDynamicsWorld()->removeRigidBody(rigidBody);
		delete rigidBody;
	}
}

void BtRigidBody::SetInternalObject(btRigidBody* rigidBody)
{
	this->rigidBody = rigidBody;
}

btRigidBody* BtRigidBody::GetInternalObject() const
{
	return rigidBody;
}

vec3 BtRigidBody::GetPosition() const
{
	return fromBt(transform.getOrigin());
}

mat3x3 BtRigidBody::GetOrientation() const
{
	return fromBt(transform.getBasis());
}

mat4x4 BtRigidBody::GetTransform() const
{
	return fromBt(transform);
}

void BtRigidBody::ApplyImpulse(const vec3& impulse)
{
	rigidBody->applyCentralImpulse(toBt(impulse));
}

void BtRigidBody::getWorldTransform(btTransform& transform) const
{
	transform = this->transform;
}

void BtRigidBody::setWorldTransform(const btTransform& transform)
{
	this->transform = transform;
}

END_INANITY_PHYSICS
