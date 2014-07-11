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
	// We cannot unload assembly from appdomain,
	// mono_assembly_close() crashes the program.
	// Possible way is to load assembly into separate appdomain
	// - than we can unload the whole appdomain.
	// Example technique:
	// http://www.west-wind.com/presentations/dynamicCode/DynamicCode.htm
}

ptr<Script::Any> Assembly::Run()
{
	char* args[1] = { (char*)assemblyFileName.c_str() };

	int result = mono_jit_exec(state->GetDomain(), assembly, sizeof(args) / sizeof(args[0]), args);

	return state->NewNumber(result);
}

END_INANITY_MONO
