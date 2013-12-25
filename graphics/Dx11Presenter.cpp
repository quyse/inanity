#include "Dx11Presenter.hpp"
#include "Dx11Device.hpp"
#include "Dx11FrameBuffer.hpp"

BEGIN_INANITY_GRAPHICS

Dx11Presenter::Dx11Presenter(ptr<Dx11Device> device)
: device(device)
{
	frameBuffer = NEW(Dx11FrameBuffer(device, this));
}

ptr<Device> Dx11Presenter::GetDevice() const
{
	return device;
}

ptr<FrameBuffer> Dx11Presenter::GetFrameBuffer() const
{
	return frameBuffer;
}

END_INANITY_GRAPHICS
