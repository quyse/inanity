#ifndef ___INANITY_INPUT_FRAME_HPP___
#define ___INANITY_INPUT_FRAME_HPP___

#include "Event.hpp"
#include "State.hpp"
#include <vector>

BEGIN_INANITY_INPUT

/// Класс кадра устройства ввода.
/** Предоставляет доступ к последовательности событий устройства ввода,
произошедших за кадр, и получать состояние устройства.
Методы, которые получают собственно состояние, а также текущее событие,
определяются в подклассе.
*/
class Frame : public Object
{
private:
	/// Вектор событий.
	std::vector<Event> events;
	/// Индекс текущего события
	size_t currentEvent;
	/// Состояние устройств.
	State state;

public:
	Frame();

	/// Получить текущее событие.
	const Event& GetCurrentEvent() const;
	/// Получить текущее состояние.
	const State& GetCurrentState() const;
	/// Перейти к следующему событию.
	bool NextEvent();
	/// Перемотать все оставшиеся события.
	void ForwardEvents();

	/// Сбросить фрейм.
	/** Очищает список событий, сбрасывает индекс текущего события в ноль. */
	void Reset();
	/// Добавить событие.
	void AddEvent(const Event& e);
	/// Скопировать события и состояние в другой кадр.
	void CopyTo(ptr<Frame> frame);
};

END_INANITY_INPUT

#endif
