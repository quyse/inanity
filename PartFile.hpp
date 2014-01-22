#ifndef ___PART_FILE_H___
#define ___PART_FILE_H___

#include "File.hpp"

BEGIN_INANITY

/// Класс частичного файла.
/** Предоставляет возможность представлять часть существующего файла как
отдельный объект File.*/
class PartFile : public File
{
private:
	ptr<File> file;
	void* data;
	size_t size;

public:
	/// Создать частичный файл.
	/** Создает частичный файл.
	\param file Исходный файл. Передается только для того, чтобы
	PartFile удерживал ссылку на него, и таким образом предотвратил
	его уничтожение раньше времени. Разрешается передавать сюда
	null, чтобы не удерживать никакую ссылку; это также полезно для
	представления памяти как файла без передачи владения памятью.
	\param data Указатель на начало буфера памяти в файле.
	\param size Размер буфера памяти в байтах.
	*/
	PartFile(ptr<File> file, void* data, size_t size);
	PartFile(ptr<File> file, size_t offset, size_t size);

	void* GetData() const;
	size_t GetSize() const;
};

END_INANITY

#endif
