#include "DxgiMonitorMode.hpp"
#include <sstream>

BEGIN_INANITY_GRAPHICS

DxgiMonitorMode::DxgiMonitorMode(const DXGI_MODE_DESC& desc)
: desc(desc)
{
	// generate a name of monitor mode
	std::ostringstream stream;

	// width, height
	stream << GetWidth() << 'x' << GetHeight();
	// format
	switch(desc.Format)
	{
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		stream << ", 32 bit";
		break;
	case DXGI_FORMAT_B5G6R5_UNORM:
		stream << ", 16 bit";
		break;
	}

	// refresh rate
	stream << ", " << GetRefreshRate() << " Hz";

	// scanline ordering
	switch(desc.ScanlineOrdering)
	{
	case DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE:
		stream << ", progressive";
		break;
	case DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST:
		stream << ", interlaced upper-first";
		break;
	case DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST:
		stream << ", interlaced lower-first";
		break;
	}

	// scaling
	switch(desc.Scaling)
	{
	case DXGI_MODE_SCALING_CENTERED:
		stream << ", centered";
		break;
	case DXGI_MODE_SCALING_STRETCHED:
		stream << ", stretched";
		break;
	}

	name = stream.str();
}

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

String DxgiMonitorMode::GetName() const
{
	return name;
}

const DXGI_MODE_DESC& DxgiMonitorMode::GetDesc() const
{
	return desc;
}

END_INANITY_GRAPHICS
