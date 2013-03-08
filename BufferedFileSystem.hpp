#ifndef ___INANITY_BUFFERED_FILE_SYSTEM_HPP___
#define ___INANITY_BUFFERED_FILE_SYSTEM_HPP___

#include "FilterFileSystem.hpp"

BEGIN_INANITY

/// Класс буферизованной файловой системы.
/** Оборачивает потоки файловой системы буферизирующими потоками.
Файлы никак не оборачиваются.
Может использоваться для улучшения производительности нативных
файловых систем. */
class BufferedFileSystem : public FilterFileSystem
{
public:
	BufferedFileSystem(ptr<FileSystem> fileSystem);

	ptr<InputStream> LoadStream(const String& fileName);
	ptr<OutputStream> SaveStream(const String& fileName);
};

END_INANITY

#endif
