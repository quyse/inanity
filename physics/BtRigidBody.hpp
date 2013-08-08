#ifndef ___INANITY_PHYSICS_BT_RIGID_BODY_HPP___
#define ___INANITY_PHYSICS_BT_RIGID_BODY_HPP___

#include "RigidBody.hpp"
#include "bt.hpp"

BEGIN_INANITY_PHYSICS

class BtWorld;
class BtShape;

/// Класс твёрдого тела Bullet.
/** Является одновременно btMotionState, для эффективного взаимодействия с bullet. */
class BtRigidBody : public RigidBody, public btMotionState
{
private:
	/// Соответствующий объект bullet.
	btRigidBody* rigidBody;
	/// Текущая трансформация твёрдого тела.
	btTransform transform;

public:
	BtRigidBody(ptr<BtWorld> world, ptr<BtShape> shape, const btTransform& startTransform);
	~BtRigidBody();

	/// Указать объект bullet.
	/** Вызывается BtWorld при создании объекта. */
	void SetInternalObject(btRigidBody* rigidBody);
	/// Получить объект bullet.
	btRigidBody* GetInternalObject() const;

	// методы RigidBody
	vec3 GetPosition() const;
	mat3x3 GetOrientation() const;
	mat4x4 GetTransform() const;
	void ApplyImpulse(const vec3& impulse);

	// методы btMotionState
	void getWorldTransform(btTransform& transform) const;
	void setWorldTransform(const btTransform& transform);
};

END_INANITY_PHYSICS

#endif
