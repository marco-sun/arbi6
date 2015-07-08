
#include "FixTimerManager.h"
#include <time.h>
#include <process.h>

////////////////////////////////////////////////////////////////////////////////
//	class FixRawTimer

FixRawTimer::FixRawTimer()
{
	m_pMngr = NULL;
	m_sExpire = 0;
	m_bPeriodic = FALSE;
}

FixRawTimer::~FixRawTimer()
{
	if (m_pMngr)
		m_pMngr->KillTimer(this);
}

void FixRawTimer::SetTime(U32 sExpire, BOOL bPeriodic/*=FALSE*/)
{
	m_sExpire = sExpire-1>0 ? sExpire-1 : 0 ;
	m_bPeriodic = bPeriodic;
}

////////////////////////////////////////////////////////////////////////////////
//	class CTimerManager

BOOL FixTimerManager::CheckTimer()
{
	long ttime = time(NULL);

	while (TRUE)
	{
		FixRawTimer *pTimer = NULL;

		if (m_pLock)
			m_pLock->Lock();

		FixTimerMap::iterator it = m_TimerMap.begin();
		if (it != m_TimerMap.end())
		{
			if (it->first < ttime)
			{
				pTimer = it->second;
				m_TimerMap.erase(it);

				if (pTimer->m_bPeriodic)
				{
					pTimer->m_it = m_TimerMap.insert(
							FixTimerMap::value_type(ttime + pTimer->m_sExpire, pTimer));
				}
				else
				{
					pTimer->m_pMngr = NULL;
					pTimer->m_it = m_TimerMap.end();
				}
			}
		}

		if (m_pLock)
			m_pLock->Unlock();

		if (! pTimer)
			break;

		pTimer->OnTimer();
	}
	return TRUE;
}

BOOL FixTimerManager::AddTimer(FixRawTimer *pTimer)
{
	long ttime = time(NULL) + pTimer->m_sExpire;

	if (pTimer->m_pMngr)
		pTimer->m_pMngr->KillTimer(pTimer);

	pTimer->m_pMngr = this;

	if (m_pLock)
		m_pLock->Lock();
	pTimer->m_it = m_TimerMap.insert(FixTimerMap::value_type(ttime, pTimer));
	if (m_pLock)
		m_pLock->Unlock();

	return TRUE;
}

BOOL FixTimerManager::KillTimer(FixRawTimer *pTimer)
{
	if (pTimer->m_pMngr == this)
	{
		if (m_pLock)
			m_pLock->Lock();
		if (pTimer->m_it != m_TimerMap.end())
		{
			m_TimerMap.erase(pTimer->m_it);
			pTimer->m_it = m_TimerMap.end();
		}
		if (m_pLock)
			m_pLock->Unlock();

		pTimer->m_pMngr = NULL;
		return TRUE;
	}
	return FALSE;
}

FixTimerManager::FixTimerManager(BOOL bNeedLock/*=TRUE*/)
{
	if (bNeedLock)
		m_pLock = new FixThreadLock();
}


FixTimerManager::~FixTimerManager(void)
{
	if (m_pLock)
		delete m_pLock;
}

////////////////////////////////////////////////////////////////////////////////
//	class FixTimerThread

BOOL FixTimerThread::DoLoop()
{
	return CheckTimer();
}

FixTimerThread::FixTimerThread(void)
	: FixTimerManager(TRUE)
{

}

FixTimerThread::~FixTimerThread(void)
{
}

void FixTimerThread::StartThread()
{
	_beginthread( FixTimerThread::fixTimerThreadFunc, 0, (void *)this);
}

void FixTimerThread::fixTimerThreadFunc(void* pParams)
{
	FixTimerThread* mainObj = (FixTimerThread*) pParams;
	while(true)
	{
		if(!mainObj->DoLoop()) break;
		Sleep(100);
	}
	_endthread();
}


static FixTimerThread g_DefaultTimerThread;
FixTimerThread* TUGetDefaultTimerThread()
{
	static BOOL s_b = TRUE;
	if (s_b)
	{
		g_DefaultTimerThread.StartThread();
		s_b = FALSE;
	}
	return &g_DefaultTimerThread;
}






