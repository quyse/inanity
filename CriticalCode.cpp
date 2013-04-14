#include "CriticalCode.hpp"

BEGIN_INANITY

CriticalCode::CriticalCode(CriticalSection& criticalSection) : criticalSection(criticalSection)
{
	criticalSection.Enter();
}

CriticalCode::~CriticalCode()
{
	criticalSection.Leave();
}

END_INANITY
