// SpPriceSource.cpp: implementation of the SpPriceSource class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SpPriceSource.h"
#include <iostream>
#include "LogStore.h"
#include <process.h>
#include "Contract.h"

#include "RohonPriceSource.h"
#include "KnownContract.h"
// 配置参数
static char  SPServerIp[32] ;		        // 前置地址
static int	 SPServerPort ;				    // 端口号
static char  SPInvestorId[16] ;			    // 投资者帐号代码
static char  SPUserId[16] ;		            // 用户
static char  SPPassword[16] ;			       // 用户密码
static char  InstrumentID[16] ;				// 行情订阅列表

int g_sendPriceReqlag;
LISTCHARM g_querylist;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//A work thread to handle comming msgs from network
void receive_tcp( void* pParams )
{ 
    SpPriceSource *sPPriceSource = (SpPriceSource *) pParams;
	if (sPPriceSource == NULL) {
		TRACE_LOG(" err: sPPriceSource is NULL");
		return;
	}
	int len = 0;
	int code = 0;
	char szmsg[1024];

	while(1) {
		memset(szmsg, 0, sizeof(szmsg));
		len = sPPriceSource->m_cTcpClinet.TCPRecive(szmsg);
		
		if (len > 0)
		{
			code = sPPriceSource->DecodeMsgHead(szmsg);
			switch(code) {
			case MSGID_USER_LOGIN:
			case MSGID_USER_LOGOUT:
				sPPriceSource->DealUserLoadReply(szmsg);
				break;
			case MSGID_ACCOUNT_LOGIN:
			case MSGID_ACCOUNT_LOGOUT:
				sPPriceSource->DealAccountLoadReply(szmsg);
				break;
			case MSGID_PRC_SNAP_REQ:
				//TRACE_LOG("MSGID_PRC_SNAP_REQ %d, %s",MSGID_PRC_SNAP_REQ, szmsg);
				break;
			case MSGID_PRC_UPD_REQ:
				break;
			case MSGID_PRC_UPD_REL:
				break;
			case MSGID_UPDATED_PRICE:
				sPPriceSource->DealSnapPrice(szmsg);
				g_sendPriceReqlag = 0;
				break;
			default:
				break;
			}
		}else
			Sleep(500);

	}
	_endthread();
}

void  waitQes()
{
	int i=200;
	while (i--)
	{
		Sleep(5);
		if (g_sendPriceReqlag == 0)
		{
			break;
		}
	}
}

//A work thread to rolling contract.
void  price_query( void* pParams )
{
	SpPriceSource *sPPriceSource = (SpPriceSource *) pParams;
	if (sPPriceSource == NULL) {
		TRACE_LOG(" err: sPPriceSource is NULL");
		return;
	}
	while (1)
	{
		EnterCriticalSection(&(sPPriceSource->csPrice));
		list<Tmsg>::iterator it ;//= g_querylist.begin();
		for(it = g_querylist.begin(); it != g_querylist.end(); ++it)//query list
		{
			char szInstrumentID[32];
			strcpy_s(szInstrumentID, 32, (*it).InstrumentID);

			sPPriceSource->SPPrcSnapReq(szInstrumentID);
			g_sendPriceReqlag = 1;
			waitQes();
			Sleep(50);
		}
		LeaveCriticalSection(&(sPPriceSource->csPrice));
		Sleep(500);
	}
	_endthread();
}

SpPriceSource::SpPriceSource()
{
	started = false;
	ReadIniConfig();
	g_sendPriceReqlag = 0;
	InitializeCriticalSection(&csPrice);
}

SpPriceSource::~SpPriceSource()
{
	DeleteCriticalSection(&csPrice);
}

void SpPriceSource::start(void)
{
    TRACE_LOG("启动 SPPriceSource......");
    if(started) {
	    TRACE_LOG("SPPriceSource already started.");
	    return;
	}
	if (1 == startTcp())
	{
		TRACE_LOG(" TCP服务器连接成功");
	}
	else
	{
		TRACE_LOG(" TCP服务器连接失败");
		exit(1);
	}
	_beginthread( receive_tcp, 0, (void *)this);
	_beginthread( price_query, 0, (void *)this);
    started = true;	
	TRACE_LOG("SPPriceSource started");
	return;
}

void SpPriceSource::add(string ID)
{
	char szInstrumentID[16];
	memset(szInstrumentID, 0, 16);
	strcpy_s(szInstrumentID, 16, ID.c_str());
	Tmsg msg;
	strcpy_s(msg.InstrumentID, 16, szInstrumentID);

	EnterCriticalSection(&csPrice);
	g_querylist.push_back (msg);
	TRACE_LOG("SPPriceSource Add Id %s.", ID.c_str());
	LeaveCriticalSection(&csPrice);
}

void SpPriceSource::ReadIniConfig()
{
	char szFrontAddr[32];
	char szServerPort[16];
	char szInvestorId[16];
	char szUserId[16];
	char szPassword[16];

	memset(szFrontAddr,  0, sizeof(szFrontAddr));
	memset(szServerPort,   0, sizeof(szServerPort));
	memset(szInvestorId, 0, sizeof(szInvestorId));
	memset(szUserId, 0, sizeof(szUserId));
	memset(szPassword,   0, sizeof(szPassword));

	if(GetPrivateProfileString("SP", "SPServerIp", "127.0.0.1", szFrontAddr, 128, ".\\AIB.ini"))
	{
		strcpy_s(SPServerIp, 32, szFrontAddr);
	}
	if(GetPrivateProfileString("SP", "SPServerPort", "8089", szServerPort, 16, ".\\AIB.ini"))
	{
		SPServerPort = atoi(szServerPort);
	}
	if(GetPrivateProfileString("SP", "SPInvestorId",  "80000003", szInvestorId, 16, ".\\AIB.ini"))
	{
        strcpy_s(SPInvestorId, 16, szInvestorId);
	}
	if(GetPrivateProfileString("SP", "SPUserId",  "80000003", szUserId, 16, ".\\AIB.ini"))
	{
        strcpy_s(SPUserId, 16, szUserId);
	}
	if(GetPrivateProfileString("SP", "SPPassword", "999999", szPassword, 16, ".\\AIB.ini"))
	{
        strcpy_s(SPPassword, 16, szPassword);
	}
}

int  SpPriceSource::DecodeMsgHead(char * pmsg)
{
	strings pars;	
	strings::iterator it;		

	int npar = 0;
	int code = 0;
	m_sPPublic.divide_string(pmsg,pars,",",TRUE);
	for(npar = 0,it = pars.begin(); it != pars.end();it++,npar++)
	{
		code = atoi((*it).c_str());
		//TRACE_LOG("%s",(*it).c_str());
		return code;
	}
	return 0;
}

// handle user log in and out.
int  SpPriceSource::DealUserLoadReply(char * pmsg)
{
	strings pars;	
	strings::iterator it;		

	int npar = 0;
	int code = 0;
	int ret = 0;

    char szerr[256];
	memset(szerr, 0, sizeof(szerr));
	m_sPPublic.divide_string(pmsg,pars,",",TRUE);
	for(npar = 0,it = pars.begin(); it != pars.end();it++,npar++)
	{
		switch(npar) {
		case 0:
			code = atoi((* it).c_str());
			break;
		case 2:
			ret = atoi((* it).c_str());
			break;
		case 3:
			strcpy_s(szerr, 256, (* it).c_str());
			break;
		default:
			break;
		}
	}
	if (0 != ret)
	{
		TRACE_LOG("msg code %d error: %s.",code, szerr);
		return 0;
	}
	return 1;
	/*
	<MessageId>,<MessageType>,<ReturnCode>,<ReturnMessage>,<UserId>,<SvrVerNo>,<SvrRelNo>,
	<SvrLangNo>,<LoginTime>,<AccNo><cr><lf>
	e.g. 3101,3,0,OK,CLIENT1,1.0,8.07,0,1234567890,CLIENT1,192.168.1.1<cr><lf>
	*/
}

// handle account log in and out.
int  SpPriceSource::DealAccountLoadReply(char * pmsg)
{
	strings pars;	
	strings::iterator it;		

	int npar = 0;
	int code = 0;
	int ret = 0;

    char szerr[256];
	memset(szerr, 0, sizeof(szerr));
	m_sPPublic.divide_string(pmsg,pars,",",TRUE);
	for(npar = 0,it = pars.begin(); it != pars.end();it++,npar++)
	{
		switch(npar) {
		case 0:
			code = atoi((* it).c_str());
			break;
		case 2:
			ret = atoi((* it).c_str());
			break;
		case 5:
			strcpy_s(szerr, 256, (* it).c_str());
			break;
		default:
			break;
		}
	}
	if (0 != ret)
	{
		TRACE_LOG("msg code %d error: %s.",code, szerr);
		return 0;
	}
	return 1;
}

// handle price notice.
int  SpPriceSource::DealSnapPrice(char * pmsg)
{
	strings pars;	
	strings::iterator it;		
	
	Pricing tmpPrice("N/A",0.0,0.0);

	int npar = 0;
	int code = 0;
	time_t now;
	time( &now );
	//TRACE_LOG("PRICE ,%s",pmsg);

	m_sPPublic.divide_string(pmsg,pars,",",TRUE);
	char  szProductId[32];

	for(npar = 0,it = pars.begin(); it != pars.end();it++,npar++)
	{
		switch(npar) {
		case 2:
			memset(szProductId, 0, 32);
			strcpy_s(szProductId, 32, (* it).c_str());
			break;
		case 12:
			tmpPrice.bid = atof((* it).c_str());
			break;
		case 22:
			tmpPrice.ask = atof((* it).c_str());
			break;
		case 32:
			tmpPrice.lastprice = atof((* it).c_str());
			break;
		case 49:
			//tmpPrice.upperLimit = atof((* it).c_str());
			break;
		case 50:
			//tmpPrice.lowerLimit = atof((* it).c_str());
			break;
		default:
			break;
		}
	}
	//tmpPrice.time = now;
	tmpPrice.contract = string(szProductId);
	//TRACE_LOG("Price for ,%s",tmpPrice.contract.c_str());
	//inform(tmpPrice);
	Contract::get(tmpPrice.contract)->setPrice(0,tmpPrice.ask,tmpPrice.bid,tmpPrice.lastprice);

	return 1;
}

int  SpPriceSource::startTcp()
{
	int ret;
	TRACE_LOG("Connecting %s:%d.", SPServerIp, SPServerPort);
	m_cTcpClinet.StartSock(SPServerIp, SPServerPort);
	ret = m_cTcpClinet.CallServer();
	return ret;
}
// not used now.
int  SpPriceSource::closeTcp()
{
	return m_cTcpClinet.TCPClose();
}
// not used now.
void SpPriceSource::SPUserLogin()
{
	char szUserLogin[256];
	memset(szUserLogin, 0, sizeof(szUserLogin));

	//get UserLogin msg
	strcat_s(szUserLogin, 256, "3101,0,");
	strcat_s(szUserLogin, 256, SPUserId);
	strcat_s(szUserLogin, 256, ",");
	strcat_s(szUserLogin, 256, SPPassword);
	strcat_s(szUserLogin, 256, ",");
	strcat_s(szUserLogin, 256, SPServerIp);
	strcat_s(szUserLogin, 256, "\r\n");
	//<MessageId>,<MessageType>,<UserId>,<Password>,<Host><cr><lf> e.g. 3101,0,CLIENT1,pass1,Host<cr><lf>

	if(1 != m_cTcpClinet.TCPSend(szUserLogin))
		TRACE_LOG("SpPriceSource: send msg failed");
}
// not used now.
void SpPriceSource::SPAccountLogin()
{
	char szAccountLogin[256];
	memset(szAccountLogin, 0, sizeof(szAccountLogin));

	//get AccountLogin msg
	strcat_s(szAccountLogin, 256, "3121,0,");
	strcat_s(szAccountLogin, 256, SPInvestorId);
	strcat_s(szAccountLogin, 256, "\r\n");
	//e.g. 3121,0,1000   <MessageId>,<MessageType>,<AccNo><cr><lf>

	if(1 != m_cTcpClinet.TCPSend(szAccountLogin))
		TRACE_LOG("SpPriceSource: send msg failed");

}
// not used now.
void SpPriceSource::SPUserLogout()
{
	char szUserLogout[256];
	memset(szUserLogout, 0, sizeof(szUserLogout));

	//get UserLogout msg
	strcat_s(szUserLogout, 256, "3102,0");
	strcat_s(szUserLogout, 256, "\r\n");
	//<MessageId>,<MessageType><cr><lf> e.g. 3102,0<cr><lf>

	if(1 != m_cTcpClinet.TCPSend(szUserLogout))
		TRACE_LOG("SpPriceSource: send msg failed");
}
// not used now.
void SpPriceSource::SPAccountLogout()
{
	char szAccountLogout[256];
	memset(szAccountLogout, 0, sizeof(szAccountLogout));
	
	//get AccountLogout msg
	strcat_s(szAccountLogout, 256, "3122,0,");
	strcat_s(szAccountLogout, 256, SPInvestorId);
	strcat_s(szAccountLogout, 256, "\r\n");
	//e.g. 3122,0,1000 <MessageId>,<MessageType>,<AccNo><cr><lf>

	if(1 != m_cTcpClinet.TCPSend(szAccountLogout))
		TRACE_LOG("SpPriceSource: send msg failed");
}

//send price request.
void SpPriceSource::SPPrcSnapReq(char * szInstrumentID)
{
	char szPrcSnapReq[256];
	memset(szPrcSnapReq, 0, sizeof(szPrcSnapReq));

	//get UserLogin msg
	strcat_s(szPrcSnapReq, 256, "4106,0,");
	strcat_s(szPrcSnapReq, 256, szInstrumentID);
	strcat_s(szPrcSnapReq, 256, "\r\n");
	//<MessageId>,<MessageType>,<ProductId><cr><lf> e.g.: 4106,0,HSIN8<cr><lf>

	//TRACE_LOG("SPPrcSnapReq: %s", szPrcSnapReq);
	if(1 != m_cTcpClinet.TCPSend(szPrcSnapReq))
		TRACE_LOG("SpPriceSource: send msg failed");

}

