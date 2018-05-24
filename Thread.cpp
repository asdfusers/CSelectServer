#include "stdafx.h"
#include "Thread.h"


unsigned CThread::__threadMain(void* arg)
{
	CThread* self = (CThread*)arg;
	self->threadMain();
	return 0;
}

CThread::CThread()
{

}


CThread::~CThread()
{
}

void CThread::run()
{
	hThread = (HANDLE)_beginthreadex(NULL, NULL, __threadMain, (void*)this, NULL, NULL);

	CloseHandle(hThread);
}
