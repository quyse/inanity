#ifndef ___INANITY_HANDLER_HPP___
#define ___INANITY_HANDLER_HPP___

#include "Object.hpp"
#include "Exception.hpp"

BEGIN_INANITY

template <typename T, typename Class>
class Delegate;

/// Класс обработчика, который просто принимает сообщение.
class Handler : public Object
{
public:
	virtual void Fire() = 0;

private:
	/// Класс делегата.
	/** Для упрощения привязки методов. */
	template <typename Class>
	class Delegate;
	/// Класс делегата для вызова объекта.
	template <typename Class>
	class CallDelegate;

public:
	/// Привязать класс с методом.
	template <typename Class>
	static ptr<Handler> Bind(ptr<Class> object, typename Delegate<Class>::Method method)
	{
		return NEW(Delegate<Class>(object, method));
	}
	/// Привязать вызываемый объект.
	template <typename T>
	static ptr<Handler> BindCall(T&& object)
	{
		return NEW(CallDelegate<T>(object));
	}
};

template <typename Class>
class Handler::Delegate : public Handler
{
public:
	typedef void (Class::*Method)();

private:
	ptr<Class> object;
	Method method;

public:
	Delegate(ptr<Class> object, Method method)
	: object(object), method(method) {}

	void Fire()
	{
		(object->*method)();
	}
};

template <typename T>
class Handler::CallDelegate : public Handler
{
private:
	T object;

public:
	CallDelegate(T object)
	: object(object) {}
	CallDelegate(T&& object)
	: object(object) {}

	void Fire()
	{
		object();
	}
};

/// Класс обработчика, который может обрабатывать ошибки.
class ErrorHandler : public Object
{
protected:
	/// Обработать исключение.
	virtual void OnError(ptr<Exception> exception) = 0;

public:
	/// Передать ошибку.
	void FireError(ptr<Exception> exception)
	{
		OnError(exception);
	}
};

/// Абстрактный класс обработчика без параметров.
class SuccessHandler : public ErrorHandler
{
protected:
	virtual void OnSuccess() = 0;

public:
	void FireSuccess()
	{
		OnSuccess();
	}
};

/// Абстрактный класс обработчика с одним параметром.
template <typename T>
class DataHandler : public ErrorHandler
{
public:
	/// Класс результата.
	/** Позволяет получить данные, если они есть, или настоящее исключение. */
	class Result
	{
	public:
		virtual T GetData() const = 0;
	};

private:
	class DataResult : public Result
	{
	private:
		T data;

	public:
		DataResult(T data) : data(data) {}

		T GetData() const { return data; }
	};
	class ErrorResult : public Result
	{
	private:
		ptr<Exception> exception;

	public:
		ErrorResult(ptr<Exception> exception) : exception(exception) {}

		T GetData() const
		{
			THROW_SECONDARY("Async exception", exception);
		}
	};

private:
	/// Класс делегата.
	/** Для упрощения привязки методов. */
	template <typename Class>
	class Delegate;

protected:
	/// Обработать данные.
	virtual void OnData(T data) = 0;

public:
	/// Передать данные.
	void FireData(T data)
	{
		OnData(data);
	}

	/// Привязать класс с методом.
	template <typename Class>
	static ptr<DataHandler> Bind(ptr<Class> object, typename Delegate<Class>::Method method)
	{
		return NEW(Delegate<Class>(object, method));
	}
};

template <typename T>
template <typename Class>
class DataHandler<T>::Delegate : public DataHandler
{
public:
	typedef void (Class::*Method)(const Result& result);

private:
	ptr<Class> object;
	Method method;

public:
	Delegate(ptr<Class> object, Method method)
	: object(object), method(method) {}

	void OnError(ptr<Exception> exception)
	{
		(object->*method)(ErrorResult(exception));
	}

	void OnData(T data)
	{
		(object->*method)(DataResult(data));
	}
};

/// Абстрактный класс обработчика с двумя параметрами.
template <typename T1, typename T2>
class Handler2 : public Object
{
protected:
	/// Обработать событие.
	/** Собственно, метод, который нужно перегружать в производном классе. */
	virtual void OnEvent(T1 data1, T2 data2) = 0;

public:
	/// Сообщить о наступлении события.
	void Fire(T1 data1, T2 data2)
	{
		OnEvent(data1, data2);
	}
};

END_INANITY

#endif
