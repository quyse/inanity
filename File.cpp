#include "File.hpp"
#include "scripting_impl.hpp"

BEGIN_INANITY

SCRIPTABLE_MAP_BEGIN(File, Inanity.File);
	SCRIPTABLE_METHOD(File, GetSize);
SCRIPTABLE_MAP_END();

END_INANITY
