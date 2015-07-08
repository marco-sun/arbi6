
#include "FixPatsPriceSource.h"
#include "logstore.h"
#include "Contract.h"
#include "fixmsgdef.h"
#include "SimFixEngine.h"
#include "FixEntry.h"
#include <time.h>

FixPatsPriceSource::FixPatsPriceSource(void)
{
	isStarted = false;
	isThreadStarted = false;
	m_pFixEngine = NULL;
	isLogonOK = false;
	hLogin = ::CreateEvent(NULL,TRUE,FALSE,NULL);
	InitializeCriticalSection(&cs);
	contractsMap.clear();
	contractsRetryMap.clear();
}


FixPatsPriceSource::~FixPatsPriceSource(void)
{
	DeleteCriticalSection(&cs);
	::CloseHandle(hLogin);
	if(isThreadStarted)
		StopThread();

}

void FixPatsPriceSource::ReadIniConfig()
{ // 登录需要的参数 HeartBeatInterval,username,password
	m_HeartBeatInterval = 30;
	m_useSimEngine = 0;
	memset(m_szUsername,   0, sizeof(m_szUsername));
	memset(m_szPassword,   0, sizeof(m_szPassword));

	GetPrivateProfileStringA("FIXPATSGW", "LOGON_UID", "patsusername", m_szUsername, 16, ".\\AIB.ini");
	GetPrivateProfileStringA("FIXPATSGW", "LOGON_PWD", "patspassword", m_szPassword, 16, ".\\AIB.ini");
	m_HeartBeatInterval = GetPrivateProfileIntA("FIXPATSGW","LOGON_HBINT",30,".\\AIB.ini");
	m_useSimEngine = GetPrivateProfileIntA("FIXPATSGW","SIMENGINE_PS",0,".\\AIB.ini");
	TRACE_LOG("FixPatsPriceSource ReadIniConfig UID:%s,PWD:%s,HeartBeatInt:%d,UseSimEngine:%d.",m_szUsername,m_szPassword,m_HeartBeatInterval,m_useSimEngine);
}

void FixPatsPriceSource::start()
{
	// 不可重入保护
	if(isStarted) return;
	
	// 读取登录配置
	ReadIniConfig();
	// 开启后台消息线程
	if(!isThreadStarted)
	{
		StartThread();
		isThreadStarted = true;
	}
	if(this->m_useSimEngine == 0)
	{
		m_pFixEngine = FixEntry::getPriceEngineInstance((IMsg*)this);
	}
	else
	{
		m_pFixEngine = SimFixEngine::getPriceEngineInstance((IMsg*)this);
	}

	// 组织登录消息，主动向Fix基础架构发起登录
	TLogonMsg sender;
	sender.type = MSGTYPE_FIXMSG_LOGON;
	sender.len = sizeof(TLogonMsg)-sizeof(TRawMsg);
	sender.EncryptMethod = 0;
	sender.HeartBtInt = m_HeartBeatInterval;
	sprintf(sender.RawData,"%s/%s",m_szUsername,m_szPassword);
	sender.RawDataLength = strlen(sender.RawData);
	sender.ResetSeqNumFlag = 'Y';
	if(m_pFixEngine) 
	{
		::ResetEvent(hLogin);
		BOOL ret = m_pFixEngine->SendMsg(&sender);
		// 吊住caller，等待登录成功消息
		DWORD dwRet = WaitForSingleObject(hLogin,180*1000); //wait 180 secs at most.
		if (dwRet == WAIT_TIMEOUT)
		{//Engine响应超时，记告警日志，退出程序
			TRACE_ALARM("FixPatsPriceSource Starts timeout (180secs)!");
			exit(1);
		}
		else if (dwRet == WAIT_OBJECT_0)
		{//Engine响应
			if(isLogonOK)
			{
				TRACE_LOG("FixPatsPriceSource Starts OK (LOGON OK).");	
				isStarted = true;
			}
			else
			{
				TRACE_LOG("FixPatsPriceSource Starts FAIL (LOGON FAIL).");
				exit(1);
			}
		}
		else //WAIT_ABANDONED
		{
			TRACE_ALARM("FixPatsPriceSource Starts failed (WAIT_ABANDONED)!");
			exit(1);
		}
	}
	else
	{ // 记错误日志，终止程序
		TRACE_LOG("[ERROR] FixPatsPriceSource can NOT find the Fix Engine.");
		exit(1);
	}
	
	
}

void FixPatsPriceSource::add(string contract)
{
	bool isDup=false;
	EnterCriticalSection(&cs);
	//看是否是重复的请求
	if(contractsMap.count(contract)!=0)
	{
		isDup = true;
	}
	if(!isDup)
	{
		PriceNote pn;
		pn.ask = 0; pn.bid = 0; pn.lastPrice = 0;
		contractsMap.insert(make_pair(contract,pn));
		string retryCount="0";
		contractsRetryMap.insert(make_pair(contract,retryCount));
	}
	LeaveCriticalSection(&cs);
	if(isDup) return;
	SendListenContractMsg(contract);
}

// ************后台消息线程******************
// Fix网关回来的所有消息在此处处理
bool FixPatsPriceSource::OnMsg(TRawMsg *msg, IMsg *remote_src)
{
	TRACE_LOG("FixPatsPriceSource recv msg.");
	if ( msg->type == MSGTYPE_FIXMSG_LOGON_RESP)
	{
		TRACE_LOG("  Is MSGTYPE_FIXMSG_LOGON_RESP.");
		TLogonRespMsg* pLogonRespMsg = (TLogonRespMsg*)msg;
		if(pLogonRespMsg->FailCode == 0)
		{
			isLogonOK = true;
		}
		else
		{
			isLogonOK = false;
			TRACE_LOG("FixPatsPriceSource Logon fail. code:%d, text:%s.",pLogonRespMsg->FailCode,pLogonRespMsg->Reason);
		}
		::SetEvent(hLogin);
	}
	else if ( msg->type == MSGTYPE_FIXMSG_MD_RESP)
	{
		TRACE_LOG("  Is MSGTYPE_FIXMSG_MD_RESP.");
		bool isReg = true;
		TMDRespMsg* pMDRespMsg = (TMDRespMsg*)msg;
		string contract = ID2Contract(pMDRespMsg->MDReqID);
		string pricetype = ID2PriceType(pMDRespMsg->MDReqID);
		TRACE_LOG("    Contract: %s pricetype: %s values: %.2f.",contract.c_str(),pricetype.c_str(),pMDRespMsg->askprice);

		PriceNote pn;
		EnterCriticalSection(&cs);
		map<string, PriceNote>::iterator iter = contractsMap.find(contract);
		if(iter == contractsMap.end())
		{
			isReg = false;
		}
		else
		{
			isReg = true;
			pn = iter->second;
		}		
		LeaveCriticalSection(&cs);
		if(!isReg)
		{
			TRACE_LOG("    Unreg contract, ignore.");
			return true;
		}
		if(pricetype=="0") pn.bid = pMDRespMsg->askprice;
		if(pricetype=="1") pn.ask = pMDRespMsg->askprice;
		if(pricetype=="2") pn.lastPrice = pMDRespMsg->askprice;
		// 回填价格
		EnterCriticalSection(&cs);
		map<string, PriceNote>::iterator iter1 = contractsMap.find(contract);
		if(iter1 != contractsMap.end())
		{
			iter1->second = pn;
		}		
		LeaveCriticalSection(&cs);

		struct tm t;
		t.tm_year = pMDRespMsg->year - 1900;
		t.tm_mon = pMDRespMsg->month - 1;
		t.tm_mday = pMDRespMsg->day;
		t.tm_hour = pMDRespMsg->hour;
		t.tm_min = pMDRespMsg->min;
		t.tm_sec = pMDRespMsg->sec;
		t.tm_isdst = 0;
		time_t calendar = mktime(&t);
		
		Contract* pContract = Contract::get(contract);

		if(pContract!=NULL)
		{
			TRACE_LOG("    setPrice(ask %.2f,bid %.2f,lastPrice %.2f).",pn.ask,pn.bid,pn.lastPrice);
			pContract->setPrice(calendar,pn.ask,pn.bid,pn.lastPrice);
		}

	}
	else if ( msg->type == MSGTYPE_FIXMSG_MD_REJECT)
	{
		TMDRejectMsg* pMDRejectMsg = (TMDRejectMsg*)msg;
		string contract = ID2Contract(pMDRejectMsg->MDReqID);
		TRACE_LOG("[WARNING] FixPatsPriceSource recv REJECT msg when add %s contract.",contract.c_str());
		EnterCriticalSection(&cs);
		string retryCount = contractsRetryMap[contract];
		int modifyCount = atoi(retryCount.c_str()) + 1;
		char tempCount[8];
		sprintf(tempCount,"%d",modifyCount);
		string theNewCount = tempCount;
		contractsRetryMap[contract]=theNewCount;
		LeaveCriticalSection(&cs);
		if(modifyCount>=3)
		{
			EnterCriticalSection(&cs);
			contractsMap.erase(contract);
			contractsRetryMap.erase(contract);
			LeaveCriticalSection(&cs);
			TRACE_LOG("[WARNING] adding %s contract FAILED, cause been reject 3 times.",contract.c_str());

		}
		else
		{
			SendListenContractMsg(contract);
		}
	}
	else
	{
		TRACE_LOG("Unknown message 0x%x received from \'%s\'", 
			msg->type, remote_src ? remote_src->GetName() : "[Local]");
	}
	return true;
}

void FixPatsPriceSource::SendListenContractMsg(string& contract)
{
	// 组织订阅消息，主动向Fix基础架构发起合约订阅
			Contract *pContract = Contract::get(contract);
			string fixpatsName = pContract->getFixName();
			string cname = fixpatsName.substr(0,fixpatsName.find_first_of('-'));
			string cexchange = fixpatsName.substr(fixpatsName.find_first_of('@')+1);
			string cCCYYMM = fixpatsName.substr(fixpatsName.find_first_of('-')+1,
			fixpatsName.find_first_of('@')-fixpatsName.find_first_of('-')-1);
			TRACE_LOG("FixPatsPriceSource parse [%s] to [%s,%s,%s].",
				contract.c_str(),cname.c_str(),cexchange.c_str(),cCCYYMM.c_str());


	TMDReqMsg sender;
	sender.type = MSGTYPE_FIXMSG_MD;
	sender.len = sizeof(TMDReqMsg)-sizeof(TRawMsg);
	
	sender.SubscriptionRequestType = 1;
	sender.MarketDepth = 1;
	sender.MDUpdateType = 1;
	sender.NoMDEntryTypes = 1;
	strcpy(sender.Symbol,cname.c_str());
	sprintf(sender.SecurityType,"FUT");
	strcpy(sender.MaturityMonthYear,cCCYYMM.c_str());
	strcpy(sender.SecurityExchange,cexchange.c_str());

	string reqID = assembleID(contract,"0");
	string reqID2 = assembleID(contract,"1");
	string reqID3 = assembleID(contract,"2");

	if(m_pFixEngine) 
	{
		strcpy(sender.MDReqID,reqID.c_str());
		sender.MDEntryType[0] = 0; 
		m_pFixEngine->SendMsg(&sender);

		strcpy(sender.MDReqID,reqID2.c_str());
		sender.MDEntryType[0] = 1; 
		m_pFixEngine->SendMsg(&sender);

		strcpy(sender.MDReqID,reqID3.c_str());
		sender.MDEntryType[0] = 2; 
		m_pFixEngine->SendMsg(&sender);

	}
	else
	{ // 从map中去除该合约
		TRACE_LOG("[ERROR] FixPatsPriceSource can NOT find the Fix Engine when add a contract.");
		EnterCriticalSection(&cs);
		contractsMap.erase(contract);
		contractsRetryMap.erase(contract);
		LeaveCriticalSection(&cs);
	}
}


string FixPatsPriceSource::assembleID(string contract,string pricetype)
{ // contract+ "_" + time[HHMMSS]+"_"+0
	// pricetype: 0 - bid, 1 - ask, 2 - last traded price
	//return "SPname_081005_0";
	time_t t = time(NULL);
	struct tm *date = gmtime(&t);
	char timebuff[8];
	sprintf(timebuff,"_%02d%02d%02d",date->tm_hour,date->tm_min,date->tm_sec);
	string ret =  contract  + timebuff + "_" + pricetype;
	return ret;
}

string FixPatsPriceSource::ID2Contract(const char* id)
{	//"SPname_081005_0";
	//return "SPname";
	string ret = "";
	string work = id;
	size_t firstidx = work.find_first_of("_");
	return work.substr(0,firstidx);
}

string  FixPatsPriceSource::ID2PriceType(const char* id)
{	//"SPname_081005_0";
	//return "0";
	string ret = "";
	string work = id;
	size_t firstidx = work.find_last_of("_");
	return work.substr(firstidx+1);
}




