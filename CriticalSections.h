#pragma once
class CriticalSections
{
private:
	CRITICAL_SECTION m_CS;

public:

	static CriticalSections* getInstance()
	{
		if (selfIns == 0)
			selfIns = new CriticalSections();

		return selfIns;
	}

	CriticalSections() { InitializeCriticalSection(&m_CS); }
	virtual ~CriticalSections() { DeleteCriticalSection(&m_CS); }
	
	void enter() { EnterCriticalSection(&m_CS); }
	void leave() { LeaveCriticalSection(&m_CS); }
	void releaseInstance();

private:
	static CriticalSections* selfIns;
	CriticalSections(const CriticalSections&) {}
	const CriticalSections& operator = (const CriticalSections&) { return *this; }
};

