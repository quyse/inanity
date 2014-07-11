#include "data/BlobFileSystem.hpp"
META_CLASS(Inanity::Data::BlobFileSystem, Inanity.Data.BlobFileSystem);
	META_CLASS_PARENT(Inanity::FileSystem);
	META_CONSTRUCTOR(ptr<File>);
META_CLASS_END();

#include "data/BlobFileSystemBuilder.hpp"
META_CLASS(Inanity::Data::BlobFileSystemBuilder, Inanity.Data.BlobFileSystemBuilder);
	META_CONSTRUCTOR(ptr<OutputStream>);
	META_METHOD(AddFile);
	META_METHOD(AddFileStream);
	META_METHOD(Finalize);
META_CLASS_END();

#include "data/Base64OutputStream.hpp"
META_CLASS(Inanity::Data::Base64OutputStream, Inanity.Data.Base64OutputStream);
	META_CLASS_PARENT(Inanity::OutputStream);
	META_CONSTRUCTOR(bool, ptr<OutputStream>);
	META_STATIC_METHOD(EncodeFile);
META_CLASS_END();
