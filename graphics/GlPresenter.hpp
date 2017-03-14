#ifndef ___INANITY_GRAPHICS_GL_PRESENTER_HPP___
#define ___INANITY_GRAPHICS_GL_PRESENTER_HPP___

#include "Presenter.hpp"

BEGIN_INANITY_GRAPHICS

class GlDevice;
class GlFrameBuffer;

class GlPresenter : public Presenter
{
protected:
	ptr<GlDevice> device;
	ptr<GlFrameBuffer> frameBuffer;

	GlPresenter(ptr<GlDevice> device, ptr<GlFrameBuffer> frameBuffer);

public:
	// Presenter's methods.
	ptr<Device> GetDevice() const override;
	ptr<FrameBuffer> GetFrameBuffer() const override;
};

END_INANITY_GRAPHICS

#endif
