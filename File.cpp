#include "File.hpp"

#ifdef ___INANITY_SCRIPTING

void File::Script_getSize(ScriptCall& call)
{
	call.EnsureArgumentsCount(0);
	call.Return((double)GetSize());
}

SCRIPTABLE_MAP_BEGIN(File);
	SCRIPTABLE_METHOD(File, getSize);
SCRIPTABLE_MAP_END();

#endif
