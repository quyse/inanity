#ifndef ___INANITY_ARCHI_PROCESSOR_HPP___
#define ___INANITY_ARCHI_PROCESSOR_HPP___

#include "general.hpp"

/// Абстрактный класс процессора - объекта, олицетворяющего команду.
class Processor : public Object
{
public:
	/// Получить команду.
	virtual String GetCommand() const = 0;
	/// Напечатать справку по команде.
	virtual void PrintHelp() const = 0;
	/// Начать выполнение команды.
	virtual void Run(const std::vector<String>& arguments) = 0;
};

#endif
