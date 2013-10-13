#ifndef ___INANITY_GRAPHICS_EMS_PRESENTER_HPP___
#define ___INANITY_GRAPHICS_EMS_PRESENTER_HPP___

#include "Presenter.hpp"

BEGIN_INANITY_GRAPHICS

class GlDevice;
class GlFrameBuffer;
class EmsOutput;

/// Presenter class for Emscripten graphics.
class EmsPresenter : public Presenter
{
private:
	ptr<GlDevice> device;
	ptr<GlFrameBuffer> frameBuffer;
	ptr<EmsOutput> output;

public:
	EmsPresenter(ptr<GlDevice> device, ptr<GlFrameBuffer> frameBuffer, ptr<EmsOutput> output);
	~EmsPresenter();

	//*** Presenter's methods.
	ptr<Device> GetDevice() const;
	ptr<FrameBuffer> GetFrameBuffer() const;
	void SetMode(ptr<MonitorMode> mode);
	void Present();
	void Resize(int width, int height);
};

END_INANITY_GRAPHICS

#endif
