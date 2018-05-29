#include "stdafx.h"
#include "ThreadManager.h"

CThreadManager*	CThreadManager::selfIns = 0;

void CThreadManager::releaseInstance()
{
	if (selfIns != 0)
	{
		delete selfIns;
		selfIns = 0;
	}
}

void CThreadManager::join()
{
	ListPosition pos, posPrev;
	pos = listThreadHandle.begin();

	while (pos != listThreadHandle.end())
	{

		posPrev = pos++;

		WaitForSingleObject(*posPrev, INFINITE);
		listThreadHandle.erase(posPrev);
	}
}

HANDLE CThreadManager::spawn(LPTHREAD_START_ROUTINE func, void * arg, DWORD*  threadID)
{
	HANDLE hThread;
	hThread = CreateThread(NULL, NULL, func, arg, 0, threadID);
	listThreadHandle.push_back(hThread);

	return hThread;
}

CThreadManager::CThreadManager()
{
	
}


CThreadManager::~CThreadManager()
{
	ListPosition pos;

	while (listThreadHandle.size() > 0)
		listThreadHandle.erase(listThreadHandle.begin());
}
