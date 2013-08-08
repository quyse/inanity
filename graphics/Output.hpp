#ifndef ___INANITY_GRAPHICS_OUTPUT_HPP___
#define ___INANITY_GRAPHICS_OUTPUT_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

class Presenter;

/// Place to graphics output.
/** Subclasses of this class represent screen, window or part of window
on concrete platformes. */
class Output : public Object
{
protected:
	Presenter* presenter;

public:
	Output();

	/// Register graphics presenter to receive notifications about size.
	void SetPresenter(Presenter* presenter);

	/// Get width of output in pixels.
	virtual int GetWidth() const = 0;
	/// Get height of output in pixels.
	virtual int GetHeight() const = 0;
};

END_INANITY_GRAPHICS

#endif
