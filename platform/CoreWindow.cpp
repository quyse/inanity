#include "CoreWindow.hpp"
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

	virtual void SetWindow(WUC::CoreWindow^ newWindow)
	{
		window->window = newWindow;
		newWindow->Closed += ref new TypedEventHandler<WUC::CoreWindow^, CoreWindowEventArgs^>(this, &ViewProvider::OnWindowClosed);
	}

	virtual void Load(::Platform::String^ entryPoint)
	{
	}

	virtual void Run()
	{
		while(!window->closed)
		{
			WUC::CoreWindow::GetForCurrentThread()->Dispatcher->
				ProcessEvents(window->active ? CoreProcessEventsOption::ProcessAllIfPresent : CoreProcessEventsOption::ProcessUntilQuit);

			// if(window->active)
			// 	window->inputManager->Update();
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
		window->active = false;
		auto deferral = args->SuspendingOperation->GetDeferral();

		concurrency::create_task([this, deferral]()
		{
			// TODO: work on suspending
			// ...

			deferral->Complete();
		});
	}

	void OnResuming(::Platform::Object^ sender, ::Platform::Object^ args)
	{
		window->active = false;
	}

	void OnWindowClosed(WUC::CoreWindow^ sender, CoreWindowEventArgs^ args)
	{
		window->closed = true;
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

CoreWindow::CoreWindow(const String& title) : title(title), active(false), closed(false)
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
	closed = true;
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
