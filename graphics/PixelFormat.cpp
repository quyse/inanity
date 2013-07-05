#include "PixelFormat.hpp"
#include "../StreamWriter.hpp"
#include "../StreamReader.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

PixelFormat::PixelFormat() : type(typeUnknown) {}
PixelFormat::PixelFormat(Pixel pixel, Format format, Size size) :
	type(typeUncompressed), pixel(pixel), format(format), size(size) {}
PixelFormat::PixelFormat(Compression compression) :
	type(typeCompressed), compression(compression) {}

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
		break;
	case typeCompressed:
		writer.WriteShortly(compression);
		break;
	default:
		THROW_PRIMARY_EXCEPTION("Unsupported type");
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
			return PixelFormat(pixel, format, size);
		}
	case typeCompressed:
		{
			Compression compression = (Compression)reader.ReadShortly();
			return PixelFormat(compression);
		}
	default:
		THROW_PRIMARY_EXCEPTION("Unsupported type");
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

PixelFormat PixelFormats::uintRGBA32(
	PixelFormat::pixelRGBA,
	PixelFormat::formatUint,
	PixelFormat::size32bit);
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

END_INANITY_GRAPHICS
