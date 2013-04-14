#ifndef ___INANITY_PHYSICS_PHYSICS_HPP___
#define ___INANITY_PHYSICS_PHYSICS_HPP___

/* Общий файл для подсистемы физики.
По идее, физика может работать через различные бэкэнды.
Сейчас есть только подсистема Bullet.
*/

#include "../Object.hpp"
// Решено просто включать математику во всю физику.
#include "../inanity-math.hpp"

#define BEGIN_INANITY_PHYSICS BEGIN_INANITY namespace Physics {
#define END_INANITY_PHYSICS } END_INANITY

BEGIN_INANITY_PHYSICS
using namespace Inanity::Math;
END_INANITY_PHYSICS

#endif
