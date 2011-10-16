#ifndef ___INANITY_FILE_HPP___
#define ___INANITY_FILE_HPP___

#include "Object.hpp"
#include "scripting.hpp"

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

#ifdef ___INANITY_SCRIPTING

	SCRIPTABLE_CLASS();
	SCRIPTABLE_CALL(getSize);

#endif
};

END_INANITY

#endif
