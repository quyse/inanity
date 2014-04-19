#ifndef ___INANITY_MEMORY_STREAM_HPP___
#define ___INANITY_MEMORY_STREAM_HPP___

#include "OutputStream.hpp"
#include <vector>

BEGIN_INANITY

class File;

/// Класс потока в памяти.
/** Представляет собой расширяемый файл в памяти с интерфейсом потока. В любой момент выделено памяти
не более, чем в два раза больше, чем размер содержимого. */
class MemoryStream : public OutputStream
{
private:
	/// Общий размер данных.
	size_t dataSize;
	/// Размер данных в последнем файле.
	size_t lastDataSize;
	/// Последовательность файлов, которые если склеить, получится результат.
	std::vector<ptr<File> > files;

	/// Сжать файлы.
	/** То есть склеить их все и заменить одним. */
	void Compact();

public:
	MemoryStream(size_t initialSize = 0x1000);

	void Write(const void* data, size_t size);

	/// Преобразовать в файл.
	ptr<File> ToFile();
};

END_INANITY

#endif
