#include "RawTextureData.hpp"
#include "../StreamWriter.hpp"
#include "../StreamReader.hpp"
#include "../MemoryFile.hpp"
#include "../Exception.hpp"
#include <algorithm>

BEGIN_INANITY_GRAPHICS

RawTextureData::RawTextureData(ptr<File> pixels, PixelFormat format, int width, int height, int depth, int mips, int count)
: pixels(pixels), format(format), width(width), height(height), depth(depth), mips(mips), count(count)
{
	try
	{
		// check params
		if(width <= 0)
			THROW("Width should be > 0");
		if(height < 0)
			THROW("Height should be >= 0");
		if(height == 0)
		{
			if(depth != 0)
				THROW("Depth != 0, but height == 0");
		}
		else
		{
			if(depth < 0)
				THROW("Depth should be >= 0");
		}
		if(mips <= 0)
			THROW("Mips should be > 0");
		if(count < 0)
			THROW("Count should be >= 0");

		// get a maximum dimension of image
		int maxDimension = std::max(std::max(width, height), depth);

		// calculate mip offsets
		// we use a floor-scheme, as it used in OpenGL
		mipOffsets.resize(mips);
		int mipOffset = 0;
		for(int i = 0; i < mips; ++i)
		{
			// if we reach the zero dimension, mips was too big
			if(maxDimension == 0)
				THROW("Too much mips");

			// store current offset
			mipOffsets[i] = mipOffset;

			// calculate size of the current mip
			int mipSize = GetMipSlicePitch(i) * GetMipDepth(i);
			// advance offset by current mip size
			mipOffset += mipSize;

			// max dimension of next mip
			maxDimension /= 2;
		}

		// store the size of one image
		arrayPitch = mipOffset;

		// check the size of file (or create it if doesn't created yet)
		if(pixels)
		{
			if(pixels->GetSize() != arrayPitch * (count ? count : 1))
				THROW("Wrong file size");
		}
		else
			this->pixels = NEW(MemoryFile(arrayPitch * (count ? count : 1)));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create raw texture data", exception);
	}
}

PixelFormat RawTextureData::GetFormat() const
{
	return format;
}

int RawTextureData::GetImageWidth() const
{
	return width;
}

int RawTextureData::GetImageHeight() const
{
	return height;
}

int RawTextureData::GetImageDepth() const
{
	return depth;
}

int RawTextureData::GetImageSize() const
{
	return arrayPitch;
}

int RawTextureData::GetImageMips() const
{
	return mips;
}

int RawTextureData::GetCount() const
{
	return count;
}

int RawTextureData::GetMipWidth(int mip) const
{
	return std::max(width >> mip, 1);
}

int RawTextureData::GetMipHeight(int mip) const
{
	return std::max(height >> mip, 1);
}

int RawTextureData::GetMipDepth(int mip) const
{
	return std::max(depth >> mip, 1);
}

int RawTextureData::GetMipBufferWidth(int mip) const
{
	int width = GetMipWidth(mip);
	if(format.type == PixelFormat::typeCompressed)
		return (width + 3) & ~3;
	return width;
}

int RawTextureData::GetMipBufferHeight(int mip) const
{
	int height = GetMipHeight(mip);
	if(format.type == PixelFormat::typeCompressed)
		return (height + 3) & ~3;
	return height;
}

int RawTextureData::GetPixelSize() const
{
	int pixelSize;
	switch(format.type)
	{
	case PixelFormat::typeUncompressed:
		pixelSize = PixelFormat::GetPixelSize(format.size);
		break;
	default:
		pixelSize = 0;
	}
	return pixelSize;
}

void* RawTextureData::GetMipData(int image, int mip) const
{
	return (char*)pixels->GetData() + image * arrayPitch + mipOffsets[mip];
}

int RawTextureData::GetMipLinePitch(int mip) const
{
	int width = GetMipBufferWidth(mip);
	switch(format.type)
	{
	case PixelFormat::typeUncompressed:
		return width * PixelFormat::GetPixelSize(format.size);
	case PixelFormat::typeCompressed:
		{
			int blockSize = 0;
			switch(format.compression)
			{
			case PixelFormat::compressionDxt1: blockSize = 8; break;
			case PixelFormat::compressionDxt2: blockSize = 16; break;
			case PixelFormat::compressionDxt3: blockSize = 16; break;
			case PixelFormat::compressionDxt4: blockSize = 16; break;
			case PixelFormat::compressionDxt5: blockSize = 16; break;
			}
			return width / 4 * blockSize;
		}
	default:
		return 0;
	}
}

int RawTextureData::GetMipSlicePitch(int mip) const
{
	int height = GetMipBufferHeight(mip);
	if(format.type == PixelFormat::typeCompressed)
		height /= 4;
	return GetMipLinePitch(mip) * height;
}

void RawTextureData::Serialize(ptr<OutputStream> stream)
{
	try
	{
		StreamWriter writer(stream);

		format.Serialize(writer);
		writer.WriteShortly(width);
		writer.WriteShortly(height);
		writer.WriteShortly(depth);
		writer.WriteShortly(mips);
		writer.WriteShortly(count);

		int realCount = count > 0 ? count : 1;
		int realMips = mips > 0 ? mips : 1;

		for(int image = 0; image < realCount; ++image)
			for(int mip = 0; mip < realMips; ++mip)
			{
				const char* sliceData = (const char*)GetMipData(image, mip);
				int mipDepth = GetMipDepth(mip);
				int mipHeight = GetMipHeight(mip);
				int mipLinePitch = GetMipLinePitch(mip);
				int mipSlicePitch = GetMipSlicePitch(mip);
				for(int slice = 0; slice < mipDepth; ++slice)
				{
					const char* lineData = sliceData;
					for(int line = 0; line < mipHeight; ++line)
					{
						writer.Write(lineData, mipLinePitch);
						lineData += mipLinePitch;
					}
					sliceData += mipSlicePitch;
				}
			}
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't serialize raw texture data", exception);
	}
}

ptr<RawTextureData> RawTextureData::Deserialize(ptr<InputStream> stream)
{
	try
	{
		StreamReader reader(stream);

		PixelFormat format = PixelFormat::Deserialize(reader);
		int width = (int)reader.ReadShortly();
		int height = (int)reader.ReadShortly();
		int depth = (int)reader.ReadShortly();
		int mips = (int)reader.ReadShortly();
		int count = (int)reader.ReadShortly();

		ptr<RawTextureData> data = NEW(RawTextureData(0, format, width, height, depth, mips, count));

		int realCount = count > 0 ? count : 1;
		int realMips = mips > 0 ? mips : 1;

		for(int image = 0; image < realCount; ++image)
			for(int mip = 0; mip < realMips; ++mip)
			{
				char* sliceData = (char*)data->GetMipData(image, mip);
				int mipDepth = data->GetMipDepth(mip);
				int mipHeight = data->GetMipHeight(mip);
				int mipLinePitch = data->GetMipLinePitch(mip);
				int mipSlicePitch = data->GetMipSlicePitch(mip);
				for(int slice = 0; slice < mipDepth; ++slice)
				{
					char* lineData = sliceData;
					for(int line = 0; line < mipHeight; ++line)
					{
						reader.Read(lineData, mipLinePitch);
						lineData += mipLinePitch;
					}
					sliceData += mipSlicePitch;
				}
			}

		return data;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't deserialize raw texture data", exception);
	}
}

void RawTextureData::Blit(RawTextureData* image, int destX, int destY, int sourceX, int sourceY, int width, int height)
{
	BEGIN_TRY();

	if(!(format == image->format))
		THROW("Format should be the same");
	if(!(
		width > 0 &&
		height > 0 &&
		depth == 0))
		THROW("Wrong destination image type");
	if(!(
		image->width > 0 &&
		image->height > 0 &&
		image->depth == 0))
		THROW("Wrong source image type");

	if(destX < 0)
	{
		sourceX -= destX;
		width += destX;
		destX = 0;
	}
	if(destY < 0)
	{
		sourceY -= destY;
		height += destY;
		destY = 0;
	}

	width = std::min(sourceX + width, image->width) - sourceX;
	width = std::min(destX + width, this->width) - destX;
	if(width <= 0)
		return;

	height = std::min(sourceY + height, image->height) - sourceY;
	height = std::min(destY + height, this->height) - destY;
	if(height <= 0)
		return;

	char* destData = (char*)GetMipData();
	char* sourceData = (char*)image->GetMipData();
	int destPitch = GetMipLinePitch();
	int sourcePitch = image->GetMipLinePitch();
	int pixelSize = GetPixelSize();
	int copySize = width * pixelSize;

	for(int i = 0; i < height; ++i)
		memcpy(
			destData + (i + destY) * destPitch + destX * pixelSize,
			sourceData + (i + sourceY) * sourcePitch + sourceX * pixelSize,
			copySize);

	END_TRY("Can't blit raw texture data");
}

ptr<RawTextureData> RawTextureData::ShelfUnion(
	const std::vector<ptr<RawTextureData> >& images,
	int resultWidth,
	int border,
	std::vector<std::pair<int, int> >& outPositions
)
{
	BEGIN_TRY();

	int imagesCount = (int)images.size();

	// check types of images
	for(int i = 0; i < imagesCount; ++i)
	{
		RawTextureData* image = images[i];
		if(!(
			image->width > 0 &&
			image->height > 0 &&
			image->depth == 0 &&
			image->mips == 1 &&
			image->count == 0))
			THROW("Wrong image type");
	}

	// first pass: sort images by height
	struct Sorter
	{
		bool operator()(const std::pair<RawTextureData*, int>& a, const std::pair<RawTextureData*, int>& b) const
		{
			return a.first->GetImageHeight() > b.first->GetImageHeight();
		}
	};
	std::vector<std::pair<RawTextureData*, int> > sortedImages(imagesCount);
	for(int i = 0; i < imagesCount; ++i)
		sortedImages[i] = std::pair<RawTextureData*, size_t>(images[i], i);
	std::sort(sortedImages.begin(), sortedImages.end(), Sorter());

	// second pass: determine result height
	int resultHeight = border;
	{
		int currentX = border, currentRowHeight = 0;
		for(int i = 0; i < imagesCount; ++i)
		{
			RawTextureData* image = sortedImages[i].first;
			int width = image->GetMipWidth();
			if(currentX + width + border > resultWidth)
			{
				// move to the next row
				resultHeight += currentRowHeight;
				currentRowHeight = 0;
				currentX = border;
			}
			currentX += width + border;
			currentRowHeight = std::max(currentRowHeight, image->GetMipHeight() + border);
		}
		resultHeight += currentRowHeight;
	}

	// third pass: combine images
	outPositions.resize(imagesCount);
	ptr<MemoryFile> pixelsFile = NEW(MemoryFile(resultWidth * resultHeight));
	char* pixelsData = (char*)pixelsFile->GetData();
	memset(pixelsData, 0, pixelsFile->GetSize());
	{
		int currentX = border, currentRowY = border, currentRowHeight = 0;
		for(int i = 0; i < imagesCount; ++i)
		{
			RawTextureData* image = sortedImages[i].first;
			int width = image->GetMipWidth();
			if(currentX + width + border > resultWidth)
			{
				// move to the next row
				currentRowY += currentRowHeight;
				currentRowHeight = 0;
				currentX = border;
			}

			// place image
			int height = image->GetMipHeight();
			const char* data = (const char*)image->GetMipData();
			for(int y = 0; y < height; ++y)
				memcpy(
					pixelsData + (currentRowY + y) * resultWidth + currentX,
					data + y * width,
					width);

			// output a position
			outPositions[sortedImages[i].second].first = currentX;
			outPositions[sortedImages[i].second].second = currentRowY;

			currentX += width + border;
			currentRowHeight = std::max(currentRowHeight, height + border);
		}
	}

	return NEW(RawTextureData(
		pixelsFile,
		PixelFormat(
			PixelFormat::pixelR,
			PixelFormat::formatUint,
			PixelFormat::size8bit),
		resultWidth,
		resultHeight,
		0, // depth
		1, // mips
		0 // count
		));

	END_TRY("Can't union 2D raw images");
}

END_INANITY_GRAPHICS
