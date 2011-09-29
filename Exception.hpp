#ifndef ___INANITY_EXCEPTION_HPP___
#define ___INANITY_EXCEPTION_HPP___

#include "Object.hpp"
#include <string>
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
	std::wstring message;
	ptr<Exception> innerException;

public:
	/// Создать первичный объект исключения.
	/** Первичный объект исключения является первым исключением,
	возникающем при ошибке.
	\param message Текстовое сообщение об ошибке.
	*/
	Exception(const std::wstring& message);

	/// Создать вторичный объект исключения.
	/** Вторичный объект исключения является исключением,
	обертывающим более низкоуровневое исключение,
	и объясняющим с более глобальной точки зрения,
	что означает данная ошибка.
	\param message Текстовое сообщение об ошибке.
	\param innerException Исключение более низкого уровня,
	породившее данное исключение.
	*/
	Exception(const std::wstring& message, ptr<Exception> innerException);

	/// Получить текст сообщения об ошибке.
	/** Данный текст включает только сообщение, которое
	относится именно к этому уровню ошибки.
	\return Текст сообщения об ошибке.
	*/
	std::wstring GetMessageText() const;

	/// Получить внутреннее (низкоуровневое) исключение
	/** \return Объект внутреннего исключения, или null,
	если данное исключение первичное.
	*/
	ptr<Exception> GetInnerException() const;

	/// Вывести в поток вывода сообщения всего стека исключений.
	/** Позволяет получить полную картину произошедшей ошибки,
	выводя всю информацию по указанной ошибке.
	*/
	virtual void PrintStack(std::wostream& stream) const;
};

#ifdef _DEBUG
//несколько макросов, для того, чтобы определить Unicode-версии __FILE__ и __LINE__
#define INANITY_EXCEPTION_WIDEN2(x) L##x
#define INANITY_EXCEPTION_WIDEN(x) INANITY_EXCEPTION_WIDEN2(x)
#define __WFILE__ INANITY_EXCEPTION_WIDEN(__FILE__)
#define INANITY_EXCEPTION_WIDENS2(x) L#x
#define INANITY_EXCEPTION_WIDENS(x) INANITY_EXCEPTION_WIDENS2(x)
#define __WLINE__ INANITY_EXCEPTION_WIDENS(__LINE__)
#endif

#ifdef _DEBUG
/// Макрос для вызова первичного исключения
#define THROW_PRIMARY_EXCEPTION(message) throw NEW(Exception(std::wstring(L"[ " __WFILE__ L", " __WLINE__ L" ] ") + (message)))
/// Макрос для вызова вторичного исключения
#define THROW_SECONDARY_EXCEPTION(message, exception) throw NEW(Exception(std::wstring(L"[ " __WFILE__ L", " __WLINE__ L" ] ") + (message), (exception)))
#else
/// Макрос для вызова первичного исключения
#define THROW_PRIMARY_EXCEPTION(message) throw NEW(Exception((message)))
/// Макрос для вызова вторичного исключения
#define THROW_SECONDARY_EXCEPTION(message, exception) throw NEW(Exception((message), (exception)))
#endif

END_INANITY

#endif
