#include "GlPresenter.hpp"
#include "GlDevice.hpp"
#include "GlFrameBuffer.hpp"
#include "GlSystem.hpp"
#include "RawTextureData.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

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

ptr<RawTextureData> GlPresenter::GetTextureData() const
{
	BEGIN_TRY();

	int width = GetWidth();
	int height = GetHeight();

	ptr<RawTextureData> data = NEW(RawTextureData(nullptr, PixelFormats::uintRGBA32, width, height, 0, 1, 0));

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ROW_LENGTH, data->GetMipWidth());
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glReadBuffer(GL_BACK);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data->GetMipData());
	GlSystem::CheckErrors("Can't read pixels");

	// mirror image vertically (because OpenGL writes image from bottom to top)
	uint8_t* lines = (uint8_t*)data->GetMipData();
	int pitch = data->GetMipLinePitch();
	for(int i = 0; i * 2 < height; ++i)
	{
		std::swap_ranges(lines + i * pitch, lines + (i + 1) * pitch, lines + (height - i - 1) * pitch);
	}

	return data;

	END_TRY("Failed to get OpenGL presenter texture data");
}

END_INANITY_GRAPHICS
