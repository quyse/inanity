#include "Device.hpp"

BEGIN_INANITY_GRAPHICS

Device::Caps::Caps() : flags(0) {}

const Device::Caps& Device::GetCaps() const
{
	return caps;
}

END_INANITY_GRAPHICS
