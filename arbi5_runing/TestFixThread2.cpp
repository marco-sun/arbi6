#include "TestFixThread2.h"
#include "fixmsgdef.h"
#include "LogStore.h"
#include <iostream>
using namespace std;

#define MSG_TYPE_THREAD2_TIMER 2

TestFixThread2::TestFixThread2(void)
{
	m_pTarget = NULL;
}


TestFixThread2::~TestFixThread2(void)
{
}

BOOL TestFixThread2::Prepare(long *pnResult)
{
	TRACE_LOG("Thread2 in Prepare stage.");
	return TRUE;
}

BOOL TestFixThread2::Cleanup(long *pnResult)
{
	KillTimer(&m_local_tm);
	TRACE_LOG("Thread2 in Cleanup stage.");
	return TRUE;
}

bool TestFixThread2::OnMsg(TRawMsg *msg, IMsg *remote_src)
{
	if ( msg->type == MSGTYPE_FIXMSG_LOGON)
	{ // received logon, wait 3 secs to return logon msg to peer.
		TLogonMsg* pLogonMsg = (TLogonMsg*)msg;
		cout << pLogonMsg->EncryptMethod << " " 
			<< pLogonMsg->HeartBtInt << " "
			<< pLogonMsg->RawDataLength << " "
			<< pLogonMsg->ResetSeqNumFlag << endl;
		cout << pLogonMsg->RawData << endl;

		TRawMsg msg;
		INIT_MSG_HEADER(&msg);
		msg.type = MSG_TYPE_THREAD2_TIMER;
		m_local_tm.SetMessage(&msg);
		m_local_tm.SetTime(3, false); //3 sec
		AddTimer(&m_local_tm);

	}
	else if ( msg->type == MSGTYPE_FIXMSG_LOGOUT)
	{ // received logout
	}
	else if ( msg->type == MSG_TYPE_THREAD2_TIMER)
	{
		TRACE_LOG("MSG_TYPE_THREAD2_TIMER received.");
		cout << "Thread2 about to send logon response" << endl;
		TLogonMsg sender;
		sender.type = MSGTYPE_FIXMSG_LOGON;
		sender.len = sizeof(TLogonMsg)-sizeof(TRawMsg);
		sender.EncryptMethod = 0;
		sender.HeartBtInt = 30;
		sender.RawDataLength = 10;
		strcpy(sender.RawData,"logon OK.");
		sender.ResetSeqNumFlag = 'Y';
		if(m_pTarget) m_pTarget->SendMsg(&sender);
	}
	else
	{
		TRACE_LOG("Unknown message 0x%x received from \'%s\'", 
			msg->type, remote_src ? remote_src->GetName() : "[Local]");
	}
	return TRUE;
}
