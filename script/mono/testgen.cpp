#include "State.hpp"
#include "BaseAssemblyGenerator.hpp"
#include "MetaProvider.hpp"
#include "../../meta/ClassBase.hpp"
#include "../../Exception.hpp"
#include <iostream>

// TEST META
#include "impl.ipp"
#include "../../inanity-base-meta.ipp"
// END TEST META

using namespace Inanity;
using namespace Inanity::Script::Mono;

int main()
{
	ptr<State> state;
	try
	{
		state = NEW(State(""));

		const char* libgenPath =
#ifdef _DEBUG
			"script/mono/generator/libgen/bin/Debug/libgen.dll"
#else
			"script/mono/generator/libgen/bin/Release/libgen.dll"
#endif
			;

		ptr<BaseAssemblyGenerator> generator = NEW(BaseAssemblyGenerator(state, libgenPath));

		generator->Register(Meta::MetaOf<MetaProvider, File>());
		generator->Register(Meta::MetaOf<MetaProvider, InputStream>());

		generator->Generate("inanity.dll");
	}
	catch(Exception* exception)
	{
		MakePointer(exception)->PrintStack(std::cout);
		std::cout << "\n";
	}

	return 0;
}
