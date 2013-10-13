#include "EmsPresenter.hpp"
#include "EmsOutput.hpp"
#include "GlDevice.hpp"
#include "GlFrameBuffer.hpp"

BEGIN_INANITY_GRAPHICS

EmsPresenter::EmsPresenter(ptr<GlDevice> device, ptr<GlFrameBuffer> frameBuffer, ptr<EmsOutput> output)
: device(device), frameBuffer(frameBuffer), output(output) {}

EmsPresenter::~EmsPresenter() {}

ptr<Device> EmsPresenter::GetDevice() const
{
	return device;
}

ptr<FrameBuffer> EmsPresenter::GetFrameBuffer() const
{
	return frameBuffer;
}

void EmsPresenter::SetMode(ptr<MonitorMode> mode)
{
}

void EmsPresenter::Present()
{
}

void EmsPresenter::Resize(int width, int height)
{
}

END_INANITY_GRAPHICS
