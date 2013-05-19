#include "DxgiMonitorMode.hpp"

BEGIN_INANITY_GRAPHICS

DxgiMonitorMode::DxgiMonitorMode(const DXGI_MODE_DESC& desc)
: desc(desc) {}

int DxgiMonitorMode::GetWidth() const
{
	return desc.Width;
}

int DxgiMonitorMode::GetHeight() const
{
	return desc.Height;
}

float DxgiMonitorMode::GetRefreshRate() const
{
	if(!desc.RefreshRate.Denominator)
		return 0;
	return float(desc.RefreshRate.Numerator) / float(desc.RefreshRate.Denominator);
}

const DXGI_MODE_DESC& DxgiMonitorMode::GetDesc() const
{
	return desc;
}

END_INANITY_GRAPHICS
