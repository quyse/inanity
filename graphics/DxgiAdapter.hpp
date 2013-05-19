#ifndef ___INANITY_GRAPHICS_DXGI_ADAPTER_HPP___
#define ___INANITY_GRAPHICS_DXGI_ADAPTER_HPP___

#include "Adapter.hpp"
#include "../ComPointer.hpp"
#include "d3d11.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс графического адаптера DXGI.
class DxgiAdapter : public Adapter
{
private:
	ComPointer<IDXGIAdapter> adapter;
	DXGI_ADAPTER_DESC desc;
	std::vector<ptr<Monitor> > monitors;
	bool monitorsInitialized;

public:
	DxgiAdapter(ComPointer<IDXGIAdapter> adapter);

	//*** методы Adapter
	String GetId() const;
	String GetName() const;
	const std::vector<ptr<Monitor> >& GetMonitors();
};

END_INANITY_GRAPHICS

#endif
