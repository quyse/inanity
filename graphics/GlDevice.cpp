#include "GlDevice.hpp"

#ifdef ___INANITY_WINDOWS
GlDevice::GlDevice(ptr<GlSystem> system, const String& deviceName, ptr<GlContext> context)
: system(system), deviceName(deviceName), context(context)
{
}
#endif

ptr<System> GlDevice::GetSystem() const
{
	return system;
}

ptr<Presenter> GlDevice::CreatePresenter(ptr<Output> abstractOutput, const PresentMode& mode)
{
	try
	{
#ifdef ___INANITY_WINDOWS
		// область вывода - только Win32
		ptr<Win32Output> output = abstractOutput.DynamicCast<Win32Output>();
		if(!output)
			THROW_PRIMARY_EXCEPTION("Only Win32 output allowed");

		// если режим полноэкранный, переключить его
		if(mode.fullscreen)
		{
			// заполнить структуру режима экрана
			DEVMODE devMode;
			ZeroMemory(&devMode, sizeof(devMode));
			devMode.dmSize = sizeof(devMode);
			devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
			devMode.dmBitsPerPel = 32;
			devMode.dmPelsWidth = mode.width;
			devMode.dmPelsHeight = mode.height;
			// сменить режим экрана
			if(ChangeDisplaySettingsEx(deviceName.c_str(), &devMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
				THROW_PRIMARY_EXCEPTION("Can't change display settings");
		}

		// получить дескриптор окна
		HDC hdc = GetDC(output->GetWindowHandle());
		if(!hdc)
			THROW_PRIMARY_EXCEPTION("Can't get hdc");

		// подобрать формат пикселов
		PIXELFORMATDESCRIPTOR pfd;
		ZeroMemory(&pfd, sizeof(pfd));
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 24;
		pfd.cDepthBits = 0; // не используем глубину для первичного буфера
		pfd.iLayerType = PFD_MAIN_PLANE;
		int pixelFormat = ChoosePixelFormat(hdc, &pfd);
		if(!pixelFormat)
			THROW_PRIMARY_EXCEPTION("Can't choose pixel format");
		// и установить этот формат
		if(!SetPixelFormat(hdc, pixelFormat, &pfd))
			THROW_PRIMARY_EXCEPTION("Can't set pixel format");

		// создать контекст
		HGLRC hglrc = wglCreateContext(hdc);
		if(!hglrc)
			THROW_PRIMARY_EXCEPTION("Can't create OpenGL window context");

		// сделать его текущим
		if(!wglMakeCurrent(hdc, hglrc))
			THROW_SECONDARY_EXCEPTION("Can't make OpenGL context current", Exception::SystemError());

#endif
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create presenter for GL device", exception);
	}
}
