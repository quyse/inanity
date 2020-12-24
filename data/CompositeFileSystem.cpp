#include "CompositeFileSystem.hpp"
#include "../File.hpp"
#include "../InputStream.hpp"
#include "../OutputStream.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_DATA

template <typename F>
auto CompositeFileSystem::WithPath(const String& path, F&& f) const -> decltype(f(ptr<FileSystem>(), String()))
{
	// iterate through filesystems
	for(size_t i = 0; i < fileSystems.size(); ++i)
	{
		// check by prefix
		const String& prefix = fileSystems[i].first;
		if(path.compare(0, prefix.length(), prefix, 0, prefix.length()) == 0)
		{
			// cut prefix off
			String subPath = path.substr(prefix.length() - 1);
			// try to apply function
			try
			{
				auto r = f(fileSystems[i].second, subPath);
				if(r) return r;
			}
			catch(Exception* exception)
			{
				MakePointer(exception);
			}
		}
	}
	// return nullptr, false, etc
	return decltype(f(ptr<FileSystem>(), String()))();
}

void CompositeFileSystem::NormalizePath(String& path)
{
	// add final slash if needed
	if(!path.length() || path.back() != '/')
		path += '/';
}

void CompositeFileSystem::Mount(ptr<FileSystem> fileSystem, String path)
{
	NormalizePath(path);
	fileSystems.push_back(std::make_pair(path, fileSystem));
}

void CompositeFileSystem::Remount(ptr<FileSystem> fileSystem, String path)
{
	NormalizePath(path);
	// try to find and replace filesystem
	for(size_t i = 0; i < fileSystems.size(); ++i)
	{
		if(fileSystems[i].first == path)
		{
			fileSystems[i].second = fileSystem;
			return;
		}
	}

	// otherwise append
	fileSystems.push_back(std::make_pair(path, fileSystem));
}

ptr<File> CompositeFileSystem::TryLoadFile(const String& fileName)
{
	return WithPath(fileName, [&](ptr<FileSystem> fs, const String& path)
	{
		return fs->TryLoadFile(path);
	});
}

ptr<Storage> CompositeFileSystem::LoadStorage(const String& fileName)
{
	ptr<Storage> storage = WithPath(fileName, [&](ptr<FileSystem> fs, const String& path)
	{
		return fs->LoadStorage(path);
	});
	if(!storage)
		THROW("Can't load storage " + fileName);
	return storage;
}

ptr<InputStream> CompositeFileSystem::LoadStream(const String& fileName)
{
	ptr<InputStream> inputStream = WithPath(fileName, [&](ptr<FileSystem> fs, const String& path)
	{
		return fs->LoadStream(path);
	});
	if(!inputStream)
		THROW("Can't load stream " + fileName);
	return inputStream;
}

void CompositeFileSystem::SaveFile(ptr<File> file, const String& fileName)
{
	bool ok = WithPath(fileName, [&](ptr<FileSystem> fs, const String& path)
	{
		fs->SaveFile(file, path);
		return true;
	});
	if(!ok)
		THROW("Can't save file " + fileName);
}

ptr<OutputStream> CompositeFileSystem::SaveStream(const String& fileName)
{
	ptr<OutputStream> outputStream = WithPath(fileName, [&](ptr<FileSystem> fs, const String& path)
	{
		return fs->SaveStream(path);
	});
	if(!outputStream)
		THROW("Can't save stream " + fileName);
	return outputStream;
}

time_t CompositeFileSystem::GetFileMTime(const String& fileName)
{
	time_t mtime;
	bool ok = WithPath(fileName, [&](ptr<FileSystem> fs, const String& path)
	{
		mtime = fs->GetFileMTime(path);
		return true;
	});
	if(!ok)
		THROW("Can't get file mtime " + fileName);
	return mtime;
}

void CompositeFileSystem::GetDirectoryEntries(const String& directoryName, std::vector<String>& entries) const
{
	bool ok = WithPath(directoryName, [&](ptr<FileSystem> fs, const String& path)
	{
		entries.clear();
		fs->GetDirectoryEntries(path, entries);
		return true;
	});
	if(!ok)
		THROW("Can't get directory entries");
}

void CompositeFileSystem::MakeDirectory(const String& directoryName)
{
	bool ok = WithPath(directoryName, [&](ptr<FileSystem> fs, const String& path)
	{
		fs->MakeDirectory(path);
		return true;
	});
	if(!ok)
		THROW("Can't make directory " + directoryName);
}

void CompositeFileSystem::DeleteEntry(const String& entryName)
{
	bool ok = WithPath(entryName, [&](ptr<FileSystem> fs, const String& path)
	{
		fs->DeleteEntry(path);
		return true;
	});
	if(!ok)
		THROW("Can't delete entry " + entryName);
}

FileSystem::EntryType CompositeFileSystem::GetEntryType(const String& entryName) const
{
	EntryType entryType;
	bool ok = WithPath(entryName, [&](ptr<FileSystem> fs, const String& path)
	{
		entryType = fs->GetEntryType(path);
		return entryType != entryTypeMissing;
	});
	return ok ? entryType : entryTypeMissing;
}

END_INANITY_DATA
