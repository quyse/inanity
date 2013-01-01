#ifndef ___INANITY_PHYSICS_BT_HPP___
#define ___INANITY_PHYSICS_BT_HPP___

/* Файл для поддержки библиотеки Bullet. */

#include "../deps/bullet/src/btBulletCollisionCommon.h"
#include "../deps/bullet/src/btBulletDynamicsCommon.h"

BEGIN_INANITY_PHYSICS

//******* функции преобразования для типов bullet

inline btVector3 toBt(const float3& a)
{
	return btVector3(a.x, a.y, a.z);
}
inline btMatrix3x3 toBt(const float3x3& a)
{
	return btMatrix3x3(
		a.t[0][0], a.t[0][1], a.t[0][2],
		a.t[1][0], a.t[1][1], a.t[1][2],
		a.t[2][0], a.t[2][1], a.t[2][2]);
}
inline btTransform toBt(const float4x4& a)
{
	return btTransform(toBt(submatrix<3, 3>(a)), btVector3(a.t[3][0], a.t[3][1], a.t[3][2]));
}

inline float3 fromBt(const btVector3& a)
{
	return float3(a.x(), a.y(), a.z());
}
inline float3x3 fromBt(const btMatrix3x3& a)
{
	float3x3 r;
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
			r.t[i][j] = a[i][j];
	return r;
}
inline float4x4 fromBt(const btTransform& a)
{
	float4x4 r;
	const btMatrix3x3& basis = a.getBasis();
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
			r.t[i][j] = basis[i][j];
		r.t[i][3] = 0;
	}
	const btVector3& origin = a.getOrigin();
	r.t[3][0] = origin.x();
	r.t[3][1] = origin.y();
	r.t[3][2] = origin.z();
	r.t[3][3] = 1;
	return r;
}

END_INANITY_PHYSICS

#endif
