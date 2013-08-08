#ifndef ___INANITY_PHYSICS_BT_HPP___
#define ___INANITY_PHYSICS_BT_HPP___

/* Файл для поддержки библиотеки Bullet. */

#include "../deps/bullet/src/btBulletCollisionCommon.h"
#include "../deps/bullet/src/btBulletDynamicsCommon.h"
#include "../deps/bullet/src/BulletCollision/CollisionDispatch/btGhostObject.h"
#include "../deps/bullet/src/BulletDynamics/Character/btKinematicCharacterController.h"

BEGIN_INANITY_PHYSICS

//******* функции преобразования для типов bullet

inline btVector3 toBt(const vec3& a)
{
	return btVector3(a.x, a.y, a.z);
}
inline btMatrix3x3 toBt(const mat3x3& a)
{
	return btMatrix3x3(
		a(0, 0), a(1, 0), a(2, 0),
		a(0, 1), a(1, 1), a(2, 1),
		a(0, 2), a(1, 2), a(2, 2));
}
inline btTransform toBt(const mat4x4& a)
{
	return btTransform(toBt(submat<float, 3, 3>(a)), btVector3(a(0, 3), a(1, 3), a(2, 3)));
}

inline vec3 fromBt(const btVector3& a)
{
	return vec3(a.x(), a.y(), a.z());
}
inline mat3x3 fromBt(const btMatrix3x3& a)
{
	mat3x3 r;
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
			r(j, i) = a[i][j];
	return r;
}
inline mat4x4 fromBt(const btTransform& a)
{
	mat4x4 r;
	const btMatrix3x3& basis = a.getBasis();
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
			r(j, i) = basis[i][j];
		r(3, i) = 0;
	}
	const btVector3& origin = a.getOrigin();
	r(0, 3) = origin.x();
	r(1, 3) = origin.y();
	r(2, 3) = origin.z();
	r(3, 3) = 1;
	return r;
}

END_INANITY_PHYSICS

#endif
