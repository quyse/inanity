#include "File.hpp"
#include "scripting_impl.hpp"

SCRIPTABLE_MAP_BEGIN(File, Inanity.File);
	SCRIPTABLE_METHOD(File, GetSize);
SCRIPTABLE_MAP_END();
