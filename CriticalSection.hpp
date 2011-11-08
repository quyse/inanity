#ifndef ___INANITY_CRITICAL_SECTION_H___
#define ___INANITY_CRITICAL_SECTION_H___

#include "Object.hpp"
#include "windows.hpp"

BEGIN_INANITY

//класс критической секции
//инкапсулирует обычную CRITICAL_SECTION
class CriticalSection
{
private:
	CRITICAL_SECTION criticalSection;

public:
	CriticalSection();
	~CriticalSection();

	void Enter();
	void Leave();
};

END_INANITY

#endif
