#ifndef ___INANITY_SCRIPTING_IMPL_HPP___
#define ___INANITY_SCRIPTING_IMPL_HPP___

/* Файл для включения в файлы реализации, где определяются
классы, которые должны быть доступны в скрипте.
*/

#include "scripting_decl.hpp"

#ifdef ___INANITY_SCRIPTING

#include "lua/scripting_impl.hpp"

#else

#define SCRIPTABLE_MAP_BEGIN(className, fullClassName)
#define SCRIPTABLE_MAP_END()
#define SCRIPTABLE_PARENT(parentClassName)
#define SCRIPTABLE_CONSTRUCTOR(...)
#define SCRIPTABLE_METHOD(methodName)

#endif

#endif
