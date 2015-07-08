#pragma once
#pragma   warning   (disable:   4786) 
#include "FixThreadMsgPool.h"
#include <map>
using namespace std;

class FixRawTimer;
class FixTimerManager;

typedef multimap<long,FixRawTimer*> FixTimerMap;

class FixRawTimer
{
protected:
	friend class FixTimerManager;

	FixTimerManager		*m_pMngr;
	FixTimerMap::iterator	m_it;

	U32					m_sExpire;
	BOOL				m_bPeriodic;

	virtual void OnTimer() = 0;

	FixRawTimer();
	virtual ~FixRawTimer();
public:

	void SetTime(U32 msExpire, BOOL bPeriodic = FALSE);
};

class FixTimerManager
{
protected:
	FixThreadLock *m_pLock;
	FixTimerMap m_TimerMap;

	virtual BOOL CheckTimer();
public:
	FixTimerManager(BOOL bNeedLock = TRUE);
	virtual ~FixTimerManager(void);

	virtual BOOL AddTimer(FixRawTimer *pTimer);
	virtual BOOL KillTimer(FixRawTimer *pTimer);
};

////////////////////////////////////////////////////////////////////////////////
//	class FixTimerThread

class FixTimerThread : public FixTimerManager
{
protected:
	BOOL DoLoop();

public:
	FixTimerThread(void);
	virtual ~FixTimerThread(void);
	void StartThread();
	static void fixTimerThreadFunc(void* pParams);
};


FixTimerThread* TUGetDefaultTimerThread();

