#ifndef ___INANITY_GRAPHICS_RAW_TEXTURE_DATA_HPP___
#define ___INANITY_GRAPHICS_RAW_TEXTURE_DATA_HPP___

#include "PixelFormat.hpp"
#include <vector>

BEGIN_INANITY

class File;
class OutputStream;
class InputStream;

END_INANITY

BEGIN_INANITY_GRAPHICS

/// The class represents raw texture data.
/**
Raw texture data represents an array of images.
Each of the images can be 1D, 2D or 3D texture.
Each of the images can have mipmap levels.
*/
class RawTextureData : public Object
{
public:
	/// Cube texture array indices for convenience.
	enum CubeFace
	{
		cubeFacePositiveX,
		cubeFaceNegativeX,
		cubeFacePositiveY,
		cubeFaceNegativeY,
		cubeFacePositiveZ,
		cubeFaceNegativeZ
	};

private:
	/// Texture data.
	/** Meaning of bytes is determined by format. */
	ptr<File> pixels;
	/// Pixel format.
	PixelFormat format;
	/// Sizes (1 if should apply, 0 if not).
	/** Acceptable combinations:
	width height depth
	W     0      0      1D texture with width W
	W     1      0      2D texture Wx1
	W     H      0      2D texture WxH
	W     H      1      3D texture WxHx1
	W     H      L      3D texture WxHxL
	*/
	int width, height, depth;
	/// Number of mip levels.
	/** Greater than or equal to one. */
	int mips;
	/// Array size
	/** If 1 - it's array with one texture.
	If 0 - it's not an array, but one texture. */
	int count;

	/// Size of one image in array (with all mips).
	int arrayPitch;
	/// Offsets to the mips in data of one image.
	std::vector<int> mipOffsets;

public:
	RawTextureData(ptr<File> pixels, PixelFormat format, int width, int height, int depth, int mips, int count);

	PixelFormat GetFormat() const;
	void SetFormat(PixelFormat format);
	/// Gets a width of topmost mip of one image.
	int GetImageWidth() const;
	/// Gets a height of topmost mip of one image.
	/** 0 if image is 1D. */
	int GetImageHeight() const;
	/// Gets a depth of topmost mip of one image.
	/** 0 if image is 1D or 2D. */
	int GetImageDepth() const;
	/// Gets a size of one image in array, in bytes.
	int GetImageSize() const;
	/// Gets a mips number of one image.
	int GetImageMips() const;
	/// Gets a count of images in array.
	int GetCount() const;

	int GetMipWidth(int mip = 0) const;
	int GetMipHeight(int mip = 0) const;
	int GetMipDepth(int mip = 0) const;
	int GetMipBufferWidth(int mip = 0) const;
	int GetMipBufferHeight(int mip = 0) const;

	/// Get pixel size (only for uncompressed formats).
	int GetPixelSize() const;

	/// Get data of one mip.
	void* GetMipData(int image = 0, int mip = 0) const;
	/// Get data size of one mip.
	int GetMipSize(int mip = 0) const;
	/// Get line pitch into data of one image.
	int GetMipLinePitch(int mip = 0) const;
	/// Get slice pitch into data of one image.
	int GetMipSlicePitch(int mip = 0) const;

	/// Save texture to a stream.
	void Serialize(ptr<OutputStream> stream);
	/// Load texture from stream.
	static ptr<RawTextureData> Deserialize(ptr<InputStream> stream);

	/// Blit another 2D image on this image.
	/** Clip if isn't in bounds. */
	void Blit(ptr<RawTextureData> image, int destX, int destY, int sourceX, int sourceY, int width, int height);

	/// Premultiply color components with alpha.
	ptr<RawTextureData> PremultiplyAlpha() const;

	/// Convert to RG format.
	ptr<RawTextureData> ConvertToRG() const;

	/// Generate mip levels from zero level.
	/** Existing levels starting from 1 are ignored. If mipsCount == 0 then optimal number of mips calculated.
	Currently 2x downscale is correct, if it's not 2x it's simple nearest neighbour.
	Algorithm simply averages per-component and doesn't weight RGB colors with alpha.
	Pre-multiply RGB with alpha to correctly generate RGBA mips. */
	ptr<RawTextureData> GenerateMips(int newMips = 0) const;

	/// Union several 2D images into one.
	/** Images packed in rows, with specified result width. */
	static ptr<RawTextureData> ShelfUnion(
		const std::vector<ptr<RawTextureData> >& images,
		std::vector<std::pair<int, int> >& outPositions,
		int resultWidth,
		int border = 0,
		bool heightPowerOfTwo = false
	);
};

END_INANITY_GRAPHICS

#endif
