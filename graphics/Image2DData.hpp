#ifndef ___INANITY_GRAPHICS_IMAGE_2D_DATA_HPP___
#define ___INANITY_GRAPHICS_IMAGE_2D_DATA_HPP___

#include "graphics.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_GRAPHICS

/// Класс буфера изображения.
/** Просто содержит графические данные - 2D текстуру.
Данные хранятся построчно, в прямом порядке, с шагом между строками
pitch. Картинки с прозрачностью хранятся не-premultiplied. */
class Image2DData : public Object
{
private:
	ptr<File> data;
	int width, height, pitch;
	PixelFormat pixelFormat;

public:
	Image2DData(ptr<File> data, int width, int height, int pitch, PixelFormat pixelFormat);

	ptr<File> GetData() const;
	int GetWidth() const;
	int GetHeight() const;
	int GetPitch() const;
	PixelFormat GetPixelFormat() const;
};

END_INANITY_GRAPHICS

#endif
