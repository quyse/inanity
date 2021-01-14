#include "CoreWindow.hpp"
#include "../graphics/Presenter.hpp"
#include "../graphics/RawTextureData.hpp"
#include "../input/CoreManager.hpp"
#include <ppltasks.h>

#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Core.h>

using namespace winrt;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel;
using namespace Windows::Foundation;
using Windows::UI::Core::CoreProcessEventsOption;
using Windows::UI::Core::CoreWindow;
using Windows::UI::Core::CoreWindowEventArgs;
using Windows::UI::Core::WindowSizeChangedEventArgs;
namespace WUC = Windows::UI::Core;

BEGIN_INANITY_PLATFORM

// helper classes

class CoreWindow::ViewProvider : public implements<CoreWindow::ViewProvider, IFrameworkView>
{
private:
	ptr<CoreWindow> window;

public:
	ViewProvider(ptr<CoreWindow> window) : window(window) {}

	void Initialize(CoreApplicationView applicationView)
	{
		applicationView.Activated({ this, &ViewProvider::OnActivated });
		CoreApplication::Suspending({ this, &ViewProvider::OnSuspending });
		CoreApplication::Resuming({ this, &ViewProvider::OnResuming });
	}

	void Uninitialize()
	{
	}

	void SetWindow(WUC::CoreWindow coreWindow)
	{
		window->window = coreWindow;
		coreWindow.SizeChanged({ this, &ViewProvider::OnWindowSizeChanged });
		coreWindow.Closed({ this, &ViewProvider::OnWindowClosed });
	}

	void Load(hstring entryPoint)
	{
	}

	void Run()
	{
		while(window->running)
		{
			WUC::CoreWindow::GetForCurrentThread().Dispatcher()
				.ProcessEvents(window->active ? CoreProcessEventsOption::ProcessAllIfPresent : CoreProcessEventsOption::ProcessUntilQuit);

			if(window->active)
			{
				if(window->inputManager)
					window->inputManager->Update();
			}
			window->activeHandler->Fire();
		}
	}

private:
	void OnActivated(CoreApplicationView applicationView, IActivatedEventArgs args)
	{
		window->active = true;
		WUC::CoreWindow::GetForCurrentThread().Activate();
	}

	void OnSuspending(IInspectable sender, SuspendingEventArgs args)
	{
		SuspendingDeferral deferral = args.SuspendingOperation().GetDeferral();

		window->active = false;

		if(window->inputManager)
			window->inputManager->ReleaseButtonsOnUpdate();

		if(window->presenter)
			window->presenter->Suspend();

		concurrency::create_task([deferral]()
		{
			// TODO: other async stuff

			deferral.Complete();
		});
	}

	void OnResuming(IInspectable sender, IInspectable args)
	{
		window->active = true;

		if(window->presenter)
			window->presenter->Resume();
	}

	void OnWindowSizeChanged(WUC::CoreWindow sender, WindowSizeChangedEventArgs args)
	{
		if(window->presenter)
			window->presenter->Resize((int)args.Size().Width, (int)args.Size().Height);
	}

	void OnWindowClosed(WUC::CoreWindow sender, CoreWindowEventArgs args)
	{
		window->Stop();
	}
};

class CoreWindow::ViewProviderFactory : public implements<CoreWindow::ViewProviderFactory, IFrameworkViewSource>
{
private:
	ptr<CoreWindow> window;

public:
	ViewProviderFactory(ptr<CoreWindow> window) : window(window) {}

	IFrameworkView CreateView()
	{
		return winrt::make<ViewProvider>(window);
	}
};


// CoreWindow class

CoreWindow::CoreWindow(const String& title) : window(nullptr), title(title), active(false)
{
}

ptr<CoreWindow> CoreWindow::CreateForDirectX(const String& title)
{
	return NEW(CoreWindow(title));
}

winrt::Windows::Foundation::IUnknown CoreWindow::GetWindowUnknown() const
{
	return window;
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
	CoreApplication::Run(winrt::make<ViewProviderFactory>(this));
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
