#ifndef ___INANITY_LIBEV_EVENT_LOOP_HPP___
#define ___INANITY_LIBEV_EVENT_LOOP_HPP___

#include "Object.hpp"

BEGIN_INANITY

/// Класс цикла обработки событий, использующего libev для реализации.
class LibevEventLoop : public Object
{
private:

public:
	/// Запустить цикл.
	void Run();
};

END_INANITY

#endif
