#ifndef ___INANITY_PHYSICS_RIGID_BODY_HPP___
#define ___INANITY_PHYSICS_RIGID_BODY_HPP___

#include "physics.hpp"

BEGIN_INANITY_PHYSICS

class World;
class Shape;

/// Абстрактный класс твёрдого тела.
class RigidBody : public Object
{
protected:
	/// Физический мир, в который добавлено тело.
	ptr<World> world;
	/// Форма твёрдого тела.
	ptr<Shape> shape;

protected:
	RigidBody(ptr<World> world, ptr<Shape> shape);

public:
	ptr<World> GetWorld() const;
	ptr<Shape> GetShape() const;

	/// Получить текущее положение твёрдого тела.
	virtual float3 GetPosition() const = 0;
	/// Получить текущую ориентацию твёрдого тела.
	virtual float3x3 GetOrientation() const = 0;
	/// Получить текущую трансформацию твёрдого тела.
	virtual float4x4 GetTransform() const = 0;

	/// Применить импульс к телу.
	virtual void ApplyImpulse(const float3& impulse) = 0;
};

END_INANITY_PHYSICS

#endif
