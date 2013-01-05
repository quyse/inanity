#ifndef ___INANITY_PHYSICS_WORLD_HPP___
#define ___INANITY_PHYSICS_WORLD_HPP___

#include "physics.hpp"

BEGIN_INANITY_PHYSICS

class Shape;
class RigidBody;
class Character;

/// Абстрактный класс физического мира.
class World : public Object
{
public:
	/// Создать форму-коробку.
	virtual ptr<Shape> CreateBoxShape(const float3& halfSize) = 0;
	/// Создать форму-сферу.
	virtual ptr<Shape> CreateSphereShape(float radius) = 0;
	/// Создать форму-капсулу.
	/** Полная высота капсулы - radius * 2 + height */
	virtual ptr<Shape> CreateCapsuleShape(float radius, float height) = 0;

	/// Создать твёрдое тело.
	virtual ptr<RigidBody> CreateRigidBody(ptr<Shape> shape, float mass, const float4x4& startTransform) = 0;

	/// Создать персонажа.
	virtual ptr<Character> CreateCharacter(ptr<Shape> shape, const float4x4& startTransform) = 0;

	/// Выполнить шаг симуляции.
	virtual void Simulate(float time) = 0;
};

END_INANITY_PHYSICS

#endif
