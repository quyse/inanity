#ifndef ___INANITY_GRAPHICS_PIXEL_FORMAT_HPP___
#define ___INANITY_GRAPHICS_PIXEL_FORMAT_HPP___

#include "graphics.hpp"

BEGIN_INANITY

class StreamWriter;
class StreamReader;

END_INANITY

BEGIN_INANITY_GRAPHICS

/// Struct which stores pixel format.
struct PixelFormat
{
	/// General type of the pixel data.
	enum Type
	{
		typeUnknown, typeUncompressed, typeCompressed
	};
	Type type;
	/// What components are in pixel.
	enum Pixel
	{
		pixelR, pixelRG, pixelRGB, pixelRGBA
	};
	/// Format of one component.
	enum Format
	{
		formatUntyped, formatUint, formatFloat
	};
	/// Size of one pixel, in bytes.
	enum Size
	{
		size8bit,
		size16bit,
		size24bit,
		size32bit,
		size64bit,
		size96bit,
		size128bit
	};
	/// Compression format.
	enum Compression
	{
		compressionDxt1,
		compressionDxt2,
		compressionDxt3,
		compressionDxt4,
		compressionDxt5
	};

	union
	{
		/// Struct for uncompressed data.
		struct
		{
			Pixel pixel;
			Format format;
			Size size;
		};
		/// Struct for compressed data.
		struct
		{
			Compression compression;
		};
	};

	/// Unknown format constructor.
	PixelFormat();
	/// Uncompressed format constructor.
	PixelFormat(Pixel pixel, Format format, Size size);
	/// Compressed format constructor.
	PixelFormat(Compression compression);

	void Serialize(StreamWriter& writer);
	static PixelFormat Deserialize(StreamReader& reader);

	static int GetPixelSize(Size size);
};

struct PixelFormats
{
	static PixelFormat uintRGBA32;
	static PixelFormat floatR16;
	static PixelFormat floatR32;
	static PixelFormat floatRGB32;
};

END_INANITY_GRAPHICS

#endif
