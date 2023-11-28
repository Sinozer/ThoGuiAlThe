#pragma once

struct CriticalSectionScope
{
	CriticalSectionScope(CRITICAL_SECTION& cs) : m_CS(cs)
	{
		EnterCriticalSection(&m_CS);
	}

	~CriticalSectionScope()
	{
		LeaveCriticalSection(&m_CS);
	}

private:
	CRITICAL_SECTION& m_CS;
};