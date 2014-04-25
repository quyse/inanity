#ifndef ___INANITY_INANITY_GRAPHICS_META_IPP___
#define ___INANITY_INANITY_GRAPHICS_META_IPP___

#include "graphics/Texture.hpp"
META_CLASS(Inanity::Graphics::Texture, Inanity.Graphics.Texture);
META_CLASS_END();

#include "graphics/RawTextureData.hpp"
META_CLASS(Inanity::Graphics::RawTextureData, Inanity.Graphics.RawTextureData);
	META_METHOD(GetImageWidth);
	META_METHOD(GetImageHeight);
	META_METHOD(GetImageDepth);
	META_METHOD(GetImageSize);
	META_METHOD(GetImageMips);
	META_METHOD(GetCount);
	META_METHOD(GetMipWidth);
	META_METHOD(GetMipHeight);
	META_METHOD(GetMipDepth);
	META_METHOD(GetMipBufferWidth);
	META_METHOD(GetMipBufferHeight);
	META_METHOD(GetPixelSize);
	META_METHOD(GetMipLinePitch);
	META_METHOD(GetMipSlicePitch);
	META_METHOD(Serialize);
	META_STATIC_METHOD(Deserialize);
	META_METHOD(Blit);
META_CLASS_END();

#include "graphics/ImageLoader.hpp"
META_CLASS(Inanity::Graphics::ImageLoader, Inanity.Graphics.ImageLoader);
	META_METHOD(Load);
META_CLASS_END();

#include "graphics/BmpImage.hpp"
META_CLASS(Inanity::Graphics::BmpImage, Inanity.Graphics.BmpImage);
	META_STATIC_METHOD(Load);
	META_STATIC_METHOD(Save);
	META_STATIC_METHOD(CreateLoader);
META_CLASS_END();

#include "graphics/TgaImageLoader.hpp"
META_CLASS(Inanity::Graphics::TgaImageLoader, Inanity.Graphics.TgaImageLoader);
	META_CLASS_PARENT(Inanity::Graphics::ImageLoader);
	META_CONSTRUCTOR();
META_CLASS_END();

#endif
