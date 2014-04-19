#include "MemoryStream.hpp"
#include "MemoryFile.hpp"
#include "PartFile.hpp"
#include <string.h>
#include <algorithm>

BEGIN_INANITY

MemoryStream::MemoryStream(size_t initialSize) : dataSize(0), lastDataSize(0), files(1)
{
	files[0] = NEW(MemoryFile(initialSize));
}

void MemoryStream::Write(const void* data, size_t size)
{
	//цикл копирования
	while(size)
	{
		File* file = files[files.size() - 1];
		size_t lastFileSize = file->GetSize();
		//если в последнем файле есть место
		if(lastDataSize < lastFileSize)
		{
			//скопировать данные
			size_t toCopy = std::min(size, lastFileSize - lastDataSize);
			memcpy((char*)file->GetData() + lastDataSize, data, toCopy);
			data = (char*)data + toCopy;
			size -= toCopy;
			dataSize += toCopy;
			lastDataSize += toCopy;
		}
		//иначе места нет
		else
		{
			//выделить новый файл
			files.push_back(NEW(MemoryFile(dataSize)));
			lastDataSize = 0;
		}
	}
}

void MemoryStream::Compact()
{
	//если файлов больше одного
	if(files.size() > 1)
	{
		//создать файл для всех данных
		ptr<File> file = NEW(MemoryFile(dataSize));
		//скопировать в него данные всех файлов
		char* data = (char*)file->GetData();
		for(size_t i = 0; i < files.size(); ++i)
		{
			File* file = &*files[i];
			size_t size = (i < files.size() - 1) ? file->GetSize() : lastDataSize;
			memcpy(data, file->GetData(), size);
			data += size;
		}
		//заменить все файлы на один
		files.assign(1, file);
		//указать действительные размеры данных
		lastDataSize = dataSize;
	}
}

ptr<File> MemoryStream::ToFile()
{
	Compact();
	return dataSize < files[0]->GetSize() ? NEW(PartFile(files[0], files[0]->GetData(), dataSize)) : files[0];
}

END_INANITY
