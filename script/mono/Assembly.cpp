#include "Assembly.hpp"
#include "State.hpp"
#include "../Any.hpp"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

BEGIN_INANITY_MONO

Assembly::Assembly(ptr<State> state, MonoAssembly* assembly)
: state(state), assembly(assembly) {}

Assembly::~Assembly()
{
	mono_assembly_close(assembly);
}

ptr<Script::Any> Assembly::Run()
{
	// mono_jit_exec requires valid pointer even for zero args
	char* args[1] = { nullptr };

	int result = mono_jit_exec(state->GetDomain(), assembly, 0, args);

	return state->NewNumber(result);
}

END_INANITY_MONO
