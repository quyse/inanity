#ifndef ___INANITY_PHYSICS_BT_WORLD_HPP___
#define ___INANITY_PHYSICS_BT_WORLD_HPP___

#include "World.hpp"
#include "bt.hpp"

BEGIN_INANITY_PHYSICS

/// Класс физического мира Bullet.
class BtWorld : public World
{
private:
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* collisionDispatcher;
	btBroadphaseInterface* broadphase;
	btConstraintSolver* solver;
	btDynamicsWorld* dynamicsWorld;

public:
	BtWorld();
	~BtWorld();

	ptr<Shape> CreateBoxShape(const float3& halfSize);
	ptr<RigidBody> CreateRigidBody(ptr<Shape> shape, float mass, const float4x4& startTransform);
	void Simulate(float time);
};

END_INANITY_PHYSICS

#endif
