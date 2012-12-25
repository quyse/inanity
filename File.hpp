#ifndef ___INANITY_FILE_HPP___
#define ___INANITY_FILE_HPP___

#include "Object.hpp"
#include "scripting_decl.hpp"

BEGIN_INANITY

/// Абстрактный класс файла.
/** Данный класс предоставляет абстракцию файла - области памяти
в адресном пространстве процесса. */
class File : public Object
{
public:
	/// Получает указатель на данные файла.
	/** Возвращаемый указатель действителен, пока сам объект File
	существует.
	\return Указатель на начало области в адресном пространстве.
	*/
	virtual void* GetData() const = 0;

	/// Получает размер файла.
	/** Размер файла не может меняться за все время существования
	файла.
	\return Размер файла в байтах.
	*/
	virtual size_t GetSize() const = 0;

	SCRIPTABLE_CLASS(File);
};

END_INANITY

#endif
