#include "Output.hpp"

BEGIN_INANITY_GRAPHICS

Output::Output() : presenter(presenter) {}

void Output::SetPresenter(Presenter* presenter)
{
	this->presenter = presenter;
}

END_INANITY_GRAPHICS
