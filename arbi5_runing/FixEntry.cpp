#include "FixEntry.h"
#include "fixmsgdef.h"
#include "LogStore.h"
#include <iostream>
using namespace std;

#define MSG_TYPE_HEARTBEATS_TIMER 6
string g_senderCompID;
string g_targetCompID;
string g_account;

FixEntry* FixEntry::pStaticEngine = NULL;

IMsg* FixEntry::getPriceEngineInstance(IMsg *priceSource)
{
	if(pStaticEngine==NULL)
	{
		// create fixengine instance
		pStaticEngine = new FixEntry();
		pStaticEngine->ReadIniConfig();
		pStaticEngine->Init();
		TRACE_LOG("FixEntry Created for pricesource.");
	}
	if(pStaticEngine->m_pPriceTarget==NULL) pStaticEngine->m_pPriceTarget = priceSource;
	return (IMsg*)pStaticEngine;
}

IMsg* FixEntry::getDealerEngineInstance(IMsg *dealer)
{
	if(pStaticEngine==NULL)
	{
		// create fixengine instance
		pStaticEngine = new FixEntry();
		pStaticEngine->ReadIniConfig();
		pStaticEngine->Init();
		TRACE_LOG("FixEntry Created for dealer.");
	}
	if(pStaticEngine->m_pDealerTarget==NULL) pStaticEngine->m_pDealerTarget = dealer;
	return (IMsg*)pStaticEngine;
}

FixEntry::FixEntry()
{
	m_pTarget = NULL;
	m_pPriceTarget = NULL;
	m_pDealerTarget = NULL;
}

void FixEntry::ReadIniConfig()
{ // FixEngine需要的参数 TGWIP,TGWPORT,senderCompID,targetCompID
	m_cfgPort = 3040;
	memset(m_cfgIP,   0, sizeof(m_cfgIP));
	memset(m_cfgSenderCompID,   0, sizeof(m_cfgSenderCompID));
	memset(m_cfgTargetCompID,   0, sizeof(m_cfgTargetCompID));
	GetPrivateProfileStringA("FIXENGINE", "TGWIP", "127.0.0.1", m_cfgIP, 16, ".\\AIB.ini");
	m_cfgPort = GetPrivateProfileIntA("FIXENGINE","TGWPORT",3040,".\\AIB.ini");
	GetPrivateProfileStringA("FIXENGINE", "SENDERCOMPID", "ZHUJIN", m_cfgSenderCompID, 64, ".\\AIB.ini");
	GetPrivateProfileStringA("FIXENGINE", "TARGETCOMPID", "PATS", m_cfgTargetCompID, 64, ".\\AIB.ini");
	GetPrivateProfileStringA("FIXENGINE", "ACCOUNT", "APPID_2350_TA", m_cfgaccount, 21, ".\\AIB.ini");
}

void FixEntry::Init()
{
	StartThread();
	m_msgSeqNum = 1;
	m_RemoteSeq = 0;
	SetPeer(&m_tcp_trans);
	m_tcp_trans.SetPeer(this);
	m_tcp_trans.StartThread();

	TFixConnect sender;
	sender.type = MSGTYPE_FIXMSG_CONNECT;
	sender.len = sizeof(TFixConnect)-sizeof(TRawMsg);
	strcpy(sender.sIp, m_cfgIP);
	sender.nPort = m_cfgPort;
	strcpy(sender.connect, "connect");
	m_tcp_trans.SendMsg(&sender);

	g_senderCompID = m_cfgSenderCompID;
	g_targetCompID = m_cfgTargetCompID;
	g_account	   = m_cfgaccount;

	m_hasLogon = false;

}

FixEntry::~FixEntry(void)
{

}
BOOL FixEntry::Prepare(long *pnResult)
{
	return TRUE;
}

BOOL FixEntry::Cleanup(long *pnResult)
{
	KillTimer(&m_local_tm);
	return TRUE;
}

bool FixEntry::OnMsg(TRawMsg *msg, IMsg *remote_src)
{
	if ( msg->type == MSGTYPE_FIXMSG_LOGON)
	{ 
		if(m_hasLogon)
		{
			TLogonRespMsg tLonResp;
			tLonResp.type = MSGTYPE_FIXMSG_LOGON_RESP;
			tLonResp.len = sizeof(TLogonRespMsg)-sizeof(TRawMsg);
			tLonResp.FailCode = 0;
			strcpy(tLonResp.Reason, "");
			if (m_pPriceTarget)
				m_pPriceTarget->SendMsg(&tLonResp);
			if(m_pDealerTarget)
				m_pDealerTarget->SendMsg(&tLonResp);
		}
		else
		{
			TLogonMsg* pLogonMsg = (TLogonMsg*)msg;

			string smsg = FixPrice::FixUserLogin(pLogonMsg, m_msgSeqNum);

			SendMsgToGW(smsg);
		}
	}
	else if ( msg->type == MSGTYPE_FIXMSG_MD)
	{ 
		TMDReqMsg* pMDReqMsg = (TMDReqMsg*)msg;

		string smsg = FixPrice::FixMDReq(pMDReqMsg, m_msgSeqNum);

		SendMsgToGW(smsg);
	}
	
	else if ( msg->type ==  MSGTYPE_FIXMSG_RESET_SEQ)
	{ 
		TResetSeqMsg* presetseqMsg = (TResetSeqMsg*)msg;

		m_msgSeqNum = presetseqMsg->SeqId;
		m_RemoteSeq = 0;
		TRACE_LOG("cur LocalSeq %d ; cur RemoteSeq  %d  ", m_msgSeqNum, m_RemoteSeq);
	}
	else if ( msg->type == MSGTYPE_FIXMSG_LOGOUT)
	{ 
		// send logout
		TLogoutMsg* pLogoutMsg = (TLogoutMsg*)msg;

		string smsg = FixPrice::FixUserLogout(pLogoutMsg, m_msgSeqNum);

		SendMsgToGW(smsg);
	}
	else if ( msg->type == MSGTYPE_FIXMSG_NOS)
	{ 
		TNewOrderSingleMsg* pNOSMMsg = (TNewOrderSingleMsg*)msg;

		string smsg = FixDealer::FixNewOrderSingle(pNOSMMsg, m_msgSeqNum);

		SendMsgToGW(smsg);
	}
	else if ( msg->type == MSGTYPE_FIXMSG_CANCEL)
	{ 
		TOrderCancelMsg* pOCMsg = (TOrderCancelMsg*)msg;

		string smsg = FixDealer::FixOrderCancel(pOCMsg, m_msgSeqNum);

		SendMsgToGW(smsg);
	}
	else if ( msg->type == MSGTYPE_FIXMSG_RECEIVE)
	{ 
		// received msg
		TFixMsg *pFixMsg = (TFixMsg*)msg;
		int sep = pFixMsg->RawDataLength/400;
		for(int i = 0; i<= sep; i++){
			char debugoutput[401];
			memset(debugoutput,0,401);
			strncpy(debugoutput,pFixMsg->RawData+i*400,400);
			TRACE_LOG("MSGTYPE_FIXMSG_RECEIVE_%d = %s",i+1, debugoutput);
		}

		FixMsgDeal(pFixMsg->RawData);
	}
	else if ( MSGTYPE_FIXMSG_RECONNECT)
	{
		TFixConnect sender;
		sender.type = MSGTYPE_FIXMSG_CONNECT;
		sender.len = sizeof(TFixConnect)-sizeof(TRawMsg);
		strcpy(sender.sIp, m_cfgIP);
		sender.nPort = m_cfgPort;
		strcpy(sender.connect, "connect");
		m_tcp_trans.SendMsg(&sender);
	}
	else if ( msg->type == MSG_TYPE_HEARTBEATS_TIMER)
	{
		//send heartbeats
	}
	else
	{
		TRACE_LOG("Unknown message 0x%x received from \'%s\'", 
			msg->type, remote_src ? remote_src->GetName() : "[Local]");
	}
	return TRUE;
}

void FixEntry::FixMsgDeal(string sbody)
{
	LISTFIXMSGALL listFixAll = FixDecode::fixDecodeMsg(sbody);
	list<LISTFIXMSG>::iterator iter ;
	for(iter = listFixAll.begin(); iter != listFixAll.end(); ++iter)
	{
		TRACE_LOG("LISTFIXMSGALL size = %d", listFixAll.size());
		LISTFIXMSG listFix = (*iter);
		if(listFix.size() >0)
		{
			list<TFIXMSG>::iterator it ;
			int i = 0;
			int connectflag = 0;
			for(i = 0, it = listFix.begin(); it != listFix.end(); ++it,i++)
			{
				int ntag = atoi((*it).msg_tag.c_str());
				switch(ntag) {
				case 34:
					{
						int seq = atoi((*it).msg_value.c_str());
						if(seq == m_RemoteSeq +1){ 
							m_RemoteSeq = seq;
							TRACE_LOG("cur RemoteSeq  %d  the receive msg seq = %d; ", m_RemoteSeq, seq);
						}
						else if (seq > m_RemoteSeq +1)
						{
							//send ResendReq (m_RemoteSeq +1, seq)
							TRACE_LOG("SendResendSeqMsg; cur RemoteSeq  %d  the receive msg seq = %d; ", m_RemoteSeq, seq);
							SendResendSeqMsg(m_RemoteSeq +1, seq);
							connectflag = 1;// drop this msg;
						}else{
							TRACE_LOG("receive smaller seq; cur RemoteSeq  %d  the receive msg seq = %d; ", m_RemoteSeq, seq);
							connectflag = 1;// seq < m_RemoteSeq + 1 ,drop this msg;
						}
					}
				default:
					break;
				}
			}
			if(connectflag == 1)
				break;
			for(i = 0, it = listFix.begin(); it != listFix.end(); ++it,i++)
			{
				int ntag = atoi((*it).msg_tag.c_str());
				switch(ntag) {
				case 35:
					if ((*it).msg_value.compare("A") == 0)//Logon
						DealLogOnACKMsg(listFix);
					else if ((*it).msg_value.compare("3") == 0)//Reject
						DealLogOnACKMsg(listFix);
					else if ((*it).msg_value.compare("5") == 0)//logout
						DealLogOutACKMsg(listFix);
					else if ((*it).msg_value.compare("X") == 0)//MD Res
						DealMDACKMsg(listFix);
					else if ((*it).msg_value.compare("0") == 0)//HB
						DealHBMsg(listFix);
					else if ((*it).msg_value.compare("1") == 0)//TR
						DealTRMsg(listFix);	
					else if ((*it).msg_value.compare("Y") == 0)//MD Reject
						DealMDRejectMsg(listFix);
					else if ((*it).msg_value.compare("8") == 0)//Order report
						DealOrderReport(listFix);
					else if ((*it).msg_value.compare("9") == 0)//Cancel Reject
						DealCancelReject(listFix);
					else if ((*it).msg_value.compare("2") == 0)//Resendreq
						DealResendSeq(listFix);
					else if ((*it).msg_value.compare("4") == 0)//grap fill
						DealGrapFill(listFix);
					break;
				default:
					break;
				}
			}
		}
		else
			TRACE_LOG("the msg is wrong! %s", sbody.c_str());
	}
		
}

void FixEntry::DealLogOnACKMsg(LISTFIXMSG listFix)
{
	TLogonRespMsg tLonResp;
	tLonResp.type = MSGTYPE_FIXMSG_LOGON_RESP;
	tLonResp.len = sizeof(TLogonRespMsg)-sizeof(TRawMsg);
	tLonResp.FailCode = 0;
	strcpy(tLonResp.Reason, "");
	
	if (m_pPriceTarget)
		m_pPriceTarget->SendMsg(&tLonResp);
	m_hasLogon = true;
	TRACE_LOG("LogOn OK!");
}

void FixEntry::DealLogOutACKMsg(LISTFIXMSG listFix)
{
	TLogonRespMsg tLonResp;
	tLonResp.type = MSGTYPE_FIXMSG_LOGOUT_RESP;
	tLonResp.len = sizeof(TLogoutRespMsg)-sizeof(TRawMsg);
	tLonResp.FailCode = 0;
	strcpy(tLonResp.Reason, "");
	
	if (m_pPriceTarget)
		m_pPriceTarget->SendMsg(&tLonResp);
	m_hasLogon = false;
	TRACE_LOG("Logout OK!");
}
void FixEntry::SendMsgToGW(string smsg)
{
	TFixMsg sender;
	sender.type = MSGTYPE_FIXMSG_SEND;
	sender.len = sizeof(TFixMsg)-sizeof(TRawMsg);
	sender.RawDataLength = smsg.size();
	strcpy(sender.RawData, smsg.c_str());
	if(m_pTarget) {
		m_pTarget->SendMsg(&sender);
		m_msgSeqNum++;
	}
}

void FixEntry::DealHBMsg(LISTFIXMSG listFix)
{
	THBMsg tHB;
	tHB.type = MSGTYPE_FIXMSG_HB;
	tHB.len = sizeof(THBMsg)-sizeof(TRawMsg);
	memset(tHB.TestReqID, 0, sizeof(tHB.TestReqID));
	
	string smsg = FixPrice::FixHB(&tHB, m_msgSeqNum);

	SendMsgToGW(smsg);
}

void FixEntry::SendResendSeqMsg(int beginNo, int endNo)
{
	TResendSeqMsg tRS;
	tRS.type = MSGTYPE_FIXMSG_RESEND_SEQ;
	tRS.len = sizeof(TResendSeqMsg)-sizeof(TRawMsg);
	tRS.beginNo = beginNo;
	tRS.endNo   = endNo;
	string smsg = FixPrice::FixResendSeq(&tRS, m_msgSeqNum);

	SendMsgToGW(smsg);
}

void FixEntry::DealTRMsg(LISTFIXMSG listFix)
{
	THBMsg tHB;
	tHB.type = MSGTYPE_FIXMSG_HB;
	tHB.len = sizeof(THBMsg)-sizeof(TRawMsg);
	memset(tHB.TestReqID, 0, sizeof(tHB.TestReqID));
	
	list<TFIXMSG>::iterator it ;
	int i = 0;
	for(i = 0, it = listFix.begin(); it != listFix.end(); ++it,i++)
	{
		int ntag = atoi((*it).msg_tag.c_str());
		switch(ntag) {
		case 112:
			strcpy(tHB.TestReqID, (*it).msg_value.c_str());
			break;
		default :
			break;
		}
	}

	string smsg = FixPrice::FixHB(&tHB, m_msgSeqNum);

	SendMsgToGW(smsg);
}

void FixEntry::DealRejectMsg(LISTFIXMSG listFix)
{
	TLogonRespMsg tLonResp;
	tLonResp.type = MSGTYPE_FIXMSG_LOGON_RESP;
	tLonResp.len = sizeof(TLogonRespMsg)-sizeof(TRawMsg);
	
	list<TFIXMSG>::iterator it ;
	int i = 0;
	for(i = 0, it = listFix.begin(); it != listFix.end(); ++it,i++)
	{
		int ntag = atoi((*it).msg_tag.c_str());
		switch(ntag) {
		case 373:
			tLonResp.FailCode = atoi((*it).msg_value.c_str());
			break;
		case 58:
			strcpy(tLonResp.Reason, (*it).msg_value.c_str());
			break;
		default:
			break;
		}
	}
	if (m_pPriceTarget)
			m_pPriceTarget->SendMsg(&tLonResp);

	TRACE_LOG("Reject");
}

void FixEntry::DealMDACKMsg(LISTFIXMSG listFix)
{
	LISTFIXMSG listtemp[10];
	TMDRespMsg tMDResp;
	tMDResp.type = MSGTYPE_FIXMSG_MD_RESP;
	tMDResp.len = sizeof(TMDRespMsg)-sizeof(TRawMsg);

	list<TFIXMSG>::iterator it ;
	int i = 0;
	int j = 0;
	int nEntryType = -1;
	for(i = 0, it = listFix.begin(); it != listFix.end(); ++it,i++)
	{
		int ntag = atoi((*it).msg_tag.c_str());
		switch(ntag) {
		case 262:
			strcpy(tMDResp.MDReqID ,(*it).msg_value.c_str());
			break;
		case 52:
			getMDResptime((*it).msg_value, tMDResp.year, tMDResp.month, tMDResp.day, tMDResp.hour, tMDResp.min, tMDResp.sec);
			break;
		case 279:
			j++;
			break;
		case 270:
			if (j >= 1)
				listtemp[j - 1].push_back((*it));
			break;
		default:
			break;
		}
	}

	tMDResp.bidprice	= -1;
	tMDResp.askprice	= -1;
	tMDResp.tradeprice	= -1;
	
	for (j = 0; j < 3; j++)
	{
		list<TFIXMSG>::iterator it ;
		int i = 0;
		
		int entryType = 0;
		for(i = 0, it = listtemp[j].begin(); it != listtemp[j].end(); ++it,i++)
		{
			int ntag = atoi((*it).msg_tag.c_str());
			switch(ntag) {
			case 270:
				tMDResp.askprice = atof((*it).msg_value.c_str());
				break;
			default:
				break;
			}
		}
	}
	if(tMDResp.bidprice != -1 || tMDResp.askprice != -1 || tMDResp.tradeprice != -1)
	{
		if (m_pPriceTarget)
			m_pPriceTarget->SendMsg(&tMDResp);
	}

	TRACE_LOG("MD ACK,m_pPriceTarget= %p, bidprice =%.2f, askprice =%.2f,tradeprice =%.2f,",
		m_pPriceTarget,tMDResp.bidprice ,tMDResp.askprice ,tMDResp.tradeprice);
}
/*
void FixEntry::DealMDACKMsg(LISTFIXMSG listFix)
{
	LISTFIXMSG listtemp[10];
	TMDRespMsg tMDResp;
	tMDResp.type = MSGTYPE_FIXMSG_MD_RESP;
	tMDResp.len = sizeof(TMDRespMsg)-sizeof(TRawMsg);

	list<TFIXMSG>::iterator it ;
	int i = 0;
	int j = 0;
	int nEntryType = -1;
	for(i = 0, it = listFix.begin(); it != listFix.end(); ++it,i++)
	{
		int ntag = atoi((*it).msg_tag.c_str());
		switch(ntag) {
		case 262:
			strcpy(tMDResp.MDReqID ,(*it).msg_value.c_str());
			break;
		case 52:
			getMDResptime((*it).msg_value, tMDResp.year, tMDResp.month, tMDResp.day, tMDResp.hour, tMDResp.min, tMDResp.sec);
			break;
		case 279:
			j++;
			break;
		case 269:
			nEntryType = atoi((*it).msg_value.c_str());
			break;
		case 278:
			if (j >= 1){
				listtemp[j - 1].push_back((*it));//ID
				string sEntryID = tMDResp.MDReqID;
				sEntryID += (*it).msg_value.c_str();
				if (nEntryType != -1 && FindMDEntryType(sEntryID) < 0)
				{
					string sEntryIDtemp = tMDResp.MDReqID;
					if(nEntryType == 0)
					{
						sEntryIDtemp +="00";//可能是bid 278=0
						m_EntryTypeIDMap.insert(MDEntryTypeIDMap::value_type(sEntryIDtemp, nEntryType));
					}
					if(nEntryType == 1)
					{
						sEntryIDtemp +="01";//可能是ask 278=0
						m_EntryTypeIDMap.insert(MDEntryTypeIDMap::value_type(sEntryIDtemp, nEntryType));
					}
					m_EntryTypeIDMap.insert(MDEntryTypeIDMap::value_type(sEntryID, nEntryType));
				}
			}
			break;
		case 270:
			if (j >= 1)
				listtemp[j - 1].push_back((*it));
			break;
		default:
			break;
		}
	}

	tMDResp.bidprice	= -1;
	tMDResp.askprice	= -1;
	tMDResp.tradeprice	= -1;
	
	int key278_0 = 0; //278等于0出现次数
	for (j = 0; j < 3; j++)
	{
		list<TFIXMSG>::iterator it ;
		int i = 0;
		
		int entryType = 0;
		for(i = 0, it = listtemp[j].begin(); it != listtemp[j].end(); ++it,i++)
		{
			int ntag = atoi((*it).msg_tag.c_str());
			switch(ntag) {
			case 278:{
				string sEntryID = tMDResp.MDReqID;
				if (atoi((*it).msg_value.c_str()) == 0)//278 = 0 有两种情况
				{
					sEntryID += (*it).msg_value.c_str();
					key278_0++;
					if (key278_0 == 1)//第一个是bid
						sEntryID +="0";
					else if (key278_0 == 2)//第一个是ask
						sEntryID +="1";
				}else
					sEntryID += (*it).msg_value.c_str();
				entryType =  FindMDEntryType(sEntryID);
				break;}	
			case 270:
				if (entryType == 0)
					tMDResp.bidprice = atof((*it).msg_value.c_str());
				else if (entryType == 1)
					tMDResp.askprice = atof((*it).msg_value.c_str());
				else if (entryType == 2)
					tMDResp.tradeprice = atof((*it).msg_value.c_str());
				break;
			default:
				break;
			}
		}
	}
	if(tMDResp.bidprice != -1 || tMDResp.askprice != -1 || tMDResp.tradeprice != -1)
	{
		if (m_pPriceTarget)
			m_pPriceTarget->SendMsg(&tMDResp);
	}

	TRACE_LOG("MD ACK,m_pPriceTarget= %p, bidprice =%.2f, askprice =%.2f,tradeprice =%.2f,",
		m_pPriceTarget,tMDResp.bidprice ,tMDResp.askprice ,tMDResp.tradeprice);
}
*/
void FixEntry::DealMDRejectMsg(LISTFIXMSG listFix)
{
	LISTFIXMSG listtemp[3];
	TMDRejectMsg tMDReject;
	tMDReject.type = MSGTYPE_FIXMSG_MD_REJECT;
	tMDReject.len = sizeof(TMDRejectMsg)-sizeof(TRawMsg);

	list<TFIXMSG>::iterator it ;
	int i = 0;
	int j = 0;
	for(i = 0, it = listFix.begin(); it != listFix.end(); ++it,i++)
	{
		int ntag = atoi((*it).msg_tag.c_str());
		switch(ntag) {
		case 262:
			strcpy(tMDReject.MDReqID ,(*it).msg_value.c_str());
			break;
		case 281:
			tMDReject.FailCode = atoi((*it).msg_value.c_str());
			break;
		case 58:
			strcpy(tMDReject.Reason ,(*it).msg_value.c_str());
			break;
		default:
			break;
		}
	}
	if (m_pPriceTarget)
		m_pPriceTarget->SendMsg(&tMDReject);
}

void FixEntry::DealOrderReport(LISTFIXMSG listFix)
{
	TOrderRespMsg tOrderResp;
	initOrderReport(tOrderResp);
	tOrderResp.type = MSGTYPE_FIXMSG_OR;
	tOrderResp.len = sizeof(TOrderRespMsg)-sizeof(TRawMsg);
	
	list<TFIXMSG>::iterator it ;
	int i = 0;
	for(i = 0, it = listFix.begin(); it != listFix.end(); ++it,i++)
	{
		int ntag = atoi((*it).msg_tag.c_str());
		switch(ntag) {
		case 11:
			strcpy(tOrderResp.CIOrdID,(*it).msg_value.c_str());
			break;
		case 41:
			strcpy(tOrderResp.OrigCIOrdID,(*it).msg_value.c_str());
			break;
		case 14:
			tOrderResp.CumQty = atoi((*it).msg_value.c_str());
			break;
		case 37:
			tOrderResp.OrderID = atoi((*it).msg_value.c_str());
			break;
		case 39:
			strcpy(tOrderResp.OrdStatus, (*it).msg_value.c_str());
			break;
		case 32:
			tOrderResp.LastShares = atoi((*it).msg_value.c_str());
			break;
		case 31:
			tOrderResp.LastPx = atof((*it).msg_value.c_str());
			break;
		case 6:
			tOrderResp.AvgPx = atof((*it).msg_value.c_str());
			break;
		case 60:
			getMDResptime((*it).msg_value, tOrderResp.year, tOrderResp.month, tOrderResp.day, tOrderResp.hour, tOrderResp.min, tOrderResp.sec);
			break;
		case 150:
			strcpy(tOrderResp.ExecType, (*it).msg_value.c_str());
			break;
		case 151:
			tOrderResp.LeavesQty = atoi((*it).msg_value.c_str());
			break;
		case 198:
			strcpy(tOrderResp.SecondaryOrderID, (*it).msg_value.c_str());
			break;
		case 58:
			strcpy(tOrderResp.Text, (*it).msg_value.c_str());
			break;
		default:
			break;
		}
	}
	if (m_pDealerTarget)
		m_pDealerTarget->SendMsg(&tOrderResp);

	TRACE_LOG("DealOrderReport");
}

void FixEntry::DealCancelReject(LISTFIXMSG listFix)
{
	TCancelRejectMsg tCancelReject;
	initCancelReject(tCancelReject);
	tCancelReject.type = MSGTYPE_FIXMSG_CANCELREJECT;
	tCancelReject.len = sizeof(TCancelRejectMsg)-sizeof(TRawMsg);
	
	list<TFIXMSG>::iterator it ;
	int i = 0;;
	int nRejResponseTo = 0;
	for(i = 0, it = listFix.begin(); it != listFix.end(); ++it,i++)
	{
		int ntag = atoi((*it).msg_tag.c_str());
		switch(ntag) {
		case 11:
			strcpy(tCancelReject.CIOrdID,(*it).msg_value.c_str());
			break;
		case 41:
			strcpy(tCancelReject.OrigCIOrdID,(*it).msg_value.c_str());
			break;
		case 37:
			tCancelReject.OrderID = atoi((*it).msg_value.c_str());
			break;
		case 39:
			strcpy(tCancelReject.OrdStatus, (*it).msg_value.c_str());
			break;
		case 434:
			nRejResponseTo = atoi((*it).msg_value.c_str());
			break;
		case 60:
			getMDResptime((*it).msg_value, tCancelReject.year, tCancelReject.month, tCancelReject.day, tCancelReject.hour, tCancelReject.min, tCancelReject.sec);
			break;
		case 102:
			tCancelReject.reason = atoi((*it).msg_value.c_str());
			break;
		case 58:
			strcpy(tCancelReject.Text, (*it).msg_value.c_str());
			break;
		default:
			break;
		}
	}
	if (m_pDealerTarget && nRejResponseTo != 2)
		m_pDealerTarget->SendMsg(&tCancelReject);

	TRACE_LOG("DealCancelReject, RejResponseTo = %d ",nRejResponseTo);
}

void FixEntry::DealResendSeq(LISTFIXMSG listFix)
{
	TGrapFillMsg tGF;
	tGF.type = MSGTYPE_FIXMSG_GRAPFILL;
	tGF.len = sizeof(TGrapFillMsg)-sizeof(TRawMsg);

	list<TFIXMSG>::iterator it ;
	int i = 0;
	for(i = 0, it = listFix.begin(); it != listFix.end(); ++it,i++)
	{
		int ntag = atoi((*it).msg_tag.c_str());
		switch(ntag) {
		case 16:
			tGF.newNo = atoi((*it).msg_value.c_str()) +1;
			break;
		default :
			break;
		}
	}

	string smsg = FixPrice::FixGrapFill(&tGF, m_msgSeqNum);

	SendMsgToGW(smsg);
}

void FixEntry::DealGrapFill(LISTFIXMSG listFix)
{
	list<TFIXMSG>::iterator it ;
	int i = 0;
	for(i = 0, it = listFix.begin(); it != listFix.end(); ++it,i++)
	{
		int ntag = atoi((*it).msg_tag.c_str());
		switch(ntag) {
		case 36:
			m_RemoteSeq = atoi((*it).msg_value.c_str()) -1 ;
			break;
		default :
			break;
		}
	}
}

void FixEntry::getMDResptime(string stime, U16 &year, U8 &month, U8 &day, U8 &hour, U8 &min, U8 &sec)
{
	strings pars, pars2;	
	strings::iterator it;		
	int npar = 0;
	FixPubTools::divide_string(stime.c_str() ,pars,"-",TRUE);
	for(npar = 0,it = pars.begin(); it != pars.end();it++,npar++)
	{
		switch(npar) {
		case 0:{//20120330
			    year  = atoi(((*it).substr(0,4)).c_str());
				month = atoi(((*it).substr(4,2)).c_str());
				day   = atoi(((*it).substr(6,2)).c_str());
				break;}
		case 1:{
			FixPubTools::divide_string((*it).c_str() ,pars2,":",TRUE);
			int npa = 0;
			strings::iterator it2;		
			for(npa = 0,it2 = pars2.begin(); it2 != pars2.end();it2++,npa++)
			{
				switch(npa) {
				case 0:	hour = atoi((*it2).c_str()); break;
				case 1: min  = atoi((*it2).c_str()); break;
				case 2: sec  = atoi((*it2).c_str()); break;
				default: break;
				}
			}
			break;}
		default: break;
		}
	}
}

int FixEntry::FindMDEntryType(string sEntryID)
{
	int ntype = -1;
	MDEntryTypeIDMap::iterator iter = m_EntryTypeIDMap.find(sEntryID);
	if( iter != m_EntryTypeIDMap.end())	
		ntype = iter->second;
	
	return ntype;
}


void FixEntry::initOrderReport(TOrderRespMsg &tOrderResp)
{
	memset(tOrderResp.CIOrdID, 0, sizeof(tOrderResp.CIOrdID));
	memset(tOrderResp.OrigCIOrdID, 0, sizeof(tOrderResp.OrigCIOrdID));
	memset(tOrderResp.OrdStatus, 0, sizeof(tOrderResp.OrdStatus));
	memset(tOrderResp.ExecType, 0, sizeof(tOrderResp.ExecType));
	tOrderResp.OrderID = 0;
	memset(tOrderResp.SecondaryOrderID, 0, sizeof(tOrderResp.ExecType));
	tOrderResp.LastShares = 0;
	tOrderResp.LeavesQty  = 0;
	tOrderResp.CumQty  = 0;
	tOrderResp.LastPx  = -1;
	tOrderResp.AvgPx   = -1;
	tOrderResp.year = 0;
	tOrderResp.month= 0;
	tOrderResp.day= 0;
	tOrderResp.hour= 0;
	tOrderResp.min= 0;
	tOrderResp.sec= 0;
	memset(tOrderResp.Text, 0, sizeof(tOrderResp.Text));
}

void FixEntry::initCancelReject(TCancelRejectMsg &tCancelReject)
{
	memset(tCancelReject.CIOrdID, 0, sizeof(tCancelReject.CIOrdID));
	memset(tCancelReject.OrigCIOrdID, 0, sizeof(tCancelReject.OrigCIOrdID));
	memset(tCancelReject.OrdStatus, 0, sizeof(tCancelReject.OrdStatus));
	tCancelReject.OrderID   = 0;
	tCancelReject.reason   = 0;
	tCancelReject.year = 0;
	tCancelReject.month= 0;
	tCancelReject.day= 0;
	tCancelReject.hour= 0;
	tCancelReject.min= 0;
	tCancelReject.sec= 0;
	memset(tCancelReject.Text, 0, sizeof(tCancelReject.Text));
}