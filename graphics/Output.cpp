#include "Output.hpp"

BEGIN_INANITY_GRAPHICS

Output::Output() : presenter(0) {}

void Output::SetPresenter(Presenter* presenter)
{
	this->presenter = presenter;
}

END_INANITY_GRAPHICS
