#include "stdafx.h"
#include "Thread.h"
#include "ThreadManager.h"

DWORD CThread::__threadMain(LPVOID arg)
{
	CThread* self = (CThread*)arg;
	self->threadMain();
	return 0;
}


void CThread::begin()
{
	if (isStarted == true)
		return;

	hThread = CThreadManager::getInstance()->spawn(__threadMain, this, &threadID);
	isStarted = true;
}

DWORD CThread::getThreadID()
{
	return 0;
}
