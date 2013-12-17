#ifndef ___INANITY_INANITY_DATA_META_IPP___
#define ___INANITY_INANITY_DATA_META_IPP___

#include "BlobFileSystem.hpp"
META_CLASS(Inanity::BlobFileSystem, Inanity.BlobFileSystem);
	META_CLASS_PARENT(Inanity::FileSystem);
	META_CONSTRUCTOR(ptr<File>);
META_CLASS_END();

#include "BlobFileSystemBuilder.hpp"
META_CLASS(Inanity::BlobFileSystemBuilder, Inanity.BlobFileSystemBuilder);
	META_CONSTRUCTOR(ptr<OutputStream>);
	META_METHOD(AddFile);
	META_METHOD(AddFileStream);
	META_METHOD(Finalize);
META_CLASS_END();

#endif
