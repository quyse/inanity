#include "ExampleGame.hpp"
#include <iostream>

struct ShadersData
{
	struct Vertex
	{
		vec3 position;
		vec4 color;
		vec2 texcoord;
	};

	ptr<Inanity::Graphics::VertexLayout> vl;
	ptr<Inanity::Graphics::AttributeLayout> al;
	ptr<Inanity::Graphics::AttributeLayoutSlot> als;

	Value<vec3> aPosition;
	Value<vec4> aColor;
	Value<vec2> aTexcoord;
	Interpolant<vec4> iColor;
	Interpolant<vec2> iTexcoord;
	Fragment<vec4> fTarget;

	ShadersData() :
		vl(NEW(Inanity::Graphics::VertexLayout(sizeof(Vertex)))),
		al(NEW(Inanity::Graphics::AttributeLayout())),
		als(al->AddSlot()),
		aPosition(al->AddElement(als, vl->AddElement(&Vertex::position))),
		aColor(al->AddElement(als, vl->AddElement(&Vertex::color))),
		aTexcoord(al->AddElement(als, vl->AddElement(&Vertex::texcoord))),
		iColor(0),
		iTexcoord(1),
		fTarget(0)
	{}
};

class Game : public ExampleGame
{
private:
	Inanity::Graphics::ContextState cs;

	ShadersData shadersData;

public:
	void ProcessInput(ptr<Inanity::Input::Frame> inputFrame)
	{
		while(inputFrame->NextEvent())
			ProcessInputEvent(inputFrame->GetCurrentEvent());
	}

	void ProcessInputEvent(const Inanity::Input::Event& inputEvent)
	{
		switch(inputEvent.device)
		{
		case Inanity::Input::Event::deviceKeyboard:
			switch(inputEvent.keyboard.key)
			{
			case Inanity::Input::Keys::Escape:
				window->Close();
				return;
			}
			break;
		case Inanity::Input::Event::deviceMouse:
			break;
		}
	}

	void Init()
	{
		cs.renderBuffers[0] = presenter->GetBackBuffer();

		cs.viewportWidth = screenWidth;
		cs.viewportHeight = screenHeight;

		// make shaders
		{
			using namespace Inanity::Graphics::Shaders;

			ShadersData& s = shadersData;

			cs.vertexShader = shaderCache->GetVertexShader((
				setPosition(newvec4(s.aPosition, 1.0f)),
				s.iTexcoord = s.aTexcoord,
				s.iColor = s.aColor
			));

			cs.pixelShader = shaderCache->GetPixelShader((
				s.iColor,
				s.iTexcoord,
				s.fTarget = s.iColor
			));

			cs.cullMode = Inanity::Graphics::ContextState::cullModeNone;
		}

		cs.attributeBinding = device->CreateAttributeBinding(shadersData.al);

		// make geometry
		{
			ShadersData::Vertex vertices[4] =
			{
				{ vec3(-0.9f, 0.9f, -0.5f), vec4(1, 0, 0, 1), vec2(0.0f, 0.0f) },
				{ vec3(-0.1f, 0.9f, -0.5f), vec4(0, 1, 0, 1), vec2(1.0f, 0.0f) },
				{ vec3(-0.1f, 0.1f, -0.5f), vec4(0, 0, 1, 0), vec2(1.0f, 1.0f) },
				{ vec3(-0.9f, 0.1f, -0.5f), vec4(1, 1, 1, 1), vec2(0.0f, 1.0f) }
			};
			cs.vertexBuffers[0] = device->CreateStaticVertexBuffer(
				Inanity::MemoryFile::CreateViaCopy(vertices, sizeof(vertices)),
				shadersData.vl
			);

			short indices[6] =
			{
				0, 1, 2, 0, 2, 3
			};
			cs.indexBuffer = device->CreateStaticIndexBuffer(
				Inanity::MemoryFile::CreateViaCopy(indices, sizeof(indices)),
				sizeof(indices[0])
			);
		}
	}

	void Tick(float tick)
	{
		context->Reset();

		float color[4] = { 0, 1, 1, 0 };
		context->ClearRenderBuffer(cs.renderBuffers[0], color);

		context->GetTargetState() = cs;
		context->Draw();
	}
};

int main()
{
	try
	{
		ptr<Game> game = NEW(Game());
		game->Run();

		printf("Game::Run exited\n");
	}
	catch(Inanity::Exception* exception)
	{
		MakePointer(exception)->PrintStack(std::cout);
	}

	printf("Main try exited\n");

	return 0;
}
