#pragma once
#ifndef THREAD_H
#define THREAD_H

#include "stdafx.h"

class CThread
{
protected:
	virtual void threadMain() = 0;
	static DWORD WINAPI __threadMain(LPVOID arg);

public:
	void begin();
	DWORD getThreadID();

private:
	HANDLE hThread;
	bool isStarted;
	DWORD threadID;
};
#endif

