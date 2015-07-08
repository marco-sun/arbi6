#include "FixTcpTrans.h"
#include "fixmsgdef.h"
#include "LogStore.h"
#include <iostream>
#include <process.h>
using namespace std;

#define MSG_TYPE_RECEIVE_TIMER 2

void fix_receive_tcp( void* pParams )
{ 
    FixTcpTrans *pFixTcpTrans = (FixTcpTrans *) pParams;
	if (pFixTcpTrans == NULL) {
		TRACE_LOG(" err: pFixTcpTrans is NULL");
		return;
	}
	TRACE_LOG("fix_receive_tcp start!");
	int len = 0;
	int code = 0;
	char szmsg[1024];
	while(1) {
		memset(szmsg, 0, sizeof(szmsg));
		len = pFixTcpTrans->m_cTcpClinet.TCPRecive(szmsg);
		if (len > 0)
		{
			TFixMsg sender;
			sender.type = MSGTYPE_FIXMSG_RECEIVE;
			sender.len = sizeof(TFixMsg)-sizeof(TRawMsg);
			sender.RawDataLength = len;
			strcpy(sender.RawData, szmsg);
			if(pFixTcpTrans->m_pTarget) pFixTcpTrans->m_pTarget->SendMsg(&sender);
		}else
		{
			TRACE_LOG("len=:%d",len);
			TRACE_LOG("tcp connect error!");

		    Sleep(1000);
			TFixReconnectMsg sender;
			sender.type = MSGTYPE_FIXMSG_RECONNECT;
			sender.len = sizeof(TFixReconnectMsg)-sizeof(TRawMsg);
			strcpy(sender.reason, "connect error");
			if(pFixTcpTrans->m_pTarget) pFixTcpTrans->m_pTarget->SendMsg(&sender);


			break;

		}
	}
	_endthread();
}

FixTcpTrans::FixTcpTrans(void)
{
	m_pTarget = NULL;
}


FixTcpTrans::~FixTcpTrans(void)
{
	
}

BOOL FixTcpTrans::Prepare(long *pnResult)
{
	return TRUE;
}

BOOL FixTcpTrans::Cleanup(long *pnResult)
{
	KillTimer(&m_local_tm);
	m_cTcpClinet.TCPClose();
	TRACE_LOG("FixTcpTrans in Cleanup stage.");
	return TRUE;
}

bool FixTcpTrans::OnMsg(TRawMsg *msg, IMsg *remote_src)
{
	//TRACE_LOG(" msgtype %d:", msg->type);
	if (msg->type == MSGTYPE_FIXMSG_CONNECT)
	{
		//TRACE_LOG(" msgtype %d:", msg->type);
		TFixConnect *pConnect = (TFixConnect*)msg;
		//TRACE_LOG(" connecting %s:%s", pConnect->sIp, pConnect->nPort);
		m_cTcpClinet.StartSock(pConnect->sIp, pConnect->nPort);
		int ret = m_cTcpClinet.CallServer();
		if (1 == ret)
			TRACE_LOG(" connect TCP server ok! %p", this);
		else{
			TRACE_ALARM(" connect TCP server failed! %p", this);
			return FALSE;
		}
		long ii = _beginthread(fix_receive_tcp, 0, (void *)this);

		TRACE_LOG(" _beginthread return:  %ld", ii);
	}
	else if (msg->type == MSGTYPE_FIXMSG_SEND)
	{ 
		TRACE_LOG("send send ");
		TFixMsg* pFixMsg = (TFixMsg*)msg;
		char szlogon[MSGSTRING_MAX_LEN];
		memset(szlogon, 0, sizeof(szlogon));
		strcpy(szlogon, pFixMsg->RawData);
		m_cTcpClinet.TCPSend(szlogon);
		TRACE_LOG("MSGTYPE_FIXMSG_SEND = %s", szlogon);
	}
	else if ( msg->type == MSG_TYPE_RECEIVE_TIMER)
	{
		TRACE_LOG("MSG_TYPE_RECEIVE_TIMER");
	}
	else
	{
		TRACE_LOG("Unknown message 0x%x received from \'%s\'", 
			msg->type, remote_src ? remote_src->GetName() : "[Local]");
	}
	return TRUE;
}
