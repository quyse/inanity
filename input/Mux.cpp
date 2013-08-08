#include "Mux.hpp"
#include "Processor.hpp"
#include "Frame.hpp"

BEGIN_INANITY_INPUT

void Mux::AddInputProcessor(ptr<Processor> processor)
{
	processors.push_back(processor);
}

void Mux::ProcessEvents(ptr<Frame> frame)
{
	//цикл по событиям
	while(frame->NextEvent())
	{
		//получить событие
		const Event& event = frame->GetCurrentEvent();
		//и обработать его
		for(size_t i = 0; i < processors.size() && processors[i]->ProcessEvent(event); ++i);
	}
	//обработать состояние ввода после всех событий
	const State& state = frame->GetCurrentState();
	for(size_t i = 0; i < processors.size(); ++i)
		processors[i]->ProcessState(state);
}

END_INANITY_INPUT
