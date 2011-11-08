#ifndef ___INANITY_DX_DX_HPP___
#define ___INANITY_DX_DX_HPP___

/* Файл, общий для подсистемы DX.
 */

#include "../graphics/graphics.hpp"

#define BEGIN_INANITY_DX BEGIN_INANITY namespace DX {
#define END_INANITY_DX } END_INANITY
#ifdef INANITY_LIB
BEGIN_INANITY_DX
END_INANITY_DX
using namespace DX;
#endif

#endif
