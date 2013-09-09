#ifndef ___INANITY_INANITY_COMPRESS_META_IPP___
#define ___INANITY_INANITY_COMPRESS_META_IPP___

#include "inanity-base-meta.ipp"

#include "CompressStream.hpp"
META_CLASS(Inanity::CompressStream, Inanity.CompressStream);
	META_CLASS_PARENT(OutputStream);
	META_METHOD(Flush);
	META_STATIC_METHOD(CreateMax);
META_CLASS_END();

#include "DecompressStream.hpp"
META_CLASS(Inanity::DecompressStream, Inanity.DecompressStream);
	META_CLASS_PARENT(InputStream);
	META_CONSTRUCTOR(ptr<InputStream>);
	META_STATIC_METHOD(DecompressFile);
META_CLASS_END();

#endif
