#ifndef ___INANITY_INANITY_BASE_META_IPP___
#define ___INANITY_INANITY_BASE_META_IPP___

#include "RefCounted.hpp"
META_CLASS(Inanity::RefCounted, Inanity.RefCounted);
META_CLASS_END();

#include "File.hpp"
META_CLASS(Inanity::File, Inanity.File);
	META_METHOD(GetSize);
	META_METHOD(Slice);
	META_METHOD(SliceFrom);
META_CLASS_END();

#include "FileSystem.hpp"
META_CLASS(Inanity::FileSystem, Inanity.FileSystem);
	META_METHOD(LoadFile);
	META_METHOD(TryLoadFile);
	META_METHOD(LoadStream);
	META_METHOD(SaveFile);
	META_METHOD(SaveStream);
META_CLASS_END();

#include "Exception.hpp"
META_CLASS(Inanity::Exception, Inanity.Exception);
META_CLASS_END();

#include "InputStream.hpp"
META_CLASS(Inanity::InputStream, Inanity.InputStream);
	META_METHOD_FULL(Read, ptr<File> (InputStream::*)(size_t), Read);
META_CLASS_END();

#include "OutputStream.hpp"
META_CLASS(Inanity::OutputStream, Inanity.OutputStream);
	META_METHOD_FULL(Write, void (OutputStream::*)(ptr<File>), Write);
	META_METHOD(ReadAllFromStream);
META_CLASS_END();

#include "Strings.hpp"
META_CLASS(Inanity::Strings, Inanity.Strings);
	META_STATIC_METHOD(String2File);
	META_STATIC_METHOD(File2String);
META_CLASS_END();

#include "MemoryFile.hpp"
META_CLASS(Inanity::MemoryFile, Inanity.MemoryFile);
	META_CLASS_PARENT(Inanity::File);
	META_CONSTRUCTOR(size_t);
	META_STATIC_METHOD_FULL(CreateViaCopy, ptr<MemoryFile> (*)(ptr<File>), CreateViaCopy);
META_CLASS_END();

#include "MemoryStream.hpp"
META_CLASS(Inanity::MemoryStream, Inanity.MemoryStream);
	META_CLASS_PARENT(Inanity::OutputStream);
	META_CONSTRUCTOR();
	META_METHOD(ToFile);
META_CLASS_END();

#endif
