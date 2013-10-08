#ifndef ___INANITY_INANITY_BASE_META_IPP___
#define ___INANITY_INANITY_BASE_META_IPP___

#include "RefCounted.hpp"
META_CLASS(Inanity::RefCounted, Inanity.RefCounted);
META_CLASS_END();

#include "File.hpp"
META_CLASS(Inanity::File, Inanity.File);
	META_METHOD(GetSize);
META_CLASS_END();

#include "FileSystem.hpp"
META_CLASS(Inanity::FileSystem, Inanity.FileSystem);
	META_METHOD(LoadFile);
	META_METHOD(TryLoadFile);
	META_METHOD(LoadStream);
	META_METHOD(SaveFile);
	META_METHOD(SaveStream);
META_CLASS_END();

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

#include "Exception.hpp"
META_CLASS(Inanity::Exception, Inanity.Exception);
META_CLASS_END();

#include "InputStream.hpp"
META_CLASS(Inanity::InputStream, Inanity.InputStream);
META_CLASS_END();

#include "OutputStream.hpp"
META_CLASS(Inanity::OutputStream, Inanity.OutputStream);
META_CLASS_END();

#include "Strings.hpp"
META_CLASS(Inanity::Strings, Inanity.Strings);
	META_STATIC_METHOD(String2File);
	META_STATIC_METHOD(File2String);
META_CLASS_END();

#endif
