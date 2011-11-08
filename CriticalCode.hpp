#ifndef ___INANITY_CRITICAL_CODE_H___
#define ___INANITY_CRITICAL_CODE_H___

#include "CriticalSection.hpp"

BEGIN_INANITY

//класс критического кода
//следует вставлять в местах, где требуется синхронизация
class CriticalCode
{
private:
	CriticalSection& criticalSection;

public:
	CriticalCode(CriticalSection& criticalSection);
	~CriticalCode();
};

END_INANITY

#endif
