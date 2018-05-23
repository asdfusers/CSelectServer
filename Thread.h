#pragma once
#ifndef THREAD_H
#define THREAD_H

#include "stdafx.h"

class CThread
{
protected:
	virtual void threadMain() = 0;

	static unsigned WINAPI __threadMain(void* arg);
public:
	CThread();
	~CThread();
	void run();


private:
	HANDLE hThread;
};
#endif

