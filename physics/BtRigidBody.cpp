#include "BtRigidBody.hpp"
#include "BtWorld.hpp"
#include "BtShape.hpp"

BtRigidBody::BtRigidBody(ptr<BtWorld> world, ptr<BtShape> shape, const btTransform& startTransform)
: RigidBody(world, shape), rigidBody(rigidBody), transform(startTransform)
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

float3 BtRigidBody::GetPosition() const
{
	return fromBt(transform.getOrigin());
}

float3x3 BtRigidBody::GetOrientation() const
{
	return fromBt(transform.getBasis());
}

float4x4 BtRigidBody::GetTransform() const
{
	return fromBt(transform);
}

void BtRigidBody::getWorldTransform(btTransform& transform) const
{
	transform = this->transform;
}

void BtRigidBody::setWorldTransform(const btTransform& transform)
{
	this->transform = transform;
}
