#ifndef ___INANITY_HANDLE_HPP___
#define ___INANITY_HANDLE_HPP___

/*
Файл содержит определение класса, инкапсулирующего Windows-дескрипторы (в Windows) или файловые дескрипторы (в Linux).
*/

#include "Object.hpp"
#ifdef ___INANITY_WINDOWS
#include "windows.hpp"
#endif // ___INANITY_WINDOWS

BEGIN_INANITY

#ifdef ___INANITY_WINDOWS

class Handle : public Object
{
private:
	HANDLE handle;

public:
	Handle(HANDLE handle = 0);
	~Handle();

	void operator=(HANDLE handle);
	bool IsValid() const;
	operator HANDLE();
};

#endif // ___INANITY_WINDOWS

#ifdef ___INANITY_LINUX

class Handle : public Object
{
private:
	int fd;

public:
	Handle(int fd = -1);
	~Handle();

	void operator=(int fd);
	bool IsValid() const;
	operator int();
};

#endif // ___INANITY_LINUX

END_INANITY

#endif
