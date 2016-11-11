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
		// BC1, or DXT1: RGB, size of 4x4 = 8 bytes
		compressionBc1,
		compressionBc1Alpha,
		// BC2, or DXT3: RGBA, size of 4x4 = 16 bytes (low cogerency of color and alpha)
		compressionBc2,
		// BC3, or DXT5: RGBA, size of 4x4 = 16 bytes (high cogerency of color data)
		compressionBc3,
		// BC4, or ATI1: R, size of 4x4 = 8 bytes
		compressionBc4,
		compressionBc4Signed,
		// BC5, or ATI2: RG, size of 4x4 = 16 bytes
		compressionBc5,
		compressionBc5Signed
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

	/// Is color data is in sRGB space.
	/** Valid only for type != typeUnknown. */
	bool srgb;

	/// Unknown format constructor.
	PixelFormat();
	/// Uncompressed format constructor.
	PixelFormat(Pixel pixel, Format format, Size size, bool srgb = false);
	/// Compressed format constructor.
	PixelFormat(Compression compression, bool srgb = false);

	void Serialize(StreamWriter& writer);
	static PixelFormat Deserialize(StreamReader& reader);

	static int GetPixelSize(Size size);
};

bool operator==(const PixelFormat& a, const PixelFormat& b);

struct PixelFormats
{
	static PixelFormat uintRGBA32;
	static PixelFormat uintRGBA32S;
	static PixelFormat floatR16;
	static PixelFormat floatR32;
	static PixelFormat floatRGB32;
	static PixelFormat floatRGBA64;
};

END_INANITY_GRAPHICS

#endif
