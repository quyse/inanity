#ifndef ___INANITY_INPUT_PROCESSOR_HPP___
#define ___INANITY_INPUT_PROCESSOR_HPP___

#include "input.hpp"

BEGIN_INANITY_INPUT

struct Event;
struct State;

/// Абстрактный класс обработчика ввода.
class Processor : public Object
{
public:
	/// Обработать событие ввода.
	/**
	\returns Нужно ли передавать событие далее по стеку обработчиков.
	Реализация по умолчанию ничего не делает, и возвращает true.
	*/
	virtual bool ProcessEvent(const Event& event);
	/// Обработать состояние ввода.
	/** Вызывается один раз за кадр, после обработки всех событий.
	Реализация по умолчанию ничего не делает.
	*/
	virtual void ProcessState(const State& state);
};

END_INANITY_INPUT

#endif
