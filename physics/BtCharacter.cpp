#include "BtCharacter.hpp"
#include "BtWorld.hpp"
#include "BtShape.hpp"

BEGIN_INANITY_PHYSICS

BtCharacter::BtCharacter(ptr<BtWorld> world, ptr<BtShape> shape, btPairCachingGhostObject* ghost, btKinematicCharacterController* controller)
: Character(world, shape), ghost(ghost), controller(controller) {}

BtCharacter::~BtCharacter()
{
	world.FastCast<BtWorld>()->GetInternalDynamicsWorld()->removeCollisionObject(ghost);
	delete controller;
	delete ghost;
}

btPairCachingGhostObject* BtCharacter::GetInternalGhost() const
{
	return ghost;
}

btKinematicCharacterController* BtCharacter::GetInternalController() const
{
	return controller;
}

void BtCharacter::Walk(const float3& speed)
{
	controller->setVelocityForTimeInterval(toBt(speed), 1);
}

float4x4 BtCharacter::GetTransform() const
{
	return fromBt(ghost->getWorldTransform());
}

END_INANITY_PHYSICS
