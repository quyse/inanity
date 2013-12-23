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
	Monitors monitors;
	bool monitorsInitialized;

public:
	DxgiAdapter(ComPointer<IDXGIAdapter> adapter);

	//*** методы Adapter
	String GetId() const;
	String GetName() const;
	const Monitors& GetMonitors();

	IDXGIAdapter* GetInterface() const;
};

END_INANITY_GRAPHICS

#endif
