#ifndef ___INANITY_GRAPHICS_GRAPHICS_HPP___
#define ___INANITY_GRAPHICS_GRAPHICS_HPP___

/*
 * Общий файл для подсистемы графики.
 * Подсистема графики является сборной. Основная часть подсистемы - заменяемая,
 * подменяется конкретными подсистемами (пока только DX).
 * А некоторые компоненты являются общими для всех реализаций.
 */

#include "../Object.hpp"
// Решено просто включать математику во всю графику.
#include "../inanity-math.hpp"

#define BEGIN_INANITY_GRAPHICS BEGIN_INANITY namespace Graphics {
#define END_INANITY_GRAPHICS } END_INANITY

BEGIN_INANITY_GRAPHICS
using namespace Inanity::Math;
END_INANITY_GRAPHICS

#endif
