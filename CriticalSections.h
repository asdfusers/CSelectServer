#pragma once
class CriticalSections
{
private:
	CRITICAL_SECTION m_CS;

public:

	CriticalSections() { InitializeCriticalSection(&m_CS); }
	virtual ~CriticalSections() { DeleteCriticalSection(&m_CS); }
	
	void enter() { EnterCriticalSection(&m_CS); }
	void leave() { LeaveCriticalSection(&m_CS); }

private:
	CriticalSections(const CriticalSections&) {}
	const CriticalSections& operator = (const CriticalSections&) { return *this; }
};

