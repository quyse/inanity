#ifndef ___INANITY_GRAPHICS_NX_PRESENTER_HPP___
#define ___INANITY_GRAPHICS_NX_PRESENTER_HPP___

#include "Presenter.hpp"
#include "../platform/platform.hpp"

BEGIN_INANITY_PLATFORM

class NxWindow;

END_INANITY_PLATFORM

BEGIN_INANITY_GRAPHICS

class GlDevice;

/// Presenter class for Nintendo Switch graphics.
class NxPresenter : public Presenter
{
private:
	ptr<GlDevice> device;
	ptr<Platform::NxWindow> window;

public:
	NxPresenter(ptr<GlDevice> device, ptr<Platform::NxWindow> window);
};

END_INANITY_GRAPHICS

#endif
