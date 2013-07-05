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
			THROW_PRIMARY_EXCEPTION("Width should be > 0");
		if(height < 0)
			THROW_PRIMARY_EXCEPTION("Height should be >= 0");
		if(height == 0)
		{
			if(depth != 0)
				THROW_PRIMARY_EXCEPTION("Depth != 0, but height == 0");
		}
		else
		{
			if(depth < 0)
				THROW_PRIMARY_EXCEPTION("Depth should be >= 0");
		}
		if(mips <= 0)
			THROW_PRIMARY_EXCEPTION("Mips should be > 0");
		if(count < 0)
			THROW_PRIMARY_EXCEPTION("Count should be >= 0");

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
				THROW_PRIMARY_EXCEPTION("Too much mips");

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
				THROW_PRIMARY_EXCEPTION("Wrong file size");
		}
		else
			this->pixels = NEW(MemoryFile(arrayPitch * (count ? count : 1)));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create raw texture data", exception);
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
		{
			int pixelSize = 0;
			switch(format.size)
			{
			case PixelFormat::size8bit: pixelSize = 1; break;
			case PixelFormat::size16bit: pixelSize = 2; break;
			case PixelFormat::size24bit: pixelSize = 3; break;
			case PixelFormat::size32bit: pixelSize = 4; break;
			case PixelFormat::size64bit: pixelSize = 8; break;
			case PixelFormat::size96bit: pixelSize = 12; break;
			case PixelFormat::size128bit: pixelSize = 16; break;
			}
			return width * pixelSize;
		}
		break;
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
		break;
	}
	return 0;
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
		THROW_SECONDARY_EXCEPTION("Can't serialize raw texture data", exception);
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
		THROW_SECONDARY_EXCEPTION("Can't deserialize raw texture data", exception);
	}
}

END_INANITY_GRAPHICS
