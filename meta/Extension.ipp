#ifndef ___INANITY_META_EXTENSION_IPP___
#define ___INANITY_META_EXTENSION_IPP___

#include "Extension.hpp"

#ifdef ___INANITY_META_LUA___
#include "../script/lua/extension.ipp"
#endif

#ifdef ___INANITY_META_V8___
#include "../script/v8/extension.ipp"
#endif

#endif
