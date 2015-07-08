
#include "FixThread.h"
#include <time.h>
#include <process.h>


//////////////////////////////////////////////////////////////////////
//	interface IMsg & IMsgHandler

BOOL IMsg::SendMsg2(U32 type, I32 len, const void *param)
{
	U8 buffer[THREAD_MSG_MAX_LEN];
	TRawMsg *msg = AssembleMsg(buffer, sizeof(buffer), type, len, param);

	if (msg)
		return SendMsg(msg);
	return FALSE;
}

IMsg::~IMsg()
{
}

IMsgHandler::~IMsgHandler()
{
}

////////////////////////////////////////////////////////////////////////////////
//	class FixMsgTimer

void FixMsgTimer::OnTimer()
{
	if (m_pTarget && m_pMsg)
		m_pTarget->SendMsg((TRawMsg*)m_pMsg);
}

FixMsgTimer::FixMsgTimer()
{
	m_pTarget = NULL;
	m_pMsg = NULL;
}

FixMsgTimer::~FixMsgTimer()
{
	if (m_pMsg)
		delete [] m_pMsg;
}

BOOL FixMsgTimer::SetMessage(IMsg *tgt, TRawMsg *msg)
{
	I32 len = MSG_TOTAL_LEN(msg);
	if (len > THREAD_MSG_MAX_LEN)
		return FALSE;

	if (m_pMsg)
		delete [] m_pMsg;

	m_pTarget = tgt;

	m_pMsg = new U8[len];
	memcpy(m_pMsg, msg, len);
	return TRUE;
}

FixThread::FixThread(void)
{
	m_strName = MSG_THREAD_DEFAULT_NAME;
	m_pMsgPool = NULL;
	TRawMsg msgTimer;
	m_SysTimer.SetMessage(this, 
			AssembleMsg(&msgTimer, sizeof(msgTimer), MT_SYSTEM_TIMER, 0, NULL));
	m_nNextExpire = 0;
}


FixThread::~FixThread(void)
{
	if (m_pMsgPool)
		delete m_pMsgPool;
}

void FixThread::RefreshSysTimer()
{
	long ttime;
	FixTimerMap::iterator it = m_TimerMap.begin();
	if (it != m_TimerMap.end())
	{
		ttime = it->first;
		if (m_nNextExpire != ttime)
		{
			m_nNextExpire = ttime;
			ttime = ttime - time(NULL);
			m_SysTimer.SetTime(ttime < 0 ? 0 : ttime);
			TUGetDefaultTimerThread()->AddTimer(&m_SysTimer);
		}
	}
	else
	{
		TUGetDefaultTimerThread()->KillTimer(&m_SysTimer);
		m_nNextExpire = 0;
	}
}

BOOL FixThread::AddTimer(FixThreadTimer *pTimer)
{
	if (FixTimerManager::AddTimer(pTimer))
	{
		pTimer->m_pThread = this;
		RefreshSysTimer();
		return TRUE;
	}
	return FALSE;
}

BOOL FixThread::KillTimer(FixThreadTimer *pTimer)
{
	if (FixTimerManager::KillTimer(pTimer))
	{
		RefreshSysTimer();
		return TRUE;
	}
	return FALSE;
}

void FixThread::StartThread()
{
	if (! m_pMsgPool)
		Init(NULL);
	//start thread
	_beginthread( FixThread::threadFunc, 0, (void *)this);
}

void FixThread::StopThread()
{
	SendMsg2(MT_TERMINATE, 0, NULL);
}

void FixThread::threadFunc( void* pParams ) 
{
	FixThread* mainObj = (FixThread*)pParams;
	 
	mainObj->Running();

	_endthread();
}

long FixThread::Running()
{
	long result = 0;
	if (Prepare(&result))
	{
		DoMsgLoop();
		Cleanup(&result);
	}
	return result;
}

BOOL FixThread::DoMsgLoop()
{
	I32 len;
	U8 buffer[THREAD_MSG_MAX_LEN];
	TRawMsg *msg = (TRawMsg*)buffer;

	while (1)
	{
		len = THREAD_MSG_MAX_LEN;
		if (m_pMsgPool->ReadMsg(msg, sizeof(buffer)))
		{
			if (msg->type == MT_TERMINATE)
			{
				return TRUE;
			}
			else if (msg->type == MT_SYSTEM_TIMER)
			{
				CheckTimer();
				m_nNextExpire = 0;
				RefreshSysTimer();
			}
			else
			{
				OnMsg(msg, NULL);
			}
		}
		Sleep(5);
	}
	return TRUE;
}

const char* FixThread::GetName()
{
	return m_strName.c_str();
}

BOOL FixThread::SendMsg(TRawMsg *msg)
{
	if (! m_pMsgPool)
		return FALSE;
	if (MSG_TOTAL_LEN(msg) > THREAD_MSG_MAX_LEN )
		return FALSE;
	return m_pMsgPool->WriteMsg(msg);
}

BOOL FixThread::Init(const char *szName,
			long nSize/*=THREAD_MSG_POOL_DEFAULT_SIZE*/, void *pBuff/*=NULL*/)
{
	if (m_pMsgPool)
		return FALSE;

	if (szName)
		m_strName = szName;

	FixThreadMsgPool *pPool = new FixThreadMsgPool();
	pPool->Init(nSize, pBuff, TRUE, FALSE);
	m_pMsgPool = pPool;

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//	class FixThreadTimer

FixThreadTimer::FixThreadTimer()
{
	m_pThread = NULL;
	m_pMsg = NULL;
}

FixThreadTimer::~FixThreadTimer()
{
	if (m_pMsg)
		delete [] m_pMsg;
}

void FixThreadTimer::OnTimer()
{
	if (m_pThread && m_pMsg)
	{
		U8 buffer[THREAD_MSG_MAX_LEN];
		I32 len = MSG_TOTAL_LEN((TRawMsg*)m_pMsg);
		memcpy(buffer, m_pMsg, len);

		m_pThread->OnMsg((TRawMsg*)buffer, NULL);
	}
}

BOOL FixThreadTimer::SetMessage(TRawMsg *msg)
{
	I32 len = MSG_TOTAL_LEN(msg);
	if (len > THREAD_MSG_MAX_LEN)
		return FALSE;

	if (m_pMsg)
		delete [] m_pMsg;

	m_pMsg = new U8[len];
	memcpy(m_pMsg, msg, len);
	return TRUE;
}

