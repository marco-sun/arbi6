#include "PatsApiPriceSource.h"
#include "logstore.h"
#include "Contract.h"
#include "patsapimsgdef.h"
#include "SimPatsApiEngine.h"
#include <time.h>

PatsApiPriceSource::PatsApiPriceSource(void)
{
	isStarted = false;
	isThreadStarted = false;
	m_pPatsApiEngine = NULL;
	isLogonOK = false;
	hLogin = ::CreateEvent(NULL,TRUE,FALSE,NULL);
	InitializeCriticalSection(&cs);
	convertNameMap.clear();
	contractsRetryMap.clear();
}


PatsApiPriceSource::~PatsApiPriceSource(void)
{
	DeleteCriticalSection(&cs);
	::CloseHandle(hLogin);
	if(isThreadStarted)
		StopThread();
}

void PatsApiPriceSource::ReadIniConfig()
{ // 登录需要的参数 username,password
	m_useSimEngine = 0;
	m_useSimEngine = GetPrivateProfileIntA("PATSAPIPS","SIMENGINE_PS",0,".\\AIB.ini");
	memset(m_szUsername,0,sizeof(m_szUsername));
	memset(m_szPassword,0,sizeof(m_szPassword));
	TRACE_LOG("PatsApiPriceSource ReadIniConfig UseSimEngine:%d.",m_useSimEngine);
}

void PatsApiPriceSource::start()
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
//		m_pFixEngine = FixEntry::getPriceEngineInstance((IMsg*)this);
	}
	else
	{
		m_pPatsApiEngine = SimPatsApiEngine::getPriceEngineInstance((IMsg*)this);
	}

	// 组织登录消息，主动向PatsApi基础架构发起登录
	TPatsApiLogonMsg sender;
	sender.type = MSGTYPE_PATSAPI_LOGON;
	sender.len = sizeof(TPatsApiLogonMsg)-sizeof(TRawMsg);
	strncpy(sender.uid,m_szUsername,255);
	strncpy(sender.pwd,m_szPassword,255);
	
	if(m_pPatsApiEngine) 
	{
		::ResetEvent(hLogin);
		BOOL ret = m_pPatsApiEngine->SendMsg(&sender);
		// 吊住caller，等待登录成功消息
		DWORD dwRet = WaitForSingleObject(hLogin,300*1000); //wait 120 secs at most.
		if (dwRet == WAIT_TIMEOUT)
		{//Engine响应超时，记告警日志，退出程序
			TRACE_ALARM("PatsApiPriceSource Starts timeout (300 secs)!");
			exit(1);
		}
		else if (dwRet == WAIT_OBJECT_0)
		{//Engine响应
			if(isLogonOK)
			{
				TRACE_LOG("PatsApiPriceSource Starts OK (LOGON OK).");	
				isStarted = true;
			}
			else
			{
				TRACE_LOG("PatsApiPriceSource Starts FAIL (LOGON FAIL).");
				exit(1);
			}
		}
		else //WAIT_ABANDONED
		{
			TRACE_ALARM("PatsApiPriceSource Starts failed (WAIT_ABANDONED)!");
			exit(1);
		}
	}
	else
	{ // 记错误日志，终止程序
		TRACE_LOG("[ERROR] PatsApiPriceSource can NOT find the Engine.");
		exit(1);
	}
	
	
}

void PatsApiPriceSource::add(string contract)
{
	bool isDup=false;
	Contract *pContract = Contract::get(contract);
	//TODO: ZHOUJIE
	//string patsapiName = pContract->getFixName();
	string patsapiName = pContract->getPatsApiName();

	if(patsapiName=="") 
	{
		TRACE_LOG("[ERROR] PatsApiPriceSource can NOT handle this contract %s.",contract.c_str());
		return;
	}
	EnterCriticalSection(&cs);
	//看是否是重复的请求
	if(convertNameMap.count(patsapiName)!=0)
	{
		isDup = true;
	}
	if(!isDup)
	{
		convertNameMap.insert(make_pair(patsapiName,contract));
		string retryCount="0";
		contractsRetryMap.insert(make_pair(patsapiName,retryCount));
	}
	LeaveCriticalSection(&cs);
	if(isDup) return;
	SendListenContractMsg(patsapiName);
}

// ************后台消息线程******************
// patsapi基础架构回来的所有消息在此处处理
bool PatsApiPriceSource::OnMsg(TRawMsg *msg, IMsg *remote_src)
{
	//TRACE_LOG("PatsApiPriceSource recv msg.");
	if ( msg->type == MSGTYPE_PATSAPI_LOGON_RESP)
	{
		TRACE_LOG("  Is MSGTYPE_PATSAPI_LOGON_RESP.");
		TPatsApiLogonRespMsg* pLogonRespMsg = (TPatsApiLogonRespMsg*)msg;
		if(pLogonRespMsg->FailCode == 0)
		{
			isLogonOK = true;
		}
		else
		{
			isLogonOK = false;
			TRACE_LOG("PatsApiPriceSource Logon fail. code:%d, text:%s.",pLogonRespMsg->FailCode,pLogonRespMsg->Reason);
		}
		::SetEvent(hLogin);
	}
	else if ( msg->type == MSGTYPE_PATSAPI_MD_SRESP)
	{
		TPatsApiMDSRespMsg* pMDRespMsg = (TPatsApiMDSRespMsg*)msg;
		//TRACE_LOG("  Is MSGTYPE_PATSAPI_MD_SRESP about %s %s %s.",pMDRespMsg->exchangeName,pMDRespMsg->contractName,pMDRespMsg->MaturityMonthYear);
		string contract = ConvertContract(AssemblePatsApiName(pMDRespMsg->exchangeName,pMDRespMsg->contractName,pMDRespMsg->MaturityMonthYear));
		if(contract!="")
		{
			time_t _now = time(NULL);
			tm *t = localtime(&_now);
			t->tm_hour = pMDRespMsg->hour;
			t->tm_min = pMDRespMsg->min;
			t->tm_sec = pMDRespMsg->sec;
			time_t calendar = mktime(t);
			//time_t calendar = time(NULL);
			TRACE_LOG("    Contract: %s values(ask,bid,last): %.2f,%.2f,%.2f.",contract.c_str(),pMDRespMsg->askprice,pMDRespMsg->bidprice,pMDRespMsg->tradeprice);
		
		
			Contract* pContract = Contract::get(contract);
			if(pContract!=NULL)
			{
				//TRACE_LOG("    setPrice(ask %.2f,bid %.2f,lastPrice %.2f).",pMDRespMsg->askprice,pMDRespMsg->bidprice,pMDRespMsg->tradeprice);
				if(strcmp(pMDRespMsg->contractName,"CMX COP")==0)
				{
					pContract->setPrice(calendar,pMDRespMsg->askprice * 100,pMDRespMsg->bidprice * 100,pMDRespMsg->tradeprice * 100);
					TRACE_LOG("PatsApiPriceSource convert CMX COP market data.");
				}
				else
				{
					pContract->setPrice(calendar,pMDRespMsg->askprice,pMDRespMsg->bidprice,pMDRespMsg->tradeprice);
				}
			}
		}
	}
	else if ( msg->type == MSGTYPE_PATSAPI_MD_REJECT)
	{
		TPatsApiMDRejectMsg* pMDRejectMsg = (TPatsApiMDRejectMsg*)msg;
		string patsapiName = AssemblePatsApiName(pMDRejectMsg->exchangeName,pMDRejectMsg->contractName,pMDRejectMsg->MaturityMonthYear);
		TRACE_LOG("[WARNING] PatsApiPriceSource recv REJECT msg when add %s.code:%d, reason:%s.",patsapiName.c_str(),pMDRejectMsg->FailCode,pMDRejectMsg->Reason);
		if(patsapiName!="")
		{
		EnterCriticalSection(&cs);
		string retryCount = contractsRetryMap[patsapiName];
		int modifyCount = atoi(retryCount.c_str()) + 1;
		char tempCount[8];
		sprintf(tempCount,"%d",modifyCount);
		string theNewCount = tempCount;
		contractsRetryMap[patsapiName]=theNewCount;
		LeaveCriticalSection(&cs);
		if(modifyCount>=3)
		{
			EnterCriticalSection(&cs);
			convertNameMap.erase(patsapiName);
			contractsRetryMap.erase(patsapiName);
			LeaveCriticalSection(&cs);
			TRACE_LOG("[WARNING] adding %s FAILED, cause been reject 3 times.",patsapiName.c_str());

		}
		else
		{
			SendListenContractMsg(patsapiName);
		}
		}
	}
	else if ( msg->type == MSGTYPE_PATSAPI_ENGINE_STATUS)
	{
		TPatsApiEngineStatusMsg* pEngineStatus = (TPatsApiEngineStatusMsg*)msg;
		TRACE_LOG("PatsApiDealer[引擎状态通知] PATSAPI引擎状态为%d.",pEngineStatus->status);
	}
	else
	{
		TRACE_LOG("Unknown message 0x%x received from \'%s\'", 
			msg->type, remote_src ? remote_src->GetName() : "[Local]");
	}
	return true;
}

void PatsApiPriceSource::SendListenContractMsg(string& patsapiName)
{
	// patsapi格式名称转为patsapi合约三要素
	// patsapi格式名称为 cname-cCCYYMM@cexchange
	string cname = patsapiName.substr(0,patsapiName.find_first_of('-'));
	string cexchange = patsapiName.substr(patsapiName.find_first_of('@')+1);
	string cCCYYMM = patsapiName.substr(patsapiName.find_first_of('-')+1,patsapiName.find_first_of('@')-patsapiName.find_first_of('-')-1);
	TRACE_LOG("PatsApiPriceSource parse patsapiName[%s] to [%s,%s,%s].",patsapiName.c_str(),cname.c_str(),cexchange.c_str(),cCCYYMM.c_str());

	// 组织订阅消息，主动向PatsApi基础架构发起合约订阅
	TPatsApiMDReqMsg sender;
	sender.type = MSGTYPE_PATSAPI_MD;
	sender.len = sizeof(TPatsApiMDReqMsg)-sizeof(TRawMsg);
	sender.requestType = 0;
	sender.method = 0;
	strncpy(sender.exchangeName,cexchange.c_str(),10);
	strncpy(sender.contractName,cname.c_str(),10);
	strncpy(sender.MaturityMonthYear,cCCYYMM.c_str(),50);
	
	if(m_pPatsApiEngine) 
	{
		m_pPatsApiEngine->SendMsg(&sender);
	}
	else
	{ // 从map中去除该合约
		TRACE_LOG("[ERROR] PatsApiPriceSource can NOT find the Engine when add a contract.");
		EnterCriticalSection(&cs);
		convertNameMap.erase(patsapiName);
		contractsRetryMap.erase(patsapiName);
		LeaveCriticalSection(&cs);
	}
}

// 将patsapi格式的合约名转换为arbi格式，若该合约未在此网关注册过，则返回空串
string PatsApiPriceSource::ConvertContract(string& patsapiName)
{
	string ret="";
	EnterCriticalSection(&cs);
	map<string,string>::iterator iter = convertNameMap.find(patsapiName);
	if(iter != convertNameMap.end())
	{
		ret = iter->second;
	}		
	LeaveCriticalSection(&cs);
	return ret;
}

// 将patsapi返回的合约三要素组合为patsapi格式的合约名，若该合约未在此网关注册过，则返回空串
// patsapi格式名称为 contract-datetime@exchange
string PatsApiPriceSource::AssemblePatsApiName(const char* exchange, const char* contract, const char* datetime)
{
	char buff[80];
	sprintf(buff,"%s-%s@%s",contract,datetime,exchange);
	string patsapiName=buff;
	EnterCriticalSection(&cs);
	//看是否已注册的合约
	if(convertNameMap.count(patsapiName)==0)
	{ // 未注册
		patsapiName = "";
	}
	LeaveCriticalSection(&cs);
	return patsapiName;
}





