#ifndef ___INANITY_SCRIPT_MONO_ASSEMBLY_HPP___
#define ___INANITY_SCRIPT_MONO_ASSEMBLY_HPP___

#include "mono.hpp"
#include "../Function.hpp"
#include <mono/metadata/image.h>

BEGIN_INANITY_MONO

class State;

/// Class representing .NET assembly.
/** It's a function in terms of general scripting.
Running means run a Main() static method.
Return value is an exit code. */
class Assembly : public Script::Function
{
private:
	ptr<State> state;
	MonoAssembly* assembly;

public:
	Assembly(ptr<State> state, MonoAssembly* assembly);
	~Assembly();

	//*** Script::Function's methods.
	ptr<Script::Any> Run();
};

END_INANITY_MONO

#endif
