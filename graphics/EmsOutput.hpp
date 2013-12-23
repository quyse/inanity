#ifndef ___INANITY_GRAPHICS_EMS_OUTPUT_HPP___
#define ___INANITY_GRAPHICS_EMS_OUTPUT_HPP___

#include "Output.hpp"

#ifndef ___INANITY_PLATFORM_EMSCRIPTEN
#error EmsOutput is for Emscripten platform
#endif

BEGIN_INANITY_GRAPHICS

/// Emscripten graphics output class.
class EmsOutput : public Output
{
public:
	EmsOutput();

	//*** Output's methods.
	int GetWidth() const;
	int GetHeight() const;
	void Resize(int width, int height);
};

END_INANITY_GRAPHICS

#endif
