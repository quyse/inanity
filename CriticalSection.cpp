#include "CriticalSection.hpp"

CriticalSection::CriticalSection()
{
	InitializeCriticalSection(&criticalSection);
}

CriticalSection::~CriticalSection()
{
	DeleteCriticalSection(&criticalSection);
}

void CriticalSection::Enter()
{
	EnterCriticalSection(&criticalSection);
}

void CriticalSection::Leave()
{
	LeaveCriticalSection(&criticalSection);
}
