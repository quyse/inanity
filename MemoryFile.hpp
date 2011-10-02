#ifndef ___INANITY_MEMORY_FILE_HPP___
#define ___INANITY_MEMORY_FILE_HPP___

#include "File.hpp"

BEGIN_INANITY

/// Класс файла в памяти.
/** Данный класс просто предоставляет возможность представить область памяти,
выделенную из кучи, как объект File, а также автоматически освобождать её.*/
class MemoryFile : public File
{
private:
	void* data;
	size_t size;

public:
	/// Создать файл в памяти.
	/** Создает файл в памяти заданного размера, самостоятельно
	выделяя память из кучи.
	\param size Требуемый размер файла в байтах. Случай с размером 0
	корректно обрабатывается.
	*/
	MemoryFile(size_t size);

	/// Создать файл в памяти.
	/** Создает файл в памяти, используя переданную память и её размер.
	Передаваемая память переходит во владение объекта, то есть будет освобождена
	деструктором. Память должна быть выделена оператором new unsigned char[].
	Использование этого конструктора не рекомендуется.
	\param data Указатель на начало буфера памяти.
	\param size Размер буфера памяти в байтах.
	*/
	MemoryFile(void* data, size_t size);

	~MemoryFile();

	static ptr<MemoryFile> CreateViaCopy(const void* data, size_t size);

	void* GetData() const;
	size_t GetSize() const;
};

END_INANITY

#endif
