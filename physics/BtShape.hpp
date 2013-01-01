#ifndef ___INANITY_PHYSICS_BT_SHAPE_HPP___
#define ___INANITY_PHYSICS_BT_SHAPE_HPP___

#include "Shape.hpp"
#include "bt.hpp"

BEGIN_INANITY_PHYSICS

class BtWorld;

/// Класс формы для Bullet-мира.
class BtShape : public Shape
{
private:
	btCollisionShape* collisionShape;

public:
	BtShape(ptr<BtWorld> world, btCollisionShape* collisionShape);
	~BtShape();

	btCollisionShape* GetInternalObject() const;
};

END_INANITY_PHYSICS

#endif
