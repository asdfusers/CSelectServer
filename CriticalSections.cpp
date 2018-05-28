#include "stdafx.h"
#include "CriticalSections.h"



CriticalSections*	CriticalSections::selfIns = 0;

void CriticalSections::releaseInstance()
{
	if (selfIns != 0)
	{
		delete selfIns;
		selfIns = 0;
	}
}
