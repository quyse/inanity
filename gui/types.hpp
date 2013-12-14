#ifndef ___INANITY_GUI_TYPES_HPP___
#define ___INANITY_GUI_TYPES_HPP___

#include "gui.hpp"
#include "../inanity-math.hpp"
#include "../input/Key.hpp"
#include "../input/Event.hpp"

BEGIN_INANITY_GUI

/// GUI distance type.
typedef float Distance;
/// GUI position type.
typedef Math::xvec<Distance, 2> Position;
/// GUI size type.
typedef Math::xvec<Distance, 2> Size;

/// Input mouse event type.
/** Contains:
enum Type
enum Button
*/
typedef Input::Event::Mouse MouseEvent;

/// Keyboard key type.
typedef Input::Key Key;
typedef Input::Keys Keys;

END_INANITY_GUI

#endif
