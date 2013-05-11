#ifndef ___INANITY_PHYSICS_CHARACTER_HPP___
#define ___INANITY_PHYSICS_CHARACTER_HPP___

#include "physics.hpp"

BEGIN_INANITY_PHYSICS

class World;
class Shape;

/// Абстрактный класс физического персонажа.
/** Персонаж имеет форму, как и твёрдое тело, но управляется
программно. Он может ходить и прыгать. */
class Character : public Object
{
protected:
	ptr<World> world;
	ptr<Shape> shape;

protected:
	Character(ptr<World> world, ptr<Shape> shape);

public:
	/// Указать, куда персонаж идёт.
	/** Требуется указывать это каждый кадр симуляции.
	Персонаж идёт туда, взаимодействуя с физическим миром. */
	virtual void Walk(const vec3& movement) = 0;

	/// Получить трансформацию персонажа.
	virtual mat4x4 GetTransform() const = 0;
};

END_INANITY_PHYSICS

#endif
