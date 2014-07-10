#include "State.hpp"
#include "BaseAssemblyGenerator.hpp"
#include "MetaProvider.hpp"
#include "Assembly.hpp"
#include "../../platform/FileSystem.hpp"
#include "../../meta/ClassBase.hpp"
#include "../../Exception.hpp"
#include <iostream>

// TEST META
#include "impl.ipp"
#include "../../inanity-base-meta.ipp"
#include "../../inanity-data-meta.ipp"
// END TEST META

using namespace Inanity;
using namespace Inanity::Data;
using namespace Inanity::Script::Mono;

BEGIN_INANITY

class TestClass
{
public:
	static ptr<FileSystem> GetFileSystem()
	{
		return Platform::FileSystem::GetNativeFileSystem();
	}
};

END_INANITY

META_CLASS(TestClass, Inanity.TestClass);
	META_STATIC_METHOD(GetFileSystem);
META_CLASS_END();

void RegisterMeta(State* state)
{
	state->Register<TestClass>();
	state->Register<File>();
	state->Register<FileSystem>();
	state->Register<InputStream>();
	state->Register<OutputStream>();
	state->Register<Base64OutputStream>();
}

int main(int argc, char** argv)
{
	argc--;
	argv++;

	ptr<State> state;

	try
	{
		if(argc < 1)
			THROW("Not enough arguments");

		String command = argv[0];
		if(command == "generate")
		{
			if(argc < 2)
				THROW("Not enough arguments for generate");

			state = NEW(State(""));

			RegisterMeta(state);

			const char* libgenPath =
#ifdef _DEBUG
				"script/mono/generator/libgen/bin/Debug/libgen.dll"
#else
				"script/mono/generator/libgen/bin/Release/libgen.dll"
#endif
			;

			ptr<BaseAssemblyGenerator> generator = NEW(BaseAssemblyGenerator(state, libgenPath));

			state->RegisterClassesForGeneration(generator);

			std::cout << argv[1] << std::endl;
			generator->Generate(argv[1]);
		}
		else if(command == "run")
		{
			if(argc < 3)
				THROW("Not enough arguments for run");

			state = NEW(State(argv[1]));

			RegisterMeta(state);

			state->LoadAssembly(argv[2])->Run();
		}
		else
			THROW("Unknown command: " + command);

		return 0;
	}
	catch(Exception* exception)
	{
		MakePointer(exception)->PrintStack(std::cout);
		std::cout << "\n";

		return 1;
	}
}
