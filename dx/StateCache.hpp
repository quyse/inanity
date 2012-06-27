#ifndef ___INANITY_DX_STATE_CACHE_HPP___
#define ___INANITY_DX_STATE_CACHE_HPP___

#include "dx.hpp"
#include "System.hpp"
#include "State.hpp"
#include "StateCreator.hpp"
#include "../../ResourceLoader.hpp"
#include "../../Exception.hpp"
#include <unordered_map>

BEGIN_INANITY_DX

/// Шаблонный класс кэша стейтов.
/** Представляет собой специальный объект, хранящий ссылки на стейты с различными настройками,
и позволяющий получать их по этим настройкам (с помощью хеш-таблицы).
\tparam Interface Интерфейс DirectX, соответствующий стейту.
\tparam Desc Структура с параметрами стейта.
*/
template <typename Interface, typename Desc>
class StateCache : public Object
{
public:
	/// Кэш стейтов - статический ресурс.
	static String staticResourceName;

private:
	/// Хэш-таблица стейтов.
	std::unordered_map<String, ptr<State<Interface> > > cache;
	/// Система.
	ptr<System> system;

	/// Получить строку, соответствующую стейту.
	static inline String HashDesc(const Desc& desc)
	{
		return String((const char*)&desc, (const char*)&desc + sizeof(desc));
	}

public:
	/// Создать кеш стейтов.
	StateCache(ptr<System> system) : system(system) {}

	/// Получить стейт.
	ptr<State<Interface> > GetState(const Desc& desc)
	{
		//преобразовать настройки в строку
		String hash = HashDesc(desc);
		//попробовать найти в хеш-таблице
		std::unordered_map<String, ptr<State<Interface> > >::const_iterator i = cache.find(hash);
		if(i != cache.end())
			return i->second;

		//создать стейт, запомнить в кэше и вернуть
		Interface* stateInterface = StateCreator<Interface>::Create(system->GetDevice(), desc);
		if(stateInterface)
		{
			ptr<State<Interface> > state = NEW(State<Interface>(stateInterface));
			cache[hash] = state;
			return state;
		}
		THROW_PRIMARY_EXCEPTION("Can't get state");
	}

	/// Создать статический ресурс - кэш.
	static ptr<StateCache> CreateStaticResource(ResourceLoader* resourceLoader)
	{
		return NEW(StateCache(resourceLoader->GetStaticResource<System>()));
	}
};

END_INANITY_DX

#endif
