#include "SamplingState.hpp"

SamplingState::SamplingState() :
	minFilter(filterPoint), mipFilter(filterPoint), magFilter(filterPoint),
	mipMapping(false),
	wrapU(wrapRepeat), wrapV(wrapRepeat), wrapW(wrapRepeat),
	minLOD(-1000), maxLOD(1000),
	dirty(true)
{
	for(int i = 0; i < 4; ++i)
		borderColor[i] = 0;
}

void SamplingState::SetFilter(Filter minFilter, Filter mipFilter, Filter magFilter)
{
	this->minFilter = minFilter;
	this->mipFilter = mipFilter;
	this->magFilter = magFilter;
	dirty = true;
}

void SamplingState::SetMipMapping(bool mipMapping)
{
	this->mipMapping = mipMapping;
	dirty = true;
}

void SamplingState::SetWrap(Wrap wrapU, Wrap wrapV, Wrap wrapW)
{
	this->wrapU = wrapU;
	this->wrapV = wrapV;
	this->wrapW = wrapW;
	dirty = true;
}

void SamplingState::SetLODLimits(float minLOD, float maxLOD)
{
	this->minLOD = minLOD;
	this->maxLOD = maxLOD;
	dirty = true;
}

void SamplingState::SetBorderColor(const float* borderColor)
{
	for(int i = 0; i < 4; ++i)
		this->borderColor[i] = borderColor[i];
	dirty = true;
}
