#ifndef ___INANITY_PHYSICS_WORLD_HPP___
#define ___INANITY_PHYSICS_WORLD_HPP___

#include "physics.hpp"
#include <vector>

BEGIN_INANITY_PHYSICS

class Shape;
class RigidBody;
class Character;

/// Абстрактный класс физического мира.
class World : public Object
{
public:
	/// Создать форму-коробку.
	virtual ptr<Shape> CreateBoxShape(const vec3& halfSize) = 0;
	/// Создать форму-сферу.
	virtual ptr<Shape> CreateSphereShape(float radius) = 0;
	/// Создать форму-капсулу.
	/** Полная высота капсулы - radius * 2 + height */
	virtual ptr<Shape> CreateCapsuleShape(float radius, float height) = 0;
	/// Create compound shape from array of shapes.
	virtual ptr<Shape> CreateCompoundShape(const std::vector<std::pair<mat4x4, ptr<Shape> > >& shapes) = 0;

	/// Создать твёрдое тело.
	virtual ptr<RigidBody> CreateRigidBody(ptr<Shape> shape, float mass, const mat4x4& startTransform) = 0;

	/// Создать персонажа.
	virtual ptr<Character> CreateCharacter(ptr<Shape> shape, const mat4x4& startTransform) = 0;

	/// Выполнить шаг симуляции.
	virtual void Simulate(float time) = 0;
};

END_INANITY_PHYSICS

#endif
