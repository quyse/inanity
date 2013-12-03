#include "SamplerSettings.hpp"

BEGIN_INANITY_GRAPHICS

SamplerSettings::SamplerSettings() :
	minFilter(filterPoint), mipFilter(filterPoint), magFilter(filterPoint),
	mipMapping(false),
	wrapU(wrapRepeat), wrapV(wrapRepeat), wrapW(wrapRepeat),
	minLOD(-1000), maxLOD(1000),
	borderColor(0, 0, 0, 0)
{}

END_INANITY_GRAPHICS
