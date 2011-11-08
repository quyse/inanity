#ifndef ___INANITY_GRAPHICS_SYSTEM_HPP___
#define ___INANITY_GRAPHICS_SYSTEM_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

/// Центральный класс графической подсистемы.
class System : public Object
{
public:
	void Resize(size_t width, size_t height);
};

END_INANITY_GRAPHICS

#endif
