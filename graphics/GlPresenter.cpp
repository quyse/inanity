#include "GlPresenter.hpp"
#include "GlDevice.hpp"
#include "GlFrameBuffer.hpp"

BEGIN_INANITY_GRAPHICS

GlPresenter::GlPresenter(ptr<GlDevice> device, ptr<GlFrameBuffer> frameBuffer)
: device(device), frameBuffer(frameBuffer)
{}

ptr<Device> GlPresenter::GetDevice() const
{
	return device;
}

ptr<FrameBuffer> GlPresenter::GetFrameBuffer() const
{
	return frameBuffer;
}

END_INANITY_GRAPHICS
