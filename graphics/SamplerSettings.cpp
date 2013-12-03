#include "SamplerSettings.hpp"

BEGIN_INANITY_GRAPHICS

SamplerSettings::SamplerSettings() :
	minFilter(filterPoint), mipFilter(filterPoint), magFilter(filterPoint),
	mipMapping(false),
	wrapU(wrapRepeat), wrapV(wrapRepeat), wrapW(wrapRepeat),
	minLOD(-1000), maxLOD(1000),
	borderColor(0, 0, 0, 0)
{}

void SamplerSettings::SetFilter(Filter minFilter, Filter mipFilter, Filter magFilter)
{
	this->minFilter = minFilter;
	this->mipFilter = mipFilter;
	this->magFilter = magFilter;
}

void SamplerSettings::SetFilter(Filter allFilter)
{
	SetFilter(allFilter, allFilter, allFilter);
}

void SamplerSettings::SetWrap(Wrap wrapU, Wrap wrapV, Wrap wrapW)
{
	this->wrapU = wrapU;
	this->wrapV = wrapV;
	this->wrapW = wrapW;
}

void SamplerSettings::SetWrap(Wrap wrapAll)
{
	SetWrap(wrapAll, wrapAll, wrapAll);
}

END_INANITY_GRAPHICS
