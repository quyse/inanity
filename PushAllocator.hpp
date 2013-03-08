#ifndef ___INANITY_PUSH_ALLOCATOR_HPP___
#define ___INANITY_PUSH_ALLOCATOR_HPP___

#include "MemoryFile.hpp"
#include <vector>

BEGIN_INANITY

/// Класс расширяющегося аллокатора памяти для объектов.
/** Он может только создавать новые объекты, или уничтожить все,
 * когда уничтожается. Объекты, конечно, не могут быть управляемыми,
 * так как они не лежат в управляемой куче.
 * Класс несколько эффективней вектора, так как не производит перевыделения
 * памяти, и копирования объектов на новое место.
 * Кроме того, он обладает преимуществом - указатели на выданные объекты остаются
 * валидными, пока аллокатор жив.
 * Аллокатор эффективно выделяет память, выполняя логарифм выделений,
 * и в любой момент не превышая объём необходимой памяти более чем в два раза.
 * Объекты должны иметь конструктор по умолчанию. */
template <typename T>
class PushAllocator : public Object
{
private:
	/// Файлы, в которых выделена память.
	std::vector<ptr<MemoryFile> > files;
	/// Общее количество инициализированных объектов.
	size_t objectsCount;
	/// Размер последнего файла в объектах.
	size_t lastFileAllCount;
	/// Сколько объектов выделено в последнем файле.
	size_t lastFileAllocatedCount;
	/// Память последнего файла.
	T* lastFileData;

public:
	/// Создать аллокатор.
	PushAllocator() : objectsCount(0), lastFileAllCount(0), lastFileAllocatedCount(0), lastFileData(0) {}
	/// Уничтожить аллокатор вместе со всеми объектами.
	~PushAllocator()
	{
		for(size_t i = 0; i < files.size(); ++i)
		{
			ptr<MemoryFile> file = files[i];
			T* objects = (T*)file->GetData();
			size_t fileObjectsCount = file->GetSize() / sizeof(T);
			for(size_t j = 0; j < fileObjectsCount && objectsCount > 0; ++j, --objectsCount)
				objects[j].~T();
		}
	}

	/// Создать объект.
	T* Allocate()
	{
		// если в последнем файле больше нет места
		if(lastFileAllocatedCount >= lastFileAllCount)
		{
			// создать новый файл
			lastFileAllCount = 1 << files.size();
			ptr<MemoryFile> newFile = NEW(MemoryFile(lastFileAllCount * sizeof(T)));
			files.push_back(newFile);
			lastFileAllocatedCount = 0;
			lastFileData = (T*)newFile->GetData();
		}
		// выделить память, и вызвать конструктор
		return new (lastFileData + lastFileAllocatedCount++) T();
	}
};

END_INANITY

#endif
