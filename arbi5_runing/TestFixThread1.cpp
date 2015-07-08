
#include "TestFixThread1.h"
#include "fixmsgdef.h"
#include "LogStore.h"
#include <iostream>
using namespace std;

#define MSG_TYPE_THREAD1_TIMER 1


TestFixThread1::TestFixThread1(void)
{
	m_pTarget = NULL;
}


TestFixThread1::~TestFixThread1(void)
{
}

BOOL TestFixThread1::Prepare(long *pnResult)
{
	TRACE_LOG("Thread1 in Prepare stage.");
	TRawMsg msg;
	INIT_MSG_HEADER(&msg);
	msg.type = MSG_TYPE_THREAD1_TIMER;
	m_local_tm.SetMessage(&msg);
	m_local_tm.SetTime(5, TRUE); //5 sec
	AddTimer(&m_local_tm);
	return TRUE;
}

BOOL TestFixThread1::Cleanup(long *pnResult)
{
	KillTimer(&m_local_tm);
	TRACE_LOG("Thread1 in Cleanup stage.");
	return TRUE;
}

bool TestFixThread1::OnMsg(TRawMsg *msg, IMsg *remote_src)
{
	if ( msg->type == MSGTYPE_FIXMSG_LOGON)
	{
		cout << "Thread1 received logon response." << endl;
	}
	else if ( msg->type == MSGTYPE_FIXMSG_LOGOUT)
	{
	}
	else if ( msg->type == MSG_TYPE_THREAD1_TIMER)
	{
		TRACE_LOG("MSG_TYPE_THREAD1_TIMER received.");
		TLogonMsg sender;
		sender.type = MSGTYPE_FIXMSG_LOGON;
		sender.len = sizeof(TLogonMsg)-sizeof(TRawMsg);
		sender.EncryptMethod = 0;
		sender.HeartBtInt = 30;
		sender.RawDataLength = 17;
		strcpy(sender.RawData,"1234567890abcdef");
		sender.ResetSeqNumFlag = 'Y';
		cout << "About to send longon to thread2" << endl;
		if(m_pTarget) m_pTarget->SendMsg(&sender);
		
	}
	else
	{
		TRACE_LOG("Unknown message 0x%x received from \'%s\'", 
			msg->type, remote_src ? remote_src->GetName() : "[Local]");
	}
	return TRUE;
}
