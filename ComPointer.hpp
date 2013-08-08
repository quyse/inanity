#ifndef ___INANITY_COM_POINTER_H___
#define ___INANITY_COM_POINTER_H___

#include "config.hpp"

/*
Файл содержит класс указателя на интерфейс объекта COM.
*/

BEGIN_INANITY

#if defined(_DEBUG) && defined(_M_IX86)
#define CHECK_OBJECT() \
	if(!object) \
		__asm int 3;
#else
#define CHECK_OBJECT()
#endif

template <typename T>
class ComPointer
{
private:
	//указатель на интерфейс COM
	T* object;

public:
	inline ComPointer(const ComPointer<T>& p)
	{
		object = p.object;
		if(object) object->AddRef();
	}

	template <typename TT>
	inline ComPointer(const ComPointer<TT>& p)
	{
		object = p;
		if(object) object->AddRef();
	}

	//конструктор, создающий указатель из неуправляемого указателя
	//автоматически обрабатываются подчиненные типы
	//считается, что одна ссылка на него уже есть, поэтому дополнительные
	//ссылки не создаются
	inline ComPointer(T* p = 0)
	{
		object = p;
	}

	//деструктор
	inline ~ComPointer()
	{
		if(object) object->Release();
	}

	//оператор присваивания указателей
	inline void operator = (const ComPointer<T>& p)
	{
		if(object) object->Release();
		object = p.object;
		if(object) object->AddRef();
	}
	//оператор присваивания указателя совместимого типа
	template <typename TT>
	inline void operator = (const ComPointer<TT>& p)
	{
		if(object) object->Release();
		object = p;
		if(object) object->AddRef();
	}

	//оператор стрелки
	inline T* operator->() const
	{
		CHECK_OBJECT();
		return object;
	}

	//оператор для неявного преобразования в указатель
	inline operator T*() const
	{
		return object;
	}

	//оператор для проверки указателя на валидность
	inline operator bool() const
	{
		return !!object;
	}

	/// Address-of operator.
	/** Primarily for using in COM-getting functions. Such functions give a one
	reference automatically, so reference counting will be ok. */
	inline T** operator&()
	{
		return &object;
	}
};

#undef CHECK_OBJECT

END_INANITY

#endif
