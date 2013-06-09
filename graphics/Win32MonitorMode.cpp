#include "Win32MonitorMode.hpp"
#include <sstream>

BEGIN_INANITY_GRAPHICS

Win32MonitorMode::Win32MonitorMode(const DEVMODE& info) : info(info)
{
	// generate name
	std::ostringstream stream;

	// width, height
	if(info.dmFields & DM_PELSWIDTH)
		stream << info.dmPelsWidth;
	else
		stream << '?';
	stream << 'x';
	if(info.dmFields & DM_PELSHEIGHT)
		stream << info.dmPelsHeight;
	else
		stream << '?';

	// format
	if(info.dmFields & DM_BITSPERPEL)
		stream << ", " << info.dmBitsPerPel << " bit";

	// refresh rate
	if(info.dmFields & DM_DISPLAYFREQUENCY)
		stream << ", " << info.dmDisplayFrequency << " Hz";

	name = stream.str();
}

int Win32MonitorMode::GetWidth() const
{
	return (info.dmFields & DM_PELSWIDTH) ? (int)info.dmPelsWidth : 0;
}

int Win32MonitorMode::GetHeight() const
{
	return (info.dmFields & DM_PELSHEIGHT) ? (int)info.dmPelsHeight : 0;
}

float Win32MonitorMode::GetRefreshRate() const
{
	return (info.dmFields & DM_DISPLAYFREQUENCY) ? (float)info.dmDisplayFrequency : 0;
}

String Win32MonitorMode::GetName() const
{
	return name;
}

const DEVMODE& Win32MonitorMode::GetInfo() const
{
	return info;
}

END_INANITY_GRAPHICS
