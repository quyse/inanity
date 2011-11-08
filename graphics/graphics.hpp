#ifndef ___INANITY_GRAPHICS_GRAPHICS_HPP___
#define ___INANITY_GRAPHICS_GRAPHICS_HPP___

/*
 * Общий файл для подсистемы графики.
 * Подсистема графики является сборной. Основная часть подсистемы - заменяемая,
 * подменяется конкретными подсистемами (пока только DX).
 * А некоторые компоненты являются общими для всех реализаций.
 */

#include "../Object.hpp"

#define BEGIN_INANITY_GRAPHICS BEGIN_INANITY namespace Graphics {
#define END_INANITY_GRAPHICS } END_INANITY
#ifdef INANITY_LIB
BEGIN_INANITY_GRAPHICS
END_INANITY_GRAPHICS
using namespace Graphics;
#endif

#endif
