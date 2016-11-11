#include "PixelFormat.hpp"
#include "../StreamWriter.hpp"
#include "../StreamReader.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

PixelFormat::PixelFormat() : type(typeUnknown) {}
PixelFormat::PixelFormat(Pixel pixel, Format format, Size size, bool srgb) :
	type(typeUncompressed), pixel(pixel), format(format), size(size), srgb(srgb)
{
	switch(pixel)
	{
	case pixelR:
	case pixelRG:
		if(srgb)
			THROW("Wrong pixel type for sRGB");
		break;
	case pixelRGB:
	case pixelRGBA:
		break;
	default:
		THROW("Wrong pixel type");
	}
}
PixelFormat::PixelFormat(Compression compression, bool srgb) :
	type(typeCompressed), compression(compression), srgb(srgb)
{
	switch(compression)
	{
	case compressionBc1:
	case compressionBc1Alpha:
	case compressionBc2:
	case compressionBc3:
		break;
	case compressionBc4:
	case compressionBc4Signed:
	case compressionBc5:
	case compressionBc5Signed:
		if(srgb)
			THROW("Wrong compression format for sRGB");
		break;
	default:
		THROW("Wrong compression format");
	}
}

void PixelFormat::Serialize(StreamWriter& writer)
{
	writer.WriteShortly(type);
	switch(type)
	{
	case typeUnknown:
		break;
	case typeUncompressed:
		writer.WriteShortly(pixel);
		writer.WriteShortly(format);
		writer.WriteShortly(size);
		writer.WriteShortly((size_t)srgb);
		break;
	case typeCompressed:
		writer.WriteShortly(compression);
		writer.WriteShortly((size_t)srgb);
		break;
	default:
		THROW("Unsupported type");
	}
}

PixelFormat PixelFormat::Deserialize(StreamReader& reader)
{
	Type type = (Type)reader.ReadShortly();
	switch(type)
	{
	case typeUnknown:
		return PixelFormat();
	case typeUncompressed:
		{
			Pixel pixel = (Pixel)reader.ReadShortly();
			Format format = (Format)reader.ReadShortly();
			Size size = (Size)reader.ReadShortly();
			bool srgb = !!reader.ReadShortly();
			return PixelFormat(pixel, format, size, srgb);
		}
	case typeCompressed:
		{
			Compression compression = (Compression)reader.ReadShortly();
			bool srgb = !!reader.ReadShortly();
			return PixelFormat(compression, srgb);
		}
	default:
		THROW("Unsupported type");
	}
}

int PixelFormat::GetPixelSize(Size size)
{
	switch(size)
	{
	case size8bit: return 1;
	case size16bit: return 2;
	case size24bit: return 3;
	case size32bit: return 4;
	case size64bit: return 8;
	case size96bit: return 12;
	case size128bit: return 16;
	default: return 0;
	}
}

bool operator==(const PixelFormat& a, const PixelFormat& b)
{
	if(a.type != b.type)
		return false;
	switch(a.type)
	{
	case PixelFormat::typeUnknown:
		return false;
	case PixelFormat::typeUncompressed:
		return
			a.pixel == b.pixel &&
			a.format == b.format &&
			a.size == b.size &&
			a.srgb == b.srgb;
	case PixelFormat::typeCompressed:
		return
			a.compression == b.compression &&
			a.srgb == b.srgb;
	}
	return false;
}

PixelFormat PixelFormats::uintRGBA32(
	PixelFormat::pixelRGBA,
	PixelFormat::formatUint,
	PixelFormat::size32bit);
PixelFormat PixelFormats::uintRGBA32S(
	PixelFormat::pixelRGBA,
	PixelFormat::formatUint,
	PixelFormat::size32bit,
	true);
PixelFormat PixelFormats::floatR16(
	PixelFormat::pixelR,
	PixelFormat::formatFloat,
	PixelFormat::size16bit);
PixelFormat PixelFormats::floatR32(
	PixelFormat::pixelR,
	PixelFormat::formatFloat,
	PixelFormat::size32bit);
PixelFormat PixelFormats::floatRGB32(
	PixelFormat::pixelRGB,
	PixelFormat::formatFloat,
	PixelFormat::size32bit);
PixelFormat PixelFormats::floatRGBA64(
	PixelFormat::pixelRGBA,
	PixelFormat::formatFloat,
	PixelFormat::size64bit);

END_INANITY_GRAPHICS
