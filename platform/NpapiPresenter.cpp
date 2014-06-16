#include "NpapiPresenter.hpp"
#ifdef ___INANITY_PLATFORM_WINDOWS
#include "../graphics/Dx11Device.hpp"
#include "../graphics/Dx11RenderBuffer.hpp"
#include "../graphics/Dx11Texture.hpp"
#endif
#include "../graphics/GlSystem.hpp"
#include "../graphics/GlDevice.hpp"
#include "../graphics/RenderBuffer.hpp"
#include "../graphics/FrameBuffer.hpp"
#include "../graphics/MonitorMode.hpp"
#include "../MemoryFile.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_PLATFORM

using namespace Graphics;

NpapiPresenter::NpapiPresenter(ptr<Device> device) :
	device(device),
	renderBufferWidth(0), renderBufferHeight(0),
	width(0), height(0)
{
	BEGIN_TRY();

	frameBuffer = device->CreateFrameBuffer();

#if defined(___INANITY_PLATFORM_WINDOWS)

	// init image header
	ZeroMemory(&bfh, sizeof(bfh));
	bfh.biSize = sizeof(bfh);
	bfh.biWidth = 0;
	bfh.biHeight = 0;
	bfh.biBitCount = 32;
	bfh.biPlanes = 1;

#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD)

	// init image
	image.width = 0;
	image.height = 0;
	image.xoffset = 0;
	image.format = ZPixmap;
	image.data = nullptr;
	image.byte_order = LSBFirst;
	image.bitmap_unit = 32;
	image.bitmap_bit_order = LSBFirst;
	image.bitmap_pad = 32;
	image.depth = 24;
	image.bytes_per_line = 0;
	image.bits_per_pixel = 32;
	image.red_mask = 0xff;
	image.green_mask = 0xff00;
	image.blue_mask = 0xff0000;
	image.obdata = nullptr;

#endif

	END_TRY("Can't create NPAPI presenter");
}

NpapiPresenter::~NpapiPresenter() {}

ptr<Device> NpapiPresenter::GetDevice() const
{
	return device;
}

int NpapiPresenter::GetWidth() const
{
	return width;
}

int NpapiPresenter::GetHeight() const
{
	return height;
}

ptr<FrameBuffer> NpapiPresenter::GetFrameBuffer() const
{
	// check if we need to (re)create render buffer
	if(!renderBuffer || renderBufferWidth != width || renderBufferHeight != height)
	{
		// recreate render buffer

#ifdef ___INANITY_PLATFORM_WINDOWS
		// on Windows try to use effective DirectX 11 method
		ptr<Dx11Device> dx11Device = device.DynamicCast<Dx11Device>();
		if(dx11Device)
			renderBuffer = dx11Device->CreateRenderBufferGdiCompatible(width, height, SamplerSettings());
		else
#endif
			// else use generic render buffer
			renderBuffer = device->CreateRenderBuffer(width, height, PixelFormats::uintRGBA32, SamplerSettings());

		frameBuffer->SetColorBuffer(0, renderBuffer);

		// save new size
		renderBufferWidth = width;
		renderBufferHeight = height;
	}

	return frameBuffer;
}

void NpapiPresenter::SetMode(ptr<MonitorMode> mode)
{
	// fullscreen mode is not supported
}

void NpapiPresenter::Present()
{
	// need to use platform-dependent present methods
	THROW("NPAPI presenter doesn't support general present");
}

void NpapiPresenter::Resize(int width, int height)
{
	this->width = width;
	this->height = height;
}

#ifdef ___INANITY_PLATFORM_WINDOWS
void NpapiPresenter::PresentOnHdc(HDC hdc, int left, int top)
{
	// try to use effective DirectX 11 method
	ptr<Dx11Device> dx11Device = device.DynamicCast<Dx11Device>();
	if(dx11Device)
	{
		// get DC of render buffer
		ID3D11ShaderResourceView* resourceView = renderBuffer->GetTexture()
			.FastCast<Dx11Texture>()->GetShaderResourceViewInterface();

		ComPointer<ID3D11Resource> resource;
		resourceView->GetResource(&resource);

		ComPointer<IDXGISurface1> surface;
		if(FAILED(resource->QueryInterface(IID_IDXGISurface1, (void**)&surface)))
			THROW("Can't get IDXGISurface1 interface");

		// perform blit
		HDC dxHdc;
		if(FAILED(surface->GetDC(FALSE, &dxHdc)))
			THROW("Can't get render buffer's DC");

		BitBlt(hdc, left, top, width, height, dxHdc, 0, 0, SRCCOPY);

		RECT nullRect = { 0, 0, 0, 0 };
		if(FAILED(surface->ReleaseDC(&nullRect)))
			THROW("Can't release DirectX DC");
	}
	// if not DirectX 11, then it should be OpenGL
	else
	{
		ptr<GlDevice> glDevice = device.DynamicCast<GlDevice>();
		if(!glDevice)
			THROW("Unsupported graphics device");

		// if image size is wrong, resize image
		if(bfh.biWidth != width || bfh.biHeight != height)
		{
			bfh.biWidth = width;
			bfh.biHeight = height;
			imageFile = NEW(MemoryFile(width * 4 * height));
		}

		// get image from framebuffer
		void* imageData = imageFile->GetData();
		glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, imageData);
		GlSystem::CheckErrors();

		// render image onto surface
		if(StretchDIBits(hdc, left, top, width, height, 0, 0, width, height, imageData, (const BITMAPINFO*)&bfh, 0, SRCCOPY) == 0)
			THROW("Can't rasterize image into hdc");
	}
}
#endif

#if defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD)
void NpapiPresenter::PresentOnXGraphicsExposeEvent(const XGraphicsExposeEvent& e, int left, int top)
{
	// if image size is wrong, resize image
	if(image.width != width || image.height != height)
	{
		image.width = width;
		image.height = height;
		image.bytes_per_line = width * 4;
		// allocate memory for image + one additional line for reversing lines
		imageFile = NEW(MemoryFile(image.bytes_per_line * (height + 1)));
		image.data = (char*)imageFile->GetData();
		XInitImage(&image);
	}

	// get image from framebuffer
	glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, image.data);
	GlSystem::CheckErrors();

	// reverse order of lines
	void* buf = image.data + image.bytes_per_line * height;
	for(int i = 0; i * 2 < height; ++i)
	{
		char* b1 = image.data + i * image.bytes_per_line;
		char* b2 = image.data + (height - i - 1) * image.bytes_per_line;
		memcpy(buf, b1, image.bytes_per_line);
		memcpy(b1, b2, image.bytes_per_line);
		memcpy(b2, buf, image.bytes_per_line);
	}

	// render image into X
	XPutImage(
		e.display,
		e.drawable,
		DefaultGC(e.display, DefaultScreen(e.display)),
		&image,
		0, 0, // source xy
		left, top, // dest xy
		width, height // size
	);
}
#endif

END_INANITY_PLATFORM
