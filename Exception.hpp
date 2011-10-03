#ifndef ___INANITY_EXCEPTION_HPP___
#define ___INANITY_EXCEPTION_HPP___

#include "Object.hpp"
#include "String.hpp"
#include <ostream>

BEGIN_INANITY

/// Базовый класс исключения.
/** Данный класс используется везде, для предоставления информации
о происходящих ошибках.
Происходящяя ошибка должна обрабатываться соответствующим кодом,
который переводит её в первичное исключение Inanity.
Это первичное исключение может быть перехвачено кодом, решающим
более высокоуровневую задачу; он должен в свою очередь создать
вторичное исключение, и описать в нем, какая ошибка произошла
с его точки зрения. Последующие уровни кода также могут создавать
вторичные исключения, каждый раз описывая проблему все более и
более глобально. Такая схема позволяет легко понять, почему
ошибка произошла, и что сделать для её устранения, а также
легко контролировать возникновение ошибок.
Сообщение, которое задаётся для исключения, должно быть одним
предложением, с большой буквы, без завершающей точки или перевода
строки.
*/
class Exception : public Object
{
protected:
	String message;
	ptr<Exception> innerException;

public:
	/// Создать первичный объект исключения.
	/** Первичный объект исключения является первым исключением,
	возникающем при ошибке.
	\param message Текстовое сообщение об ошибке.
	*/
	Exception(const String& message);

	/// Создать вторичный объект исключения.
	/** Вторичный объект исключения является исключением,
	обертывающим более низкоуровневое исключение,
	и объясняющим с более глобальной точки зрения,
	что означает данная ошибка.
	\param message Текстовое сообщение об ошибке.
	\param innerException Исключение более низкого уровня,
	породившее данное исключение.
	*/
	Exception(const String& message, ptr<Exception> innerException);

	/// Получить текст сообщения об ошибке.
	/** Данный текст включает только сообщение, которое
	относится именно к этому уровню ошибки.
	\return Текст сообщения об ошибке.
	*/
	String GetMessageText() const;

	/// Получить внутреннее (низкоуровневое) исключение
	/** \return Объект внутреннего исключения, или null,
	если данное исключение первичное.
	*/
	ptr<Exception> GetInnerException() const;

	/// Вывести в поток вывода сообщения всего стека исключений.
	/** Позволяет получить полную картину произошедшей ошибки,
	выводя всю информацию по указанной ошибке.
	*/
	virtual void PrintStack(std::ostream& stream) const;

	/// Создать исключение, описывающее ошибку системы.
	/** Ошибка системы определяется специфичным для платформы способом (Windows - GetLastError, Linux - errno). */
	static ptr<Exception> SystemError();
	static ptr<Exception> SystemError(int errorCode);
};

#ifdef _DEBUG
#define __SLINE2__(x) #x
#define __SLINE3__(x) __SLINE2__(x)
#define __SLINE__ __SLINE3__(__LINE__)
/// Макрос для вызова первичного исключения
#define THROW_PRIMARY_EXCEPTION(message) throw NEW(Exception(String("[ " __FILE__ ", " __SLINE__ " ] ") + (message)))
/// Макрос для вызова вторичного исключения
#define THROW_SECONDARY_EXCEPTION(message, exception) throw NEW(Exception(String("[ " __FILE__ ", " __SLINE__ " ] ") + (message), (exception)))
#else
/// Макрос для вызова первичного исключения
#define THROW_PRIMARY_EXCEPTION(message) throw NEW(Exception((message)))
/// Макрос для вызова вторичного исключения
#define THROW_SECONDARY_EXCEPTION(message, exception) throw NEW(Exception((message), (exception)))
#endif

END_INANITY

#endif
