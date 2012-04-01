#ifndef ___INANITY_DX_CACHED_STATE_HPP___
#define ___INANITY_DX_CACHED_STATE_HPP___

#include "dx.hpp"
#include "StateCache.hpp"

BEGIN_INANITY_DX

/// Шаблонный класс кэшированного графического стейта.
/** Предоставляет абстракцию non-immutable стейта (в отличие
от стейтов DirectX). То есть можно изменять все настройки,
а когда стейт наконец понадобится, то внутренний immutable-объект
берется из кэша StateCache.
Предполагается, что от данного класса наследуются классы конкретных
стейтов, предоставляющие удобный интерфейс для изменения настроек.
При каждом изменении настроек подкласс должен обнулять state.
Не смотря на то, что он изменяемый, рекомендуется изменять его
как можно меньше - все равно там некоторое количество операций
производится, даже чтобы забрать его из кэша.
\tparam Interface Интерфейс DirectX, соответствующий стейту.
\tparam Desc Структура DirectX с настройками стейта.
*/
template <typename Interface, typename Desc>
class CachedState : public Object
{
public:
	/// Тип кэша стейтов.
	typedef StateCache<Interface, Desc> Cache;

private:
	/// Кэш стейтов.
	ptr<Cache> cache;

protected:
	/// Структура с настройками.
	Desc desc;
	/// Текущий стейт.
	/** Или 0, если стейт уже не актуален. */
	mutable ptr<State<Interface> > state;

public:
	CachedState(ptr<Cache> cache) : cache(cache)
	{
	}

	Interface* GetInternalInterface() const
	{
		if(!state)
			state = cache->GetState(desc);

		return state->GetInternalInterface();
	}

	const Desc& GetDesc() const
	{
		return desc;
	}

	void SetDesc(const Desc& desc)
	{
		this->desc = desc;
	}
};

END_INANITY_DX

#endif
