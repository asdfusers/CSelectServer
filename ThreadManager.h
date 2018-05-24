#pragma once
#include <list>
class CThreadManager
{
public:
	static CThreadManager* getInstance()
	{
		if (selfIns == 0)
			selfIns = new CThreadManager();

		return selfIns;
	}
	static	void			releaseInstance();

	void join();
	HANDLE spawn(LPTHREAD_START_ROUTINE func, void* arg, DWORD* threadID);

public:
	CThreadManager();
	~CThreadManager();

private:

	static CThreadManager* selfIns;
	std::list<HANDLE> listThreadHandle;
	typedef std::list<HANDLE>::iterator ListPosition;
};

