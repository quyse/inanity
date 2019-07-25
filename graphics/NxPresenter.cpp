#include "NxPresenter.hpp"
#include "GlDevice.hpp"
#include "../platform/NxWindow.hpp"

BEGIN_INANITY_GRAPHICS

NxPresenter::NxPresenter(ptr<GlDevice> device, ptr<Platform::NxWindow> window)
: device(device), window(window)
{
}

END_INANITY_GRAPHICS
