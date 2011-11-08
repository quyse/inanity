#include "Context.hpp"

DX::Context::Context(ID3D11DeviceContext* context)
	: context(context), lastViewportWidth(0), lastViewportHeight(0)
{
}
