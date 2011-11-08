#ifndef ___INANITY_DX_CONTEXT_HPP___
#define ___INANITY_DX_CONTEXT_HPP___

#include "dx.hpp"
#include "d3d.hpp"
#include "../../ComPointer.hpp"

BEGIN_INANITY_DX

class Context : public Object
{
private:
	ComPointer<ID3D11DeviceContext> context;
	size_t lastViewportWidth, lastViewportHeight;

public:
	/// Создать графический контекст.
	Context(ID3D11DeviceContext* context);
};

END_INANITY_DX

#endif
