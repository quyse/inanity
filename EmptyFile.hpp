#ifndef ___INANITY_EMPTY_FILE_HPP___
#define ___INANITY_EMPTY_FILE_HPP___

#include "File.hpp"

/*
Файл содержит класс пустого файла нулевой длины, который нельзя читать.
*/

BEGIN_INANITY

/// Класс пустого файла.
/** Пригодится в вырожденных случаях, когда нужно выдать файл нулевой длины. */
class EmptyFile : public File
{
public:
	void* GetData() const;
	size_t GetSize() const;
};

END_INANITY

#endif
