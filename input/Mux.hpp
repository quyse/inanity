#ifndef ___INANITY_INPUT_MUX_HPP___
#define ___INANITY_INPUT_MUX_HPP___

#include "input.hpp"
#include <vector>

BEGIN_INANITY_INPUT

class Processor;
class Frame;

/// Класс, управляющий обработкой ввода.
/**
Суть такова. Есть набор обработчиков ввода.
Каждое событие обрабатывается сначала первым обработчиком,
затем вторым и так далее. Обработчик может сказать, что дальше обрабатывать не нужно,
тогда оставшиеся обработчики не получают событие.
После обработки событий обработчики могут обработать состояние ввода
(такое, как нажатость клавиш). Состояние ввода получают все обработчики,
прервать цепочку они не могут.
*/
class Mux : public Object
{
private:
	/// Обработчики ввода.
	std::vector<ptr<Processor> > processors;

public:
	void AddInputProcessor(ptr<Processor> processor);

	void ProcessEvents(ptr<Frame> frame);
};

END_INANITY_INPUT

#endif
