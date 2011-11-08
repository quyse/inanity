#include "CriticalCode.hpp"

CriticalCode::CriticalCode(CriticalSection& criticalSection) : criticalSection(criticalSection)
{
	criticalSection.Enter();
}

CriticalCode::~CriticalCode()
{
	criticalSection.Leave();
}
