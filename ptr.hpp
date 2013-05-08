#ifndef ___INANITY_POINTER_HPP___
#define ___INANITY_POINTER_HPP___

#include "config.hpp"
#include <functional>

BEGIN_INANITY

#if defined(_DEBUG) && defined(_M_IX86)
#define CHECK_OBJECT() \
	if(!object) \
		__asm int 3;
#else
#define CHECK_OBJECT()
#endif

/// Класс указателя на управляемый объект.
template <typename T>
class ptr
{
private:
	/// Указатель на управляемый объект.
	T* object;

public:
	/// Конструктор.
	inline ptr(const ptr<T>& p)
	{
		object = p.object;
		if(object) object->Reference();
	}

	/// Конструктор, создающий указатель из другого указателя.
	template <typename TT>
	inline ptr(const ptr<TT>& p)
	{
		object = &*p;
		if(object) object->Reference();
	}

	/// Конструктор, создающий указатель из неуправляемого указателя.
	/// Автоматически обрабатываются подчиненные типы.
	inline ptr(T* p = 0)
	{
		object = p;
		if(object) object->Reference();
	}

	/// Деструктор.
	inline ~ptr()
	{
		if(object) object->Dereference();
	}

	/// Оператор присваивания указателя.
	inline void operator = (const ptr<T>& p)
	{
		if(object) object->Dereference();
		object = p.object;
		if(object) object->Reference();
	}
	/// Оператор присваивания указателя совместимого типа.
	template <typename TT>
	inline void operator = (const ptr<TT>& p)
	{
		if(object) object->Dereference();
		object = p;
		if(object) object->Reference();
	}

	/// Оператор разыменования указателя.
	inline T& operator*() const
	{
		CHECK_OBJECT();
		return *object;
	}

	/// Оператор стрелки.
	inline T* operator->() const
	{
		CHECK_OBJECT();
		return object;
	}

	/// Оператор для неявного преобразования в указатель.
	inline operator T*() const
	{
		return object;
	}

	/// Оператор для проверки указателя на валидность.
	inline operator bool() const
	{
		return !!object;
	}

	/// Оператор для сравнения указателей.
	inline friend bool operator<(const ptr<T>& a, const ptr<T>& b)
	{
		return a.object < b.object;
	}

	/// Еще один оператор для сравнения указателей.
	inline friend bool operator==(const ptr<T>& a, const ptr<T>& b)
	{
		return a.object == b.object;
	}

	/// И еще один оператор для сравнения указателей.
	inline friend bool operator!=(const ptr<T>& a, const ptr<T>& b)
	{
		return a.object != b.object;
	}

	/// Оператор для статического преобразования типа.
	template <typename TT>
	ptr<TT> StaticCast() const
	{
		return static_cast<TT*>(object);
	}

	/// Оператор для динамического преобразования типа.
	template <typename TT>
	ptr<TT> DynamicCast() const
	{
		return dynamic_cast<TT*>(object);
	}

	/// Оператор для быстрого преобразования типа.
	/** В отладочном режиме он всё проверяет, в релизе - нет. */
	template <typename TT>
	ptr<TT> FastCast() const
	{
#ifdef _DEBUG
		return dynamic_cast<TT*>(object);
#else
		return static_cast<TT*>(object);
#endif
	}
};

/// Специальная функция для явного создания управляемого указателя
template <typename T>
inline ptr<T> MakePointer(T* p)
{
	return p;
}

#undef CHECK_OBJECT

END_INANITY

// Оператор для хеширования указателей.
namespace std
{
	template <typename T>
	struct hash<Inanity::ptr<T> >
	{
		size_t operator()(const Inanity::ptr<T>& p) const
		{
			return reinterpret_cast<size_t>(static_cast<T*>(p));
		}
	};
}

#endif
