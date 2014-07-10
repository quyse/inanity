#include "Assembly.hpp"
#include "State.hpp"
#include "../Any.hpp"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

BEGIN_INANITY_MONO

Assembly::Assembly(ptr<State> state, MonoAssembly* assembly, const String& assemblyFileName)
: state(state), assembly(assembly), assemblyFileName(assemblyFileName) {}

Assembly::~Assembly()
{
	mono_assembly_close(assembly);
}

ptr<Script::Any> Assembly::Run()
{
	char* args[1] = { (char*)assemblyFileName.c_str() };

	int result = mono_jit_exec(state->GetDomain(), assembly, sizeof(args) / sizeof(args[0]), args);

	return state->NewNumber(result);
}

END_INANITY_MONO
