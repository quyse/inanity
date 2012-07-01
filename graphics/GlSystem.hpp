#ifndef ___INANITY_GRAPHICS_GL_SYSTEM_HPP___
#define ___INANITY_GRAPHICS_GL_SYSTEM_HPP___

#include "System.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс графической системы OpenGL.
class GlSystem : public System
{
public:
	// методы System
	ptr<Window> CreateWindow();
	ptr<Device> CreatePrimaryDevice();
};

END_INANITY_GRAPHICS

#endif
