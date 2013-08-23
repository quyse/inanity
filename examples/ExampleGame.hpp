#ifndef ___EXAMPLE_GAME_HPP___
#define ___EXAMPLE_GAME_HPP___

#include "general.hpp"

/// Skeleton game class for examples.
class ExampleGame : public Inanity::Object
{
protected:
	// Screen mode.
	static const int screenWidth;
	static const int screenHeight;
	static const bool fullscreen;

	/// Name of shaders cache SQLite database file.
	static const char* shadersCacheFileName;

	ptr<Inanity::Graphics::Device> device;
	ptr<Inanity::Graphics::Presenter> presenter;
	ptr<Inanity::Graphics::Context> context;
	ptr<Inanity::Platform::Window> window;
	ptr<Inanity::Input::Manager> inputManager;
	ptr<Inanity::Graphics::ShaderCache> shaderCache;

	Inanity::Ticker ticker;

	void Do();

	// Methods to implement.
	virtual void Init();
	virtual void ProcessInput(ptr<Inanity::Input::Frame> inputFrame) = 0;
	virtual void Tick(float tick) = 0;

public:
	void Run();
};

#endif
