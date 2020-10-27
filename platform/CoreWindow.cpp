#include "CoreWindow.hpp"
#include "../graphics/Presenter.hpp"
#include "../graphics/RawTextureData.hpp"
#include "../input/CoreManager.hpp"
#include <ppltasks.h>

using Windows::ApplicationModel::Activation::IActivatedEventArgs;
using Windows::ApplicationModel::SuspendingEventArgs;
using Windows::ApplicationModel::Core::CoreApplication;
using Windows::ApplicationModel::Core::CoreApplicationView;
using Windows::UI::Core::CoreWindowEventArgs;
using Windows::ApplicationModel::Core::IFrameworkView;
using Windows::ApplicationModel::Core::IFrameworkViewSource;
using Windows::UI::Core::CoreProcessEventsOption;
using Windows::UI::Core::CoreWindow;
using Windows::UI::Core::WindowSizeChangedEventArgs;
using Windows::Foundation::EventHandler;
using Windows::Foundation::TypedEventHandler;
namespace WUC = Windows::UI::Core;

BEGIN_INANITY_PLATFORM

// helper classes

ref class CoreWindow::ViewProvider sealed : public IFrameworkView
{
private:
	ptr<CoreWindow> window;

internal:
	ViewProvider(ptr<CoreWindow> window) : window(window) {}

public:
	virtual void Initialize(CoreApplicationView^ applicationView)
	{
		applicationView->Activated += ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &ViewProvider::OnActivated);
		CoreApplication::Suspending += ref new EventHandler<SuspendingEventArgs^>(this, &ViewProvider::OnSuspending);
		CoreApplication::Resuming += ref new EventHandler<::Platform::Object^>(this, &ViewProvider::OnResuming);
	}

	virtual void Uninitialize()
	{
	}

	virtual void SetWindow(WUC::CoreWindow^ coreWindow)
	{
		window->window = coreWindow;
		coreWindow->SizeChanged += ref new TypedEventHandler<WUC::CoreWindow^, WindowSizeChangedEventArgs^>(this, &ViewProvider::OnWindowSizeChanged);
		coreWindow->Closed += ref new TypedEventHandler<WUC::CoreWindow^, CoreWindowEventArgs^>(this, &ViewProvider::OnWindowClosed);
	}

	virtual void Load(::Platform::String^ entryPoint)
	{
	}

	virtual void Run()
	{
		while(window->running)
		{
			WUC::CoreWindow::GetForCurrentThread()->Dispatcher->
				ProcessEvents(window->active ? CoreProcessEventsOption::ProcessAllIfPresent : CoreProcessEventsOption::ProcessUntilQuit);

			if(window->active)
			{
				if(window->inputManager)
					window->inputManager->Update();
			}
			window->activeHandler->Fire();
		}
	}

private:
	void OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
	{
		window->active = true;
		WUC::CoreWindow::GetForCurrentThread()->Activate();
	}

	void OnSuspending(::Platform::Object^ sender, SuspendingEventArgs^ args)
	{
		auto deferral = args->SuspendingOperation->GetDeferral();

		window->active = false;

		if(window->inputManager)
			window->inputManager->ReleaseButtonsOnUpdate();

		if(window->presenter)
			window->presenter->Suspend();

		concurrency::create_task([deferral]()
		{
			// TODO: other async stuff

			deferral->Complete();
		});
	}

	void OnResuming(::Platform::Object^ sender, ::Platform::Object^ args)
	{
		window->active = true;

		if(window->presenter)
			window->presenter->Resume();
	}

	void OnWindowSizeChanged(WUC::CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
	{
		if(window->presenter)
			window->presenter->Resize((int)args->Size.Width, (int)args->Size.Height);
	}

	void OnWindowClosed(WUC::CoreWindow^ sender, CoreWindowEventArgs^ args)
	{
		window->Stop();
	}
};

ref class CoreWindow::ViewProviderFactory sealed : public IFrameworkViewSource
{
private:
	ptr<CoreWindow> window;

internal:
	ViewProviderFactory(ptr<CoreWindow> window) : window(window) {}

public:
	virtual IFrameworkView^ CreateView()
	{
		return ref new ViewProvider(window);
	}
};


// CoreWindow class

CoreWindow::CoreWindow(const String& title) : title(title), active(false)
{
}

ptr<CoreWindow> CoreWindow::CreateForDirectX(const String& title)
{
	return NEW(CoreWindow(title));
}

IUnknown* CoreWindow::GetWindowUnknown() const
{
	return reinterpret_cast<IUnknown*>(window.Get());
}

void CoreWindow::SetInputManager(ptr<Input::CoreManager> inputManager)
{
	this->inputManager = inputManager;
}

void CoreWindow::SetTitle(const String& title)
{
}

void CoreWindow::Close()
{
	Stop();
}

void CoreWindow::SetFullScreen(bool fullscreen)
{
}

float CoreWindow::GetDPIScale() const
{
	return 1;
}

void CoreWindow::GetRect(int& left, int& top, int& width, int& height)
{
}

void CoreWindow::Run(ptr<Handler> activeHandler)
{
	running = true;
	this->activeHandler = activeHandler;
	CoreApplication::Run(ref new ViewProviderFactory(this));
}

void CoreWindow::PlaceCursor(int x, int y)
{
}

ptr<Window::Icon> CoreWindow::CreateIcon(ptr<Graphics::RawTextureData> texture)
{
	return nullptr;
}

void CoreWindow::SetIcon(ptr<Icon> icon)
{
}

ptr<Window::Cursor> CoreWindow::CreateCursor(ptr<Graphics::RawTextureData> texture, int hotX, int hotY)
{
	return nullptr;
}

void CoreWindow::SetCursor(ptr<Cursor> cursor)
{
}

void CoreWindow::UpdateMouseLock()
{
}

void CoreWindow::UpdateCursorVisible()
{
}

END_INANITY_PLATFORM
