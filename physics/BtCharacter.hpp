#ifndef ___INANITY_PHYSICS_BT_CHARACTER_HPP___
#define ___INANITY_PHYSICS_BT_CHARACTER_HPP___

#include "Character.hpp"
#include "bt.hpp"

BEGIN_INANITY_PHYSICS

class BtWorld;
class BtShape;

/// Класс персонажа Bullet.
class BtCharacter : public Character
{
private:
	btPairCachingGhostObject* ghost;
	btKinematicCharacterController* controller;

public:
	BtCharacter(ptr<BtWorld> world, ptr<BtShape> shape, btPairCachingGhostObject* ghost, btKinematicCharacterController* controller);
	~BtCharacter();

	btPairCachingGhostObject* GetInternalGhost() const;
	btKinematicCharacterController* GetInternalController() const;

	void Walk(const float3& movement);
	float4x4 GetTransform() const;
};

END_INANITY_PHYSICS

#endif
