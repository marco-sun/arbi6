#include "SimFixEngine.h"
#include "fixmsgdef.h"
#include "LogStore.h"
#include <time.h>

#define MSG_TYPE_LOGON_TIMER 1
#define MSG_TYPE_MD_TIMER 2

SimFixEngine* SimFixEngine::pStaticEngine = NULL;

IMsg* SimFixEngine::getPriceEngineInstance(IMsg *priceSource)
{
	if(pStaticEngine==NULL)
	{
		// create fixengine instance
		pStaticEngine = new SimFixEngine();
		pStaticEngine->ReadIniConfig();
		TRACE_LOG("SimFixEngine Created.");
	}
	pStaticEngine->pPriceSource = priceSource;
	return (IMsg*)pStaticEngine;
}

IMsg* SimFixEngine::getDealerEngineInstance(IMsg *dealer)
{
	if(pStaticEngine==NULL)
	{
		// create fixengine instance
		pStaticEngine = new SimFixEngine();
		pStaticEngine->ReadIniConfig();
		TRACE_LOG("SimFixEngine Created.");
	}
	pStaticEngine->pDealer = dealer;
	return (IMsg*)pStaticEngine;
}


SimFixEngine::SimFixEngine(void)
{
	pDealer = NULL;
	pPriceSource = NULL;
	contracts.clear();
	StartThread();
}


SimFixEngine::~SimFixEngine(void)
{
}

void SimFixEngine::ReadIniConfig()
{ // 登录需要的参数 HeartBeatInterval,username,password
	/*
	m_HeartBeatInterval = 30;
	memset(m_szUsername,   0, sizeof(m_szUsername));
	memset(m_szPassword,   0, sizeof(m_szPassword));

	GetPrivateProfileStringA("FIXPATSGW", "LOGON_UID", "patsusername", m_szUsername, 16, ".\\AIB.ini");
	GetPrivateProfileStringA("FIXPATSGW", "LOGON_PWD", "patspassword", m_szPassword, 16, ".\\AIB.ini");
	m_HeartBeatInterval = GetPrivateProfileIntA("FIXPATSGW","LOGON_HBINT",30,".\\AIB.ini");
	*/
}

BOOL SimFixEngine::Prepare(long *pnResult)
{
	TRACE_LOG("SimFixEngine Thread Prepare...");
	TRawMsg msg;
	INIT_MSG_HEADER(&msg);
	msg.type = MSG_TYPE_MD_TIMER;
	m_local_tm.SetMessage(&msg);
	m_local_tm.SetTime(2, TRUE); //2 sec
	AddTimer(&m_local_tm);
	return TRUE;
}

BOOL SimFixEngine::Cleanup(long *pnResult)
{
	TRACE_LOG("SimFixEngine Thread Cleanup...");
	KillTimer(&m_local_tm);
	return TRUE;
}

bool SimFixEngine::OnMsg(TRawMsg *msg, IMsg *remote_src)
{
	if ( msg->type == MSGTYPE_FIXMSG_LOGON)
	{
		TRACE_LOG("RECV LOGON request.");
		TRawMsg msg;
		INIT_MSG_HEADER(&msg);
		msg.type = MSG_TYPE_LOGON_TIMER;
		
		local_tm.SetMessage(&msg);
		local_tm.SetTime(5, FALSE); //5 sec
		AddTimer(&local_tm);
	}
	else if ( msg->type == MSG_TYPE_LOGON_TIMER)
	{ // 回复登录成功
		TRACE_LOG("Time to send logon resp.");
		TLogonRespMsg sender;
		sender.type = MSGTYPE_FIXMSG_LOGON_RESP;
		sender.len = sizeof(TLogonRespMsg)-sizeof(TRawMsg);
		sender.FailCode = 0;
		strcpy(sender.Reason,"");
		if(pPriceSource) pPriceSource->SendMsg(&sender);
		if(pDealer) pDealer->SendMsg(&sender);
	}
	else if ( msg->type == MSGTYPE_FIXMSG_MD)
	{
		TMDReqMsg *pMDReqMsg = (TMDReqMsg*)msg;
		string contract = pMDReqMsg->MDReqID;
		contracts.push_back(contract);
	}
	else if ( msg->type == MSG_TYPE_MD_TIMER)
	{
		list<string>::iterator iter = contracts.begin();
		while(iter != contracts.end())
		{
			issuePrice(*iter);
			iter++;
		}
	}
	else
	{
		TRACE_LOG("Unknown message 0x%x received from \'%s\'", 
			msg->type, remote_src ? remote_src->GetName() : "[Local]");
	}
	return TRUE;
}

void SimFixEngine::issuePrice(string MDId)
{
	TMDRespMsg sender;
	sender.type = MSGTYPE_FIXMSG_MD_RESP;
	sender.len = sizeof(TMDRespMsg)-sizeof(TRawMsg);
	strcpy(sender.MDReqID,MDId.c_str());
	sender.askprice = 100.15;
	sender.bidprice = 99.85;
	sender.tradeprice = 100.05;
	time_t t = time(NULL);
	struct tm *date = gmtime(&t);
	sender.year = 1900 + date->tm_year;
	sender.month = date->tm_mon + 1;
	sender.day = date->tm_mday;
	sender.hour = date->tm_hour;
	sender.min = date->tm_min;
	sender.sec = date->tm_sec;
	if(pPriceSource) pPriceSource->SendMsg(&sender);
}

