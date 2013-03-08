#ifndef ___INANITY_GRAPHICS_OUTPUT_HPP___
#define ___INANITY_GRAPHICS_OUTPUT_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

/// Абстракнтый класс, представляющий область графического вывода.
/** Подклассы зависят от платформы, и представляют, например, окно
в Windows или оконном менеджере Linux, или полный экран, или canvas
в NativeClient-браузере, или ещё что-то. */
class Output : public Object
{
public:
};

END_INANITY_GRAPHICS

#endif
