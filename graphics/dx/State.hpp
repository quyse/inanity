#ifndef ___INANITY_DX_GRAPHICS_STATE_HPP___
#define ___INANITY_DX_GRAPHICS_STATE_HPP___

#include "dx.hpp"
#include "../../ComPointer.hpp"

BEGIN_INANITY_DX

/// Шаблонный класс для хранения стейтов.
/** Стейт - это объект, который представляет собой фактически структуру с настройками,
но в DirectX является неизменяемым объектом. Для удобного использования предназначен
класс CachedGraphicsState, работающий совместно с GraphicsStateCache. Этот же класс
представляет собой просто обёртку вокруг DirectX-интерфейса. */
template <typename Interface>
class State : public Object
{
private:
	ComPointer<Interface> state;

public:
	State(Interface* state) : state(state)
	{
	}

	Interface* GetInternalInterface() const
	{
		return state;
	}
};

END_INANITY_DX

#endif
