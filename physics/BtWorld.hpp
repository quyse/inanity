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

	btDynamicsWorld* GetInternalDynamicsWorld() const;

	ptr<Shape> CreateBoxShape(const float3& halfSize);
	ptr<Shape> CreateSphereShape(float radius);
	ptr<Shape> CreateCapsuleShape(float radius, float height);
	ptr<RigidBody> CreateRigidBody(ptr<Shape> shape, float mass, const float4x4& startTransform);
	ptr<Character> CreateCharacter(ptr<Shape> shape, const float4x4& startTransform);
	void Simulate(float time);
};

END_INANITY_PHYSICS

#endif
