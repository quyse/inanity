#include "Image2DData.hpp"
#include "../File.hpp"

Image2DData::Image2DData(ptr<File> data, int width, int height, int pitch, PixelFormat pixelFormat)
: data(data), width(width), height(height), pitch(pitch), pixelFormat(pixelFormat) {}

ptr<File> Image2DData::GetData() const
{
	return data;
}

int Image2DData::GetWidth() const
{
	return width;
}

int Image2DData::GetHeight() const
{
	return height;
}

int Image2DData::GetPitch() const
{
	return pitch;
}

PixelFormat Image2DData::GetPixelFormat() const
{
	return pixelFormat;
}
