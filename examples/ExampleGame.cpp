#include "ExampleGame.hpp"

const int ExampleGame::screenWidth = 800;
const int ExampleGame::screenHeight = 600;
const bool ExampleGame::fullscreen = false;
const char* ExampleGame::shadersCacheFileName = "shaders";

void ExampleGame::Init()
{
	// by default no additional initialization
}

void ExampleGame::Run()
{
	BEGIN_TRY();

	//** Initialization

	{
		// create a default graphics system object
		ptr<Inanity::Graphics::System> system = Inanity::Platform::Game::CreateDefaultGraphicsSystem();
		// get a first graphics adapter in system
		ptr<Inanity::Graphics::Adapter> adapter = system->GetAdapters()[0];
		// create graphics device object for this adapter
		device = system->CreateDevice(adapter);
		// get a first monitor of the adapter
		ptr<Inanity::Graphics::Monitor> monitor = adapter->GetMonitors()[0];

		// create default game window
		window = monitor->CreateDefaultWindow("Inanity Example", screenWidth, screenHeight);

		// if fullscreen, try to get appropriate monitor mode
		ptr<Inanity::Graphics::MonitorMode> monitorMode;
		if(fullscreen)
			// this method returns 0 if mode couldn't be created, and doesn't throw exception
			monitorMode = monitor->TryCreateMode(screenWidth, screenHeight);
		// create graphics presenter for a window
		presenter = device->CreatePresenter(window->CreateOutput(), monitorMode);

		// create shader cache
		shaderCache = NEW(Inanity::Graphics::ShaderCache(
			// filesystem to load and store compiled shaders
			NEW(Inanity::SQLiteFileSystem(shadersCacheFileName)),
			// graphics device object
			device,
			// shader compiler to compile binary shaders from source
			system->CreateShaderCompiler(),
			// shader generator to translate C++ shaders into text source
			system->CreateShaderGenerator(),
			// hash algorithm used to identify binary blobs
			NEW(Inanity::Crypto::WhirlpoolStream())
		));

		// create device context
		context = system->CreateContext(device);
	}

	// create default input manager for the window
	inputManager = Inanity::Platform::Game::CreateInputManager(window);

	// additional initialization
	Init();

	//** Running game
	window->Run(Inanity::Handler::Bind(MakePointer(this), &ExampleGame::Do));

	END_TRY("Can't run game");
}

void ExampleGame::Do()
{
	ProcessInput(inputManager->GetCurrentFrame());

	Tick(ticker.Tick());

	presenter->Present();
}
