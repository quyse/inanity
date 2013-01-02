#include "BtWorld.hpp"
#include "BtShape.hpp"
#include "BtRigidBody.hpp"
#include "../Exception.hpp"

BtWorld::BtWorld() :
	collisionConfiguration(0), collisionDispatcher(0), broadphase(0),
	solver(0), dynamicsWorld(0)
{
	try
	{
		collisionConfiguration = new btDefaultCollisionConfiguration();
		collisionDispatcher = new btCollisionDispatcher(collisionConfiguration);
		broadphase = new btDbvtBroadphase();
		solver = new btSequentialImpulseConstraintSolver();
		dynamicsWorld = new btDiscreteDynamicsWorld(collisionDispatcher, broadphase, solver, collisionConfiguration);

		dynamicsWorld->setGravity(btVector3(0, 0, -9.8));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create bullet world", exception);
	}
}

BtWorld::~BtWorld()
{
	if(dynamicsWorld)
		delete dynamicsWorld;
	if(solver)
		delete solver;
	if(broadphase)
		delete broadphase;
	if(collisionDispatcher)
		delete collisionDispatcher;
	if(collisionConfiguration)
		delete collisionConfiguration;
}

btDynamicsWorld* BtWorld::GetInternalDynamicsWorld() const
{
	return dynamicsWorld;
}

ptr<Shape> BtWorld::CreateBoxShape(const float3& halfSize)
{
	try
	{
		btCollisionShape* collisionShape = new btBoxShape(toBt(halfSize));

		return NEW(BtShape(this, collisionShape));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create bullet box shape", exception);
	}
}

ptr<RigidBody> BtWorld::CreateRigidBody(ptr<Shape> abstractShape, float mass, const float4x4& startTransform)
{
	try
	{
		ptr<BtShape> shape = abstractShape.FastCast<BtShape>();
		if(!shape)
			THROW_PRIMARY_EXCEPTION("Non-bullet shape");

		btCollisionShape* collisionShape = shape->GetInternalObject();
		btVector3 localInertia(0, 0, 0);
		if(mass != 0)
			collisionShape->calculateLocalInertia(mass, localInertia);

		ptr<BtRigidBody> rigidBody = NEW(BtRigidBody(this, shape, toBt(startTransform)));
		btRigidBody::btRigidBodyConstructionInfo info(mass, &*rigidBody, shape->GetInternalObject(), localInertia);
		btRigidBody* internalRigidBody = new btRigidBody(info);
		rigidBody->SetInternalObject(internalRigidBody);

		dynamicsWorld->addRigidBody(internalRigidBody);

		return rigidBody;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create bullet rigid body", exception);
	}
}

void BtWorld::Simulate(float time)
{
	dynamicsWorld->stepSimulation(time, 10);
}
