#pragma once
#pragma   warning   (disable:   4786) 
#include "FixThreadMsgPool.h"
#include "FixTimerManager.h"
#include <string>  
using namespace std;

#define MT_TERMINATE			((U32)-1)
#define MT_SYSTEM_TIMER			((U32)-5)
#define THREAD_MSG_POOL_DEFAULT_SIZE	(1000000)
#define THREAD_MSG_MAX_LEN				(5000)
#define MSG_THREAD_NAME_MAX_LEN		(50)
#define MSG_THREAD_DEFAULT_NAME		"defaultfixthread"
//////////////////////////////////////////////////////////////////////
//	interface IMsg & IMsgHandler

struct IMsg
{
	virtual const char* GetName() = 0;
	virtual BOOL SendMsg(TRawMsg *msg) = 0;
	virtual BOOL SendMsg2(U32 type, I32 len, const void *param);
	virtual ~IMsg();
};

struct IMsgHandler
{
	virtual bool OnMsg(TRawMsg *msg, IMsg *remote_src) = 0;
	virtual ~IMsgHandler();
};

////////////////////////////////////////////////////////////////////////////////
//	class CMsgTimer

class FixMsgTimer : public FixRawTimer
{
protected:
	IMsg *m_pTarget;
	U8 *m_pMsg;

	virtual void OnTimer();

public:
	FixMsgTimer();
	virtual ~FixMsgTimer();

	BOOL SetMessage(IMsg *tgt, TRawMsg *msg);
};

class FixThreadTimer;
class FixThread : public IMsg, public IMsgHandler, public FixTimerManager
{
public:
	FixThread(void);
	virtual ~FixThread(void);
	void StartThread();
	void StopThread();
	static void threadFunc( void* pParams ) ;
	const char* GetName();
	BOOL SendMsg(TRawMsg *msg);
	BOOL Init(const char *szName, long nSize = THREAD_MSG_POOL_DEFAULT_SIZE, void *pBuff = NULL);

protected:
	string m_strName;

	FixMsgTimer m_SysTimer;
	long m_nNextExpire;

	FixThreadMsgPool *m_pMsgPool;

	virtual long Running();

	virtual BOOL DoMsgLoop();

	void RefreshSysTimer();

	////////////////////////////////////////////////////////////
	//service functions
	friend class FixThreadTimer;
	virtual BOOL AddTimer(FixThreadTimer *pTimer);
	virtual BOOL KillTimer(FixThreadTimer *pTimer);
	virtual BOOL Prepare(long *pnResult) {return TRUE;};
	virtual BOOL Cleanup(long *pnResult) {return TRUE;};
};

////////////////////////////////////////////////////////////////////////////////
//	class FixMsgThreadTimer

class FixThreadTimer : public FixRawTimer
{
protected:
	friend class FixThread;

	FixThread *m_pThread;
	U8 *m_pMsg;

	virtual void OnTimer();

public:
	FixThreadTimer();
	virtual ~FixThreadTimer();

	BOOL SetMessage(TRawMsg *msg);
};
