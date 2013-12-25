#ifndef ___INANITY_GRAPHICS_DX11_PRESENTER_HPP___
#define ___INANITY_GRAPHICS_DX11_PRESENTER_HPP___

#include "Presenter.hpp"

BEGIN_INANITY_GRAPHICS

class Dx11Device;
class Dx11FrameBuffer;
class Dx11RenderBuffer;

class Dx11Presenter : public Presenter
{
protected:
	ptr<Dx11Device> device;
	ptr<Dx11FrameBuffer> frameBuffer;

public:
	Dx11Presenter(ptr<Dx11Device> device);

	virtual ptr<Dx11RenderBuffer> GetBackBuffer() = 0;

	//*** Presenter's methods.
	ptr<Device> GetDevice() const;
	ptr<FrameBuffer> GetFrameBuffer() const;
};

END_INANITY_GRAPHICS

#endif
