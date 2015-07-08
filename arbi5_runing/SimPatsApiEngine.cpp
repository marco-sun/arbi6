#include "SimPatsApiEngine.h"
#include "LogStore.h"
#include <time.h>

//*******************static info****************************
// 网络状态标记 ptLinkOpened 1, ptLinkConnecting 2, ptLinkConnected 3, ptLinkClosed 4, ptLinkInvalid 5 
static const char* szLinkStatusDesc[] = {"N/A","ptLinkOpened","ptLinkConnecting","ptLinkConnected","ptLinkClosed","ptLinkInvalid"};

// Order States ptQueued 1, ptSent 2, ptWorking 3, ptRejected 4, ptCancelled 5, ptBalCancelled 6, ptPartFilled 7, ptFilled 8, ptCancelPending 9, 
// ptAmendPending 10, ptUnconfirmedFilled 11, ptUnconfirmedPartFilled 12, ptHeldOrder 13, ptCancelHeldOrder 14 
// ptTransferred 20, ptExternalCancelled 24 (added for GT)
static const char* szOrderStatusDesc[] = {"N/A","ptQueued","ptSent","ptWorking","ptRejected","ptCancelled","ptBalCancelled","ptPartFilled","ptFilled","ptCancelPending",
											"ptAmendPending","ptUnconfirmedFilled","ptUnconfirmedPartFilled","ptHeldOrder","ptCancelHeldOrder","15","16","17","","",
											"ptTransferred","21","22","23","ptExternalCancelled"};

// engine status: 0-born(start state), 1-intialised, 2-log in, 3-partial log out, 4-total log out(end state)
static const char* szEngineStatusDesc[] = {"born","intialised","log in","partial log out","total log out"};

//****************callbacks from the API*************************
// 登录后数据下载完成时得到通知。作为登录成功的标志。
void WINAPI DataDLComplete()
{
	TRACE_LOG("[ENGINE][CALLBACK]DataDLComplete report.");
	IMsg* pEngine = NULL;
	pEngine = SimPatsApiEngine::getEngineInstance();
	TRawMsg sender;
	INIT_MSG_HEADER(&sender);
	sender.type = MSGTYPE_PATSAPI_INNER_DL;
	if(pEngine) pEngine->SendMsg(&sender);
}

// 登录请求结果的通知。根据其中状态码作为登录失败的标志。
void WINAPI LogonStatus()
{
	TRACE_LOG("[ENGINE][CALLBACK]Logon report.");
	IMsg* pEngine = NULL;
	pEngine = SimPatsApiEngine::getEngineInstance();
	TRawMsg sender;
	INIT_MSG_HEADER(&sender);
	sender.type = MSGTYPE_PATSAPI_INNER_LOGON;
	if(pEngine) pEngine->SendMsg(&sender);
}

// 往主服务器的网络连接准备好的通知。应作为开始发起登录动作的标志。
// 状态标记 ptLinkOpened 1, ptLinkConnecting 2, ptLinkConnected 3, ptLinkClosed 4, ptLinkInvalid 5 
void WINAPI HostLinkStateChange(LinkStateStructPtr data)
{
	TRACE_LOG("[ENGINE][CALLBACK]HostLinkStateChange report. from %s to %s.",szLinkStatusDesc[data->OldState],szLinkStatusDesc[data->NewState]);
	IMsg* pEngine = NULL;
	pEngine = SimPatsApiEngine::getEngineInstance();
	TPatsApiInnerHostMsg sender;
	sender.type = MSGTYPE_PATSAPI_INNER_HOSTLINK;
	sender.len = sizeof(TPatsApiInnerHostMsg)-sizeof(TRawMsg);
	sender.status = data->NewState;
	if(pEngine) pEngine->SendMsg(&sender);
}

// 往价格服务器的网络连接准备好的通知。因价格订阅会缓存在本地api中，所以仅作日志输出，业务逻辑可忽略。
void WINAPI PriceLinkStateChange(LinkStateStructPtr data)
{
	TRACE_LOG("[ENGINE][CALLBACK]PriceLinkStateChange report. from %s to %s.",szLinkStatusDesc[data->OldState],szLinkStatusDesc[data->NewState]);
	IMsg* pEngine = NULL;
	pEngine = SimPatsApiEngine::getEngineInstance();
	TPatsApiInnerPriceMsg sender;
	sender.type = MSGTYPE_PATSAPI_INNER_PRICELINK;
	sender.len = sizeof(TPatsApiInnerPriceMsg)-sizeof(TRawMsg);
	sender.status = data->NewState;
	if(pEngine) pEngine->SendMsg(&sender);
}

// 价格有更新的通知。应作为启动价格变动逻辑的标志。
void WINAPI PriceUpdate(PriceUpdStructPtr data)
{
	//TRACE_LOG("[ENGINE][CALLBACK]PriceUpdate report. Exchange=%s Contract=%s ContractDate=%s",data->ExchangeName,data->ContractName,data->ContractDate);
	IMsg* pEngine = NULL;
	pEngine = SimPatsApiEngine::getEngineInstance();
	TPatsApiInnerMDUpdMsg sender;
	sender.type = MSGTYPE_PATSAPI_INNER_MDUPD;
	sender.len = sizeof(TPatsApiInnerMDUpdMsg)-sizeof(TRawMsg);
	strcpy(sender.exchangeName,data->ExchangeName);
	strcpy(sender.contractName,data->ContractName);
	strcpy(sender.MaturityMonthYear,data->ContractDate);
	if(pEngine) pEngine->SendMsg(&sender);
}

// 各种用户和告警消息的通知。应作为业务逻辑日志输出，或将消息上送engine用户。
void WINAPI Message(MsgIDPtr MsgID)
{ 
	TRACE_LOG("[ENGINE][CALLBACK]Message report. ID=%s",MsgID);
	IMsg* pEngine = NULL;
	pEngine = SimPatsApiEngine::getEngineInstance();
	TPatsApiInnerMsgMsg sender;
	sender.type = MSGTYPE_PATSAPI_INNER_MESSAGE;
	sender.len = sizeof(TPatsApiInnerMsgMsg)-sizeof(TRawMsg);
	strcpy(sender.patsMsgID,(const char *)(MsgID));
	if(pEngine) pEngine->SendMsg(&sender);
}

// 报单状态变化的通知。应继续分析报单状态，关注报单的几个终态是否达到。
void WINAPI OrderCallback(OrderUpdStructPtr OrderID)
{
	TRACE_LOG("[ENGINE][CALLBACK]OrderCallback report. OrderID %s, oldID %s, status: %s.",OrderID->OrderID,OrderID->OldOrderID,szOrderStatusDesc[OrderID->OrderStatus]);
	IMsg* pEngine = NULL;
	pEngine = SimPatsApiEngine::getEngineInstance();
	TPatsApiInnerORMsg sender;
	memset(&sender,0,sizeof(TPatsApiInnerORMsg));
	sender.type = MSGTYPE_PATSAPI_INNER_OR;
	sender.len = sizeof(TPatsApiInnerORMsg)-sizeof(TRawMsg);
	strncpy(sender.patsOrderID,OrderID->OrderID,10);
	strncpy(sender.oldPatsOrderID,OrderID->OldOrderID,10);
	if(pEngine) pEngine->SendMsg(&sender);
}

// 报单成交通知。应分析成交细节信息，启动报单成交汇报逻辑。
void WINAPI Fill(FillUpdStructPtr FillID)
{
	TRACE_LOG("[ENGINE][CALLBACK]Fill report. Order %s, Fill %s",FillID->OrderID,FillID->FillID);
	IMsg* pEngine = NULL;
	pEngine = SimPatsApiEngine::getEngineInstance();
	TPatsApiInnerFRMsg sender;
	memset(&sender,0,sizeof(TPatsApiInnerFRMsg));
	sender.type = MSGTYPE_PATSAPI_INNER_FR;
	sender.len = sizeof(TPatsApiInnerFRMsg)-sizeof(TRawMsg);
	strncpy(sender.patsOrderID,FillID->OrderID,10);
	strncpy(sender.patsFillID,FillID->FillID,70);
	if(pEngine) pEngine->SendMsg(&sender);
}

// 当一个新合约进入市场时通知。
void WINAPI ContractAdded(ContractUpdStructPtr data)
{
	//will fire for intra day strategy creations for example at Liffe
	//report state changes
	TRACE_LOG("[ENGINE][CALLBACK]ContractAdded report. Exchange=%s Contract=%s ContractDate=%s. No Action.",data->ExchangeName,data->ContractName,data->ContractDate);
}

// 当一个旧合约退出市场时通知。
void WINAPI ContractDeleted(ContractUpdStructPtr data)
{
	TRACE_LOG("[ENGINE][CALLBACK]ContractDeleted report. Exchange=%s Contract=%s ContractDate=%s. No Action.",data->ExchangeName,data->ContractName,data->ContractDate);
}

// 合约的交易状态变更时通知。可以得到开盘，收盘，集合竞价等等状态
void WINAPI StatusChange(StatusUpdStructPtr data)
{
	TRACE_LOG("[ENGINE][CALLBACK]ContractStatusChange report. Exchange=%s Contract=%s ContractDate=%s satus=%d. No Action.",
		data->ExchangeName,data->ContractName,data->ContractDate,data->Status);
}

// 当服务器主动将api退登时通知。必须处理，通知engine用户，记录日志，并永久关闭engine。
void WINAPI ForcedLogout()
{
	//you must handle this call back as it is received if the application is left logged in
	//over night. The TAS will force log out the API and your app should know that the API
	//has been loged out.
	TRACE_LOG("[ENGINE][CALLBACK]ForcedLogout report.");
	IMsg* pEngine = NULL;
	pEngine = SimPatsApiEngine::getEngineInstance();
	TRawMsg sender;
	INIT_MSG_HEADER(&sender);
	sender.type = MSGTYPE_PATSAPI_INNER_FORCELOGOUT;
	if(pEngine) pEngine->SendMsg(&sender);
}
//***********************************************************************************************



SimPatsApiEngine* SimPatsApiEngine::pStaticEngine = NULL;

IMsg* SimPatsApiEngine::getPriceEngineInstance(IMsg *priceSource)
{
	if(pStaticEngine==NULL)
	{
		// create patsengine instance
		pStaticEngine = new SimPatsApiEngine();
		pStaticEngine->ReadIniConfig();
		if(0==pStaticEngine->Initialise())
		{
			TRACE_LOG("[ENGINE]PatsApiEngine Created OK when getPriceEngineInstance.");
		}
		else
		{
			TRACE_LOG("[ENGINE]PatsApiEngine Create FAIL! when getPriceEngineInstance.");
			delete pStaticEngine;
			pStaticEngine = NULL;
			return pStaticEngine;
		}
	}
	pStaticEngine->pPriceSource = priceSource;
	return (IMsg*)pStaticEngine;
}

IMsg* SimPatsApiEngine::getDealerEngineInstance(IMsg *dealer)
{
	if(pStaticEngine==NULL)
	{
		// create fixengine instance
		pStaticEngine = new SimPatsApiEngine();
		pStaticEngine->ReadIniConfig();
		if(0==pStaticEngine->Initialise())
		{
			TRACE_LOG("[ENGINE]PatsApiEngine Created when getDealerEngineInstance.");
		}
		else
		{
			TRACE_LOG("[ENGINE]PatsApiEngine Create FAIL! when getDealerEngineInstance.");
			delete pStaticEngine;
			pStaticEngine = NULL;
			return pStaticEngine;
		}
	}
	pStaticEngine->pDealer = dealer;
	return (IMsg*)pStaticEngine;
}

IMsg* SimPatsApiEngine::getEngineInstance()
{
	if(pStaticEngine==NULL)
	{
		TRACE_LOG("[ENGINE]Internal Error, PatsApiEngine is empty when getEngineInstance.");
	}
	return (IMsg*)pStaticEngine;
}

SimPatsApiEngine::SimPatsApiEngine(void)
{
	pDealer = NULL;
	pPriceSource = NULL;
	subPriceMap.clear();
	user2PatsMap.clear();
	pats2UserMap.clear();
	this->m_HostLink_Status = 0;
	this->m_PriceLink_Status = 0;
	m_nEngineStatus = ES_BORN;
	InitializeCriticalSection(&cs);
	StartThread();
}


// 初始化过程，准备dll，Initialise Dll，注册回调，连接服务器（通过ptReady）
int SimPatsApiEngine::Initialise(void)
{
	int nStatus=0;
	int ret = 0;

	// 1. 准备DLL
	if(this->SetupDll()==0)
	{
		TRACE_LOG("[ENGINE]Initialise, 1. Prepare DLL OK.");
	}
	else
	{
		TRACE_LOG("[ENGINE]Initialise, 1. Prepare DLL FAIL!");
		return 1;
	}

	// 2. 初始化DLL
	if(ptSuccess!=(nStatus=ptInitialise(ptClient,ptAPIversion,"ArbiTrade (API 2350)","0.9","FCB2350E003P",true)))
	//initialise the api with demo env
	//if(ptSuccess!=(nStatus=ptInitialise(ptTestClient,ptAPIversion,"APPID_2350","0.9","BETA",true)))
	//if(ptSuccess!=(nStatus=ptInitialise(ptDemoClient,ptAPIversion," "," "," ",true)))
	{
		TRACE_LOG("[ENGINE]Initialise, 2. Init DLL FAIL!");
		return 1;
	}
	else
	{
		TRACE_LOG("[ENGINE]Initialise, 2. Init DLL OK.");
	}

	// 3. 注册回调
	if(0==this->SetupCallbacks())
	{
		this->m_nEngineStatus = ES_INIT;
		TRACE_LOG("[ENGINE]Initialise, 3. Register Callbacks OK.");
	}
	else
	{
		TRACE_LOG("[ENGINE]Initialise, 3. Register Callbacks FAIL!");
		return 1;
	}
	// 4. ptReady启动API
	if(ptSuccess==(nStatus=this->SetupConnection()))
	{
		TRACE_LOG("[ENGINE]Initialise, 4. SetupConnection(ptReady) OK.");
		
	}
	else
	{
		TRACE_LOG("[ENGINE]Initialise, 4. SetupConnection(ptReady) FAIL! retcode=%d.",nStatus);
		return 1;
	}
	TRACE_LOG("[ENGINE]Initialise OK.");
	return ret;
}

// 准备API的过程，映射以下接口函数
// ptInitialise,ptSetHostAddress,ptSetPriceAddress,ptSetHostReconnect,ptSetPriceReconnect,ptReady,ptEnable,ptDisable,ptSubscribePrice,ptNotifyAllMessages,
// ptSetPriceAgeCounter,ptCountExchanges,ptGetExchange,ptGetExchangeByName,ptExchangeExists,ptGetExchangeRate,ptCountTraders,ptGetTrader,ptTraderExists,
// ptCountOrderTypes,ptGetOrderType,ptCountReportTypes,ptGetReportType,ptReportTypeExists,ptGetReportSize,ptGetReport,ptCountCommodities,ptGetCommodity,
// ptCommodityExists,ptGetCommodityByName,ptCountContracts,ptGetContract,ptGetExtendedContract,ptGetContractByName,ptGetExtendedContractByName,ptContractExists,
// ptLogOn,ptLogOff,ptGetLogonStatus,ptDOMEnabled,ptPostTradeAmendEnabled,ptCountUsrMsgs,ptGetUsrMsg,ptGetUsrMsgByID,ptAcknowledgeUsrMsg,
// ptRegisterCallback,ptRegisterLinkStateCallback,ptRegisterMsgCallback,ptRegisterPriceCallback,ptRegisterOrderCallback,
// ptRegisterFillCallback,ptRegisterContractCallback,ptRegisterStatusCallback,
// ptCountFills,ptGetFill,ptGetFillByID,ptGetContractPosition,ptGetOpenPosition,ptGetTotalPosition,ptGetAveragePrice,
// ptBlankPrices,ptGetPrice,ptGetPriceForContract,ptAddOrder,ptAmendOrder,ptCancelOrder,ptCancelBuys,ptCancelSells,ptCancelAll,
// ptQueryOrderStatus,ptOrderChecked,ptCountOrders,ptGetOrder,ptGetOrderByID,ptCountOrderHistory,ptGetOrderHistory,
// ptSetEncryptionCode,ptSetHandShakePeriod,ptSetInternetUser
int SimPatsApiEngine::SetupDll(void)
{
	//initialise the API
	int nStatus=0;
	int ret = 0;

	//load the DLL into memory
	//if(NULL==(hInstance=::LoadLibrary("demoapi.dll")))
	if(NULL==(hInstance=::LoadLibrary("patsapi.dll")))
	{
		TRACE_LOG("[ENGINE]SetupDll FAIL! Load patsapi.dll failed.");
		return 1;
	}

	//load the callable function by address
	// 只有这里读入（映射）的接口函数才能在engine中执行
	if(NULL==(ptInitialise=(lp_ptInitialise)::GetProcAddress(hInstance,"ptInitialise")))
	{
		TRACE_LOG("[ENGINE]SetupDll FAIL! Load ptInitialise failed.");
		return 1;
	}

	if(NULL==(ptSetHostAddress=(lp_ptSetHostAddress)::GetProcAddress(hInstance,"ptSetHostAddress")))
	{
		TRACE_LOG("[ENGINE]SetupDll FAIL! Load ptSetHostAddress failed.");
		return 1;
	}


	if(NULL==(ptSetPriceAddress=(lp_ptSetPriceAddress)::GetProcAddress(hInstance,"ptSetPriceAddress")))
	{
		TRACE_LOG("[ENGINE]SetupDll FAIL! Load ptSetPriceAddress failed.");
		return 1;
	}

	if(NULL==(ptSetHostReconnect=(lp_ptSetHostReconnect)::GetProcAddress(hInstance,"ptSetHostReconnect")))
	{
		TRACE_LOG("[ENGINE]SetupDll FAIL! Load ptSetHostReconnect failed.");
		return 1;
	}


	if(NULL==(ptSetPriceReconnect=(lp_ptSetPriceReconnect)::GetProcAddress(hInstance,"ptSetPriceReconnect")))
	{
		TRACE_LOG("[ENGINE]SetupDll FAIL! Load ptSetPriceReconnect failed.");
		return 1;
	}


	if(NULL==(ptReady=(lp_ptReady)::GetProcAddress(hInstance,"ptReady")))
	{
		TRACE_LOG("[ENGINE]SetupDll FAIL! Load ptReady failed.");
		return 1;
	}


	if(NULL==(ptEnable=(lp_ptEnable)::GetProcAddress(hInstance,"ptEnable")))
	{
		TRACE_LOG("[ENGINE]SetupDll FAIL! Load ptEnable failed.");
		return 1;
	}


	if(NULL==(ptDisable=(lp_ptDisable)::GetProcAddress(hInstance,"ptDisable")))
	{
		TRACE_LOG("[ENGINE]SetupDll FAIL! Load ptDisable failed.");
		return 1;
	}


	if(NULL==(ptSubscribePrice=(lp_ptSubscribePrice)::GetProcAddress(hInstance,"ptSubscribePrice")))
	{
		TRACE_LOG("[ENGINE]SetupDll FAIL! Load ptSubscribePrice failed.");
		return 1;
	}

	if(NULL==(ptUnsubscribePrice=(lp_ptUnsubscribePrice)::GetProcAddress(hInstance,"ptUnsubscribePrice")))
	{
		TRACE_LOG("[ENGINE]SetupDll FAIL! Load ptUnsubscribePrice failed.");
		return 1;
	}

	if(NULL==(ptNotifyAllMessages=(lp_ptNotifyAllMessages)::GetProcAddress(hInstance,"ptNotifyAllMessages")))
	{
		TRACE_LOG("[ENGINE]SetupDll FAIL! Load ptNotifyAllMessages failed.");
		return 1;
	}


	if(NULL==(ptSetPriceAgeCounter=(lp_ptSetPriceAgeCounter)::GetProcAddress(hInstance,"ptSetPriceAgeCounter")))
	{
		TRACE_LOG("[ENGINE]SetupDll FAIL! Load ptSetPriceAgeCounter failed.");
		return 1;
	}


	if(NULL==(ptCountExchanges=(lp_ptCountExchanges)::GetProcAddress(hInstance,"ptCountExchanges")))
	{
		TRACE_LOG("[ENGINE]SetupDll FAIL! Load ptCountExchanges failed.");
		return 1;
	}


	if(NULL==(ptGetExchange=(lp_ptGetExchange)::GetProcAddress(hInstance,"ptGetExchange")))
	{
		TRACE_LOG("[ENGINE]SetupDll FAIL! Load ptGetExchange failed.");
		return 1;
	}


	if(NULL==(ptGetExchangeByName=(lp_ptGetExchangeByName)::GetProcAddress(hInstance,"ptGetExchangeByName")))
	{
		TRACE_LOG("[ENGINE]SetupDll FAIL! Load ptGetExchangeByName failed.");
		return 1;
	}

	if(NULL==(ptExchangeExists=(lp_ptExchangeExists)::GetProcAddress(hInstance,"ptExchangeExists")))
	{
		TRACE_LOG("[ENGINE]SetupDll FAIL! Load ptExchangeExists failed.");
		return 1;
	}


	if(NULL==(ptGetExchangeRate=(lp_ptGetExchangeRate)::GetProcAddress(hInstance,"ptGetExchangeRate")))
	{
		TRACE_LOG("[ENGINE]SetupDll FAIL! Load ptGetExchangeRate failed.");
		return 1;
	}


	if(NULL==(ptCountTraders=(lp_ptCountTraders)::GetProcAddress(hInstance,"ptCountTraders")))
	{
		//you've got the idea now about reporting errros
		return 1;
	}


	if(NULL==(ptGetTrader=(lp_ptGetTrader)::GetProcAddress(hInstance,"ptGetTrader")))
	{
		return 1;
	}


	if(NULL==(ptTraderExists=(lp_ptTraderExists)::GetProcAddress(hInstance,"ptTraderExists")))
	{
		return 1;
	}


	if(NULL==(ptCountOrderTypes=(lp_ptCountOrderTypes)::GetProcAddress(hInstance,"ptCountOrderTypes")))
	{
		return 1;
	}


	if(NULL==(ptGetOrderType=(lp_ptGetOrderType)::GetProcAddress(hInstance,"ptGetOrderType")))
	{
		return 1;
	}

	if(NULL==(ptCountReportTypes=(lp_ptCountReportTypes)::GetProcAddress(hInstance,"ptCountReportTypes")))
	{
		return 1;
	}


	if(NULL==(ptGetReportType=(lp_ptGetReportType)::GetProcAddress(hInstance,"ptGetReportType")))
	{
		return 1;
	}


	if(NULL==(ptReportTypeExists=(lp_ptReportTypeExists)::GetProcAddress(hInstance,"ptReportTypeExists")))
	{
		return 1;
	}


	if(NULL==(ptGetReportSize=(lp_ptGetReportSize)::GetProcAddress(hInstance,"ptGetReportSize")))
	{
		return 1;
	}


	if(NULL==(ptGetReport=(lp_ptGetReport)::GetProcAddress(hInstance,"ptGetReport")))
	{
		return 1;
	}


	if(NULL==(ptCountCommodities=(lp_ptCountCommodities)::GetProcAddress(hInstance,"ptCountCommodities")))
	{
		return 1;
	}


	if(NULL==(ptGetCommodity=(lp_ptGetCommodity)::GetProcAddress(hInstance,"ptGetCommodity")))
	{
		return 1;
	}


	if(NULL==(ptCommodityExists=(lp_ptCommodityExists)::GetProcAddress(hInstance,"ptCommodityExists")))
	{
		return 1;
	}


	if(NULL==(ptGetCommodityByName=(lp_ptGetCommodityByName)::GetProcAddress(hInstance,"ptGetCommodityByName")))
	{
		return 1;
	}


	if(NULL==(ptCountContracts=(lp_ptCountContracts)::GetProcAddress(hInstance,"ptCountContracts")))
	{
		return 1;
	}


	if(NULL==(ptGetContract=(lp_ptGetContract)::GetProcAddress(hInstance,"ptGetContract")))
	{
		return 1;
	}


	if(NULL==(ptGetExtendedContract=(lp_ptGetExtendedContract)::GetProcAddress(hInstance,"ptGetExtendedContract")))
	{
		return 1;
	}

	if(NULL==(ptGetContractByName=(lp_ptGetContractByName)::GetProcAddress(hInstance,"ptGetContractByName")))
	{
		return 1;
	}

	if(NULL==(ptGetExtendedContractByName=(lp_ptGetExtendedContractByName)::GetProcAddress(hInstance,"ptGetExtendedContractByName")))
	{
		return 1;
	}

	if(NULL==(ptContractExists=(lp_ptContractExists)::GetProcAddress(hInstance,"ptContractExists")))
	{
		return 1;
	}

	if(NULL==(ptLogon=(lp_ptLogOn)::GetProcAddress(hInstance,"ptLogOn")))
	{
		return 1;
	}


	if(NULL==(ptLogoff=(lp_ptLogOff)::GetProcAddress(hInstance,"ptLogOff")))
	{
		return 1;
	}

	if(NULL==(ptDisconnect=(lp_ptDisconnect)::GetProcAddress(hInstance,"ptDisconnect")))
	{
		return 1;
	}

	if(NULL==(ptGetLogonStatus=(lp_ptGetLogonStatus)::GetProcAddress(hInstance,"ptGetLogonStatus")))
	{
		return 1;
	}


	if(NULL==(ptDOMEnabled=(lp_ptDOMEnabled)::GetProcAddress(hInstance,"ptDOMEnabled")))
	{
		return 1;
	}


	if(NULL==(ptPostTradeAmendEnabled=(lp_ptPostTradeAmendEnabled)::GetProcAddress(hInstance,"ptPostTradeAmendEnabled")))
	{
		return 1;
	}


	if(NULL==(ptCountUsrMsgs=(lp_ptCountUsrMsgs)::GetProcAddress(hInstance,"ptCountUsrMsgs")))
	{
		return 1;
	}

	
	if(NULL==(ptGetUsrMsg=(lp_ptGetUsrMsg)::GetProcAddress(hInstance,"ptGetUsrMsg")))
	{
		return 1;
	}


	if(NULL==(ptGetUsrMsgByID=(lp_ptGetUsrMsgByID)::GetProcAddress(hInstance,"ptGetUsrMsgByID")))
	{
		return 1;
	}


	if(NULL==(ptAcknowledgeUsrMsg=(lp_ptAcknowledgeUsrMsg)::GetProcAddress(hInstance,"ptAcknowledgeUsrMsg")))
	{
		return 1;
	}

	
	if(NULL==(ptRegisterCallback=(lp_ptRegisterCallback)::GetProcAddress(hInstance,"ptRegisterCallback")))
	{
		return 1;
	}


	if(NULL==(ptRegisterLinkStateCallback=(lp_ptRegisterLinkStateCallback)::GetProcAddress(hInstance,"ptRegisterLinkStateCallback")))
	{
		return 1;
	}


	if(NULL==(ptRegisterMsgCallback=(lp_ptRegisterMsgCallback)::GetProcAddress(hInstance,"ptRegisterMsgCallback")))
	{
		return 1;
	}


	if(NULL==(ptRegisterPriceCallback=(lp_ptRegisterPriceCallback)::GetProcAddress(hInstance,"ptRegisterPriceCallback")))
	{
		return 1;
	}


	if(NULL==(ptRegisterOrderCallback=(lp_ptRegisterOrderCallback)::GetProcAddress(hInstance,"ptRegisterOrderCallback")))
	{
		return 1;
	}


	if(NULL==(ptRegisterFillCallback=(lp_ptRegisterFillCallback)::GetProcAddress(hInstance,"ptRegisterFillCallback")))
	{
		return 1;
	}

	if(NULL==(ptRegisterContractCallback=(lp_ptRegisterContractCallback)::GetProcAddress(hInstance,"ptRegisterContractCallback")))
	{
		return 1;
	}

	if(NULL==(ptRegisterStatusCallback=(lp_ptRegisterStatusCallback)::GetProcAddress(hInstance,"ptRegisterStatusCallback")))
	{
		return 1;
	}

	if(NULL==(ptCountFills=(lp_ptCountFills)::GetProcAddress(hInstance,"ptCountFills")))
	{
		return 1;
	}

	if(NULL==(ptGetFill=(lp_ptGetFill)::GetProcAddress(hInstance,"ptGetFill")))
	{
		return 1;
	}

	if(NULL==(ptGetFillByID=(lp_ptGetFillByID)::GetProcAddress(hInstance,"ptGetFillByID")))
	{
		return 1;
	}

	if(NULL==(ptGetContractPosition=(lp_ptGetContractPosition)::GetProcAddress(hInstance,"ptGetContractPosition")))
	{
		return 1;
	}

	if(NULL==(ptGetOpenPosition=(lp_ptGetOpenPosition)::GetProcAddress(hInstance,"ptGetOpenPosition")))
	{
		return 1;
	}

	if(NULL==(ptGetTotalPosition=(lp_ptGetTotalPosition)::GetProcAddress(hInstance,"ptGetTotalPosition")))
	{
		return 1;
	}

	if(NULL==(ptGetAveragePrice=(lp_ptGetAveragePrice)::GetProcAddress(hInstance,"ptGetAveragePrice")))
	{
		return 1;
	}

	if(NULL==(ptBlankPrices=(lp_ptBlankPrices)::GetProcAddress(hInstance,"ptBlankPrices")))
	{
		return 1;
	}

	if(NULL==(ptGetPrice=(lp_ptGetPrice)::GetProcAddress(hInstance,"ptGetPrice")))
	{
		return 1;
	}

	if(NULL==(ptGetPriceForContract=(lp_ptGetPriceForContract)::GetProcAddress(hInstance,"ptGetPriceForContract")))
	{
		return 1;
	}

	if(NULL==(ptAddOrder=(lp_ptAddOrder)::GetProcAddress(hInstance,"ptAddOrder")))
	{
		return 1;
	}

	if(NULL==(ptAmendOrder=(lp_ptAmendOrder)::GetProcAddress(hInstance,"ptAmendOrder")))
	{
		return 1;
	}

	if(NULL==(ptCancelOrder=(lp_ptCancelOrder)::GetProcAddress(hInstance,"ptCancelOrder")))
	{
		return 1;
	}

	if(NULL==(ptCancelBuys=(lp_ptCancelBuys)::GetProcAddress(hInstance,"ptCancelBuys")))
	{
		return 1;
	}

	if(NULL==(ptCancelSells=(lp_ptCancelSells)::GetProcAddress(hInstance,"ptCancelSells")))
	{
		return 1;
	}

	if(NULL==(ptCancelAll=(lp_ptCancelAll)::GetProcAddress(hInstance,"ptCancelAll")))
	{
		return 1;
	}

	if(NULL==(ptQueryOrderStatus=(lp_ptQueryOrderStatus)::GetProcAddress(hInstance,"ptQueryOrderStatus")))
	{
		return 1;
	}

	if(NULL==(ptOrderChecked=(lp_ptOrderChecked)::GetProcAddress(hInstance,"ptOrderChecked")))
	{
		return 1;
	}

	if(NULL==(ptCountOrders=(lp_ptCountOrders)::GetProcAddress(hInstance,"ptCountOrders")))
	{
		return 1;
	}

	if(NULL==(ptGetOrder=(lp_ptGetOrder)::GetProcAddress(hInstance,"ptGetOrder")))
	{
		return 1;
	}

	if(NULL==(ptGetOrderByID=(lp_ptGetOrderByID)::GetProcAddress(hInstance,"ptGetOrderByID")))
	{
		return 1;
	}

	if(NULL==(ptCountOrderHistory=(lp_ptCountOrderHistory)::GetProcAddress(hInstance,"ptCountOrderHistory")))
	{
		return 1;
	}

	if(NULL==(ptGetOrderHistory=(lp_ptGetOrderHistory)::GetProcAddress(hInstance,"ptGetOrderHistory")))
	{
		return 1;
	}

	if(NULL==(ptSetEncryptionCode=(lp_ptSetEncryptionCode)::GetProcAddress(hInstance,"ptSetEncryptionCode")))
	{
		return 1;
	}

	if(NULL==(ptSetHandShakePeriod=(lp_ptSetHandShakePeriod)::GetProcAddress(hInstance,"ptSetHandShakePeriod")))
	{
		return 1;
	}

	if(NULL==(ptSetInternetUser=(lp_ptSetInternetUser)::GetProcAddress(hInstance,"ptSetInternetUser")))
	{
		return 1;
	}
	return ret;
}

// 所有的回调Callback函数都必须预先注册，这里注册了以下Callback
// ptDataDLComplete,ptLogonStatus,ptForcedLogout,HostLinkStateChange,ptPriceLinkStateChange,
// ptContractAdded,ptContractDeleted,ptStatusChange
// ptMessage,
// ptOrder,ptFill,ptPriceUpdate,
int SimPatsApiEngine::SetupCallbacks(void)
{
	int nStatus = 0;
	const int func_error = 1;
	//register callback functions
	if(ptSuccess!=(nStatus=ptRegisterCallback(ptDataDLComplete,&DataDLComplete)))
	{ // needed
		TRACE_LOG("[ENGINE]SetupCallbacks FAIL! Failed to register callback DataDLComplete");
		return func_error;
	}

	if(ptSuccess!=(nStatus=ptRegisterCallback(ptLogonStatus,&LogonStatus)))
	{ // needed
		TRACE_LOG("[ENGINE]SetupCallbacks FAIL! Failed to register callback LogonStatus");
		return func_error;
	}

	if(ptSuccess!=(nStatus=ptRegisterCallback(ptForcedLogout,&ForcedLogout)))
	{
		TRACE_LOG("[ENGINE]SetupCallbacks FAIL! Failed to register callback forecedlogout");
		return func_error;
	}

	if(ptSuccess!=(nStatus=ptRegisterContractCallback(ptContractAdded,&ContractAdded)))
	{
		TRACE_LOG("[ENGINE]SetupCallbacks FAIL! Failed to register callback ContractAdded");
		return func_error;
	}

	if(ptSuccess!=(nStatus=ptRegisterContractCallback(ptContractDeleted,&ContractDeleted)))
	{
		TRACE_LOG("[ENGINE]SetupCallbacks FAIL! Failed to register callback ContractDeleted");
		return func_error;
	}

	if(ptSuccess!=(nStatus=ptRegisterLinkStateCallback(ptHostLinkStateChange,&HostLinkStateChange)))
	{ // needed
		TRACE_LOG("[ENGINE]SetupCallbacks FAIL! Failed to register callback HostLinkStateChange");
		return func_error;
	}

	if(ptSuccess!=(nStatus=ptRegisterLinkStateCallback(ptPriceLinkStateChange,&PriceLinkStateChange)))
	{ 
		TRACE_LOG("[ENGINE]SetupCallbacks FAIL! Failed to register callback PriceLinkStateChange");
		return func_error;
	}

	if(ptSuccess!=(nStatus=ptRegisterMsgCallback(ptMessage,&Message)))
	{
		TRACE_LOG("[ENGINE]SetupCallbacks FAIL! Failed to register callback Message");
		return func_error;
	}

	if(ptSuccess!=(nStatus=ptRegisterOrderCallback(ptOrder,&OrderCallback)))
	{ // needed
		TRACE_LOG("[ENGINE]SetupCallbacks FAIL! Failed to register callback Order");
		return func_error;
	}

	if(ptSuccess!=(nStatus=ptRegisterFillCallback(ptFill,&Fill)))
	{ // needed
		TRACE_LOG("[ENGINE]SetupCallbacks FAIL! Failed to register callback Fill");
		return func_error;
	}

	if(ptSuccess!=(nStatus=ptRegisterPriceCallback(ptPriceUpdate,&PriceUpdate)))
	{ // needed
		TRACE_LOG("[ENGINE]SetupCallbacks FAIL! Failed to register callback Order");
		return func_error;
	}

	if(ptSuccess!=(nStatus=ptRegisterStatusCallback(ptStatusChange,&StatusChange)))
	{
		TRACE_LOG("[ENGINE]SetupCallbacks FAIL! Failed to register callback Status");
		return func_error;
	}
	return 0;
}

// 设置连接参数和调试参数后，发起连接。这将导致连接成功后自动的登录动作。
// 在api初次初始化以及暂时退登后，都需要通过该函数重新准备api。
int SimPatsApiEngine::SetupConnection(void)
{
	int nStatus = 0;
	char szHostPort[5];
	sprintf(szHostPort,"%d",this->m_nHostServerPort);
	char szPricePort[5];
	sprintf(szPricePort,"%d",this->m_nPriceServerPort);
	if(ptSuccess!=(nStatus=ptSetPriceAddress(this->m_szPriceServerIP,szPricePort)))
	{
		TRACE_LOG("[ENGINE]SetupConnection FAIL! Failed setting price address");
		return nStatus;
	}
	if(ptSuccess!=(nStatus=ptSetHostAddress(this->m_szHostServerIP,szHostPort)))
	{
		TRACE_LOG("[ENGINE]SetupConnection FAIL! Failed setting host address");
		return nStatus;
	}

	if(ptSuccess!=(nStatus=ptSetHandShakePeriod(5)))
	{
		TRACE_LOG("[ENGINE]SetupConnection FAIL! Failed setting host handshake");
		return nStatus;
	}

	if(ptSuccess!=(nStatus=ptSetHostReconnect(15)))
	{
		TRACE_LOG("[ENGINE]SetupConnection FAIL! Failed setting host reconnect");
		return nStatus;
	}
		
	if(ptSuccess!=(nStatus=ptSetPriceReconnect(15)))
	{
		TRACE_LOG("[ENGINE]SetupConnection FAIL! Failed setting price reconnect");
		return nStatus;
	}

	if(ptSuccess!=(nStatus=ptSetInternetUser(0)))
	{
		TRACE_LOG("[ENGINE]SetupConnection FAIL! Failed setting internet user");
		return nStatus;
	}
	
	// bit 0 show program flow and messages
	// bit 1 show traffic to/from host
	// bit 2 show traffic to/from price server
	// bit 3 show depth-of-market flow
	// bit 4 show order processing
	// bit 5 write procedure call log on normal exit
	// bit 6 show calls to API
	// bit 7 show detailed log of IP socket and locking
	ptEnable(19); // 推荐的API日志记录级别是19（014）和83（0146），兼容性测试用219（013467）
	TRACE_LOG("[ENGINE]SetupConnection Set ptEnable to 014.");
	//now we have set up the params we need for processing depending
	//on whether it a demo api or a live api (onto a test system) fire up the API for real
	if(ptSuccess!=(nStatus=ptReady()))
	{
		TRACE_LOG("[ENGINE]SetupConnection FAIL! ptReady return failed, code=%d.",nStatus);
	}
	return nStatus;
}

SimPatsApiEngine::~SimPatsApiEngine(void)
{
	DeleteCriticalSection(&cs);
}

void SimPatsApiEngine::ReadIniConfig()
{ // 登录需要的参数
	GetPrivateProfileStringA("PATSAPIENGINE", "USERNAME", "patsusername", m_szUsername, 64, ".\\AIB.ini");
	GetPrivateProfileStringA("PATSAPIENGINE", "PASSWORD", "patspassword", m_szPassword, 64, ".\\AIB.ini");
	GetPrivateProfileStringA("PATSAPIENGINE", "HOSTSERVERIP", "192.168.0.1", m_szHostServerIP, 16, ".\\AIB.ini");
	GetPrivateProfileStringA("PATSAPIENGINE", "PRICESERVERIP", "192.168.0.2", m_szPriceServerIP, 16, ".\\AIB.ini");
	m_nHostServerPort = GetPrivateProfileIntA("PATSAPIENGINE","HOSTSERVERPORT",111,".\\AIB.ini");
	m_nPriceServerPort = GetPrivateProfileIntA("PATSAPIENGINE","PRICESERVERPORT",112,".\\AIB.ini");
	TRACE_LOG("[ENGINE]ReadIniConfig uid:%s,pwd:%s,host[%s:%d],price[%s:%d].",m_szUsername,m_szPassword,m_szHostServerIP,m_nHostServerPort,m_szPriceServerIP,m_nPriceServerPort);
}

BOOL SimPatsApiEngine::Prepare(long *pnResult)
{
	TRACE_LOG("[ENGINE][THREAD]Prepare...");
	return TRUE;
}

BOOL SimPatsApiEngine::Cleanup(long *pnResult)
{
	TRACE_LOG("[ENGINE][THREAD]Cleanup...");
	return TRUE;
}

// 收到api的HostLink通知后，在线程中启动登录动作
void SimPatsApiEngine::Login(void)
{
	int nStatus=-1;
	LogonStruct logonDetails;

	strcpy(logonDetails.UserID,this->m_szUsername);
	strcpy(logonDetails.Password,this->m_szPassword);
	strcpy(logonDetails.NewPassword,"\0");
	logonDetails.Reset='Y';
	TRACE_LOG("[ENGINE][THREAD]prepare login using UserID %s, Password %s.",this->m_szUsername,this->m_szPassword);

	if(ptSuccess!=(nStatus=ptLogon(&logonDetails)))
	{
		TRACE_LOG("[ENGINE][THREAD]Call api Login FAIL! nStatus=%d.",nStatus);	
	}
	else
	{
		TRACE_LOG("[ENGINE][THREAD]Call api Login OK.");
	}
}

// 收到上层的退登指示后，在线程中启动退登动作
void SimPatsApiEngine::LogOut(TPatsApiLogoutMsg* pMsg)
{
	int nStatus;
	TPatsApiLogoutRespMsg sender;
	sender.type = MSGTYPE_PATSAPI_LOGOUT_RESP;
	sender.len = sizeof(TPatsApiLogoutRespMsg)-sizeof(TRawMsg);
	
			
	if(pMsg->logoutType==0)
	{ // partial logout
		if(ptSuccess!=(nStatus=ptDisconnect()))
		{
			TRACE_LOG("[ENGINE][THREAD]Call api ptDisconnect FAIL! nStatus=%d.",nStatus);	
			sender.code = 100;
			sprintf(sender.reason,"Error from dll, code:%d.",nStatus);
		}
		else
		{
			TRACE_LOG("[ENGINE][THREAD]Call api ptDisconnect OK.");
			sender.code = 0;
			sprintf(sender.reason,"success");
			//this->m_bLogon = false;
			this->m_nEngineStatus = ES_LOGOUT;
		}
	}
	else
	{ // entire logout
		if(ptSuccess!=(nStatus=ptLogoff()))
		{
			TRACE_LOG("[ENGINE][THREAD]Call api ptLogoff FAIL! nStatus=%d.",nStatus);	
			sender.code = 100;
			sprintf(sender.reason,"Error from dll, code:%d.",nStatus);
		}
		else
		{
			TRACE_LOG("[ENGINE][THREAD]Call api ptLogoff OK.");
			sender.code = 1;
			sprintf(sender.reason,"success");
			//this->m_bLogon = false;
			this->m_nEngineStatus = ES_DEAD;
		}
	}
	if(pPriceSource) pPriceSource->SendMsg(&sender);
	if(pDealer) pDealer->SendMsg(&sender);

	if(sender.code==0) NotifyEngineStatus();
}

void SimPatsApiEngine::NotifyEngineStatus()
{
	TRACE_LOG("[ENGINE][THREAD]NotifyEngineStatus %s.",szEngineStatusDesc[m_nEngineStatus]);
	TPatsApiEngineStatusMsg sender;
	sender.type = MSGTYPE_PATSAPI_ENGINE_STATUS;
	sender.len = sizeof(TPatsApiEngineStatusMsg)-sizeof(TRawMsg);
	sender.status = this->m_nEngineStatus;
	if(pPriceSource) pPriceSource->SendMsg(&sender);
	if(pDealer) pDealer->SendMsg(&sender);
}

void SimPatsApiEngine::PrintPriceDetail(const char * tag, PriceDetailStruct& priceDetail)
{
	char* priceValue = priceDetail.Price;
	if(strcmp(tag,"Total")==0) priceValue = "N/A";
	char* priceDirection = "still";
	if(priceDetail.Direction==ptPriceRise) priceDirection = "rising";
	if(priceDetail.Direction==ptPriceFall) priceDirection = "falling";
	TRACE_LOG("PriceDetail for %s, (%s,%d), is %s, age:%d, %02d:%02d:%02d.",tag,priceValue,priceDetail.Volume,priceDirection,priceDetail.AgeCounter,
		priceDetail.Hour,priceDetail.Minute,priceDetail.Second);
}

void SimPatsApiEngine::GetPrice(TPatsApiCPReqMsg* pCPReqMsg)
{
	int nStatus = -1;
	TRACE_LOG("[ENGINE][THREAD]RECV GetCurrentPrice for Exchange=%s Commodity=%s Contract=%s",pCPReqMsg->exchangeName,pCPReqMsg->contractName,pCPReqMsg->MaturityMonthYear);
	PriceStruct newPrice;
	//call getPrice on this contract
	if(ptSuccess!=(nStatus=ptGetPriceForContract((ExchNamePtr)(pCPReqMsg->exchangeName),
		(ConNamePtr)(pCPReqMsg->contractName),(ConDatePtr)(pCPReqMsg->MaturityMonthYear),&newPrice)))
		//(ExchNamePtr)(pMsg->exchangeName),(ConNamePtr)(pMsg->contractName),(ConDatePtr)(pMsg->MaturityMonthYear),&newPrice)))
	{
		TRACE_LOG("[ENGINE][THREAD]GetPrice failed %d",nStatus);
	}
	else
	{ // send price to upper
		TRACE_LOG("PriceInfo Status %d, Mask %d, PriceStatus %d, as follow...:", newPrice.Status, newPrice.ChangeMask, newPrice.PriceStatus);
		this->PrintPriceDetail("Bid",newPrice.Bid);
		this->PrintPriceDetail("Offer",newPrice.Offer);
		this->PrintPriceDetail("ImpliedBid",newPrice.ImpliedBid);
		this->PrintPriceDetail("ImpliedOffer",newPrice.ImpliedOffer);
		this->PrintPriceDetail("RFQ",newPrice.RFQ);
		this->PrintPriceDetail("Last0",newPrice.Last0);
		this->PrintPriceDetail("Last1",newPrice.Last1);
		this->PrintPriceDetail("Last2",newPrice.Last2);
		this->PrintPriceDetail("Last3",newPrice.Last3);
		this->PrintPriceDetail("Last4",newPrice.Last4);
		this->PrintPriceDetail("Total",newPrice.Total);
		this->PrintPriceDetail("High",newPrice.High);
		this->PrintPriceDetail("Low",newPrice.Low);
		this->PrintPriceDetail("Opening",newPrice.Opening);
		this->PrintPriceDetail("Closing",newPrice.Closing);
		this->PrintPriceDetail("BidDOM0",newPrice.BidDOM0);
		this->PrintPriceDetail("BidDOM1",newPrice.BidDOM1);
		this->PrintPriceDetail("BidDOM2",newPrice.BidDOM2);
		this->PrintPriceDetail("BidDOM3",newPrice.BidDOM3);
		this->PrintPriceDetail("BidDOM4",newPrice.BidDOM4);
		this->PrintPriceDetail("OfferDOM0",newPrice.OfferDOM0);
		this->PrintPriceDetail("OfferDOM1",newPrice.OfferDOM1);
		this->PrintPriceDetail("OfferDOM2",newPrice.OfferDOM2);
		this->PrintPriceDetail("OfferDOM3",newPrice.OfferDOM3);
		this->PrintPriceDetail("OfferDOM4",newPrice.OfferDOM4);
		this->PrintPriceDetail("LimitUp",newPrice.LimitUp);
		this->PrintPriceDetail("LimitDown",newPrice.LimitDown);
		this->PrintPriceDetail("pvIndBid",newPrice.pvIndBid);
		this->PrintPriceDetail("pvIndOffer",newPrice.pvIndOffer);
		this->PrintPriceDetail("ReferencePrice",newPrice.ReferencePrice);
		TRACE_LOG("PriceInfo end......................................");

		//if( (newPrice.ChangeMask&ptChangeBid) || (newPrice.ChangeMask&ptChangeOffer) )// || (newPrice.ChangeMask&ptChangeLast) )
		//{	
		TPatsApiMDSRespMsg sender;
		sender.type = MSGTYPE_PATSAPI_MD_SRESP;
		sender.len = sizeof(TPatsApiMDSRespMsg)-sizeof(TRawMsg);
		sender.bidprice = atof(newPrice.Bid.Price);
		sender.askprice = atof(newPrice.Offer.Price);
		sender.tradeprice = atof(newPrice.Last0.Price);
		strncpy(sender.exchangeName,pCPReqMsg->exchangeName,11);
		strncpy(sender.contractName,pCPReqMsg->contractName,11);
		strncpy(sender.MaturityMonthYear,pCPReqMsg->MaturityMonthYear,51);
		TRACE_LOG("[ENGINE][THREAD]GetPrice Report: Bid %s [%d] Offer %s [%d] Last %s [%d]",newPrice.Bid.Price,newPrice.Bid.Volume,newPrice.Offer.Price,newPrice.Offer.Volume,newPrice.Last0.Price,newPrice.Last0.Volume);
		if(pPriceSource) pPriceSource->SendMsg(&sender);
		//}
	}
}

void SimPatsApiEngine::SubPrice2PriceServer()
{
	int nStatus = -1;

	map<string,int>::iterator it = subPriceMap.begin();
	while (it != subPriceMap.end())
	{
		string key = it->first;
		TRACE_LOG("In map, subprice Key=%s.",key.c_str());
		//LME#CA#3M
		string ename = key.substr(0,key.find_first_of('#'));
		string cdate = key.substr(key.find_last_of('#')+1);
		string cname = key.substr(key.find_first_of('#')+1,key.find_last_of('#')-key.find_first_of('#')-1);

		// unsub it
		if(ptSuccess!=(nStatus=ptUnsubscribePrice((ExchNamePtr)(ename.c_str()),
			(ConNamePtr)(cname.c_str()),(ConDatePtr)(cdate.c_str()))))
		{
			TRACE_LOG("[ENGINE][THREAD]SubPrice call ptUnsubscribePrice FAIL! code=%d Exchange=%s Commodity=%s Contract=%s",nStatus,ename.c_str(),cname.c_str(),cdate.c_str());
			/*
			TPatsApiMDRejectMsg sender;
			memset(&sender,0,sizeof(TPatsApiMDRejectMsg));
			sender.type = MSGTYPE_PATSAPI_MD_REJECT;
			sender.len = sizeof(TPatsApiMDRejectMsg)-sizeof(TRawMsg);
			sender.FailCode = nStatus;
			strncpy(sender.exchangeName,pMDReqMsg->exchangeName,10);
			strncpy(sender.contractName,pMDReqMsg->contractName,10);
			strncpy(sender.MaturityMonthYear,pMDReqMsg->MaturityMonthYear,50);
			if(pPriceSource) pPriceSource->SendMsg(&sender);
			*/
		}
		else
		{ 
			TRACE_LOG("[ENGINE][THREAD]SubPrice call ptUnsubscribePrice OK. Exchange=%s Commodity=%s Contract=%s",ename.c_str(),cname.c_str(),cdate.c_str());
		}
		// sub it.
		if(ptSuccess!=(nStatus=ptSubscribePrice((ExchNamePtr)(ename.c_str()),
			(ConNamePtr)(cname.c_str()),(ConDatePtr)(cdate.c_str()))))
		{
			TRACE_LOG("[ENGINE][THREAD]SubPrice call ptSubscribePrice FAIL! code=%d Exchange=%s Commodity=%s Contract=%s",nStatus,ename.c_str(),cname.c_str(),cdate.c_str());
			/*
			TPatsApiMDRejectMsg sender;
			memset(&sender,0,sizeof(TPatsApiMDRejectMsg));
			sender.type = MSGTYPE_PATSAPI_MD_REJECT;
			sender.len = sizeof(TPatsApiMDRejectMsg)-sizeof(TRawMsg);
			sender.FailCode = nStatus;
			strncpy(sender.exchangeName,pMDReqMsg->exchangeName,10);
			strncpy(sender.contractName,pMDReqMsg->contractName,10);
			strncpy(sender.MaturityMonthYear,pMDReqMsg->MaturityMonthYear,50);
			if(pPriceSource) pPriceSource->SendMsg(&sender);
			*/
		}
		else
		{ 
			TRACE_LOG("[ENGINE][THREAD]SubPrice call ptSubscribePrice OK. Exchange=%s Commodity=%s Contract=%s",ename.c_str(),cname.c_str(),cdate.c_str());
		}

		it ++;
	}
}

void SimPatsApiEngine::SubPrice(TPatsApiMDReqMsg* pMDReqMsg)
{
	int nStatus = -1;
	string part1 = pMDReqMsg->exchangeName;
	string part2 = pMDReqMsg->contractName;
	string part3 = pMDReqMsg->MaturityMonthYear;
	string key = part1 + "#" + part2 + "#" + part3;
	TRACE_LOG("subprice Key=%s.",key.c_str());
	if(pMDReqMsg->requestType==0)
	{ // subPrice
		this->subPriceMap[key] = 1;
		TRACE_LOG("[ENGINE][THREAD]SubPrice add to map OK. Exchange=%s Commodity=%s Contract=%s",pMDReqMsg->exchangeName,pMDReqMsg->contractName,pMDReqMsg->MaturityMonthYear);
		if(this->m_PriceLink_Status == ptLinkConnected)
		{
			if(ptSuccess!=(nStatus=ptSubscribePrice((ExchNamePtr)(pMDReqMsg->exchangeName),
			(ConNamePtr)(pMDReqMsg->contractName),(ConDatePtr)(pMDReqMsg->MaturityMonthYear))))
			{
				TRACE_LOG("[ENGINE][THREAD]SubPrice call ptSubscribePrice FAIL! code=%d Exchange=%s Commodity=%s Contract=%s",nStatus,pMDReqMsg->exchangeName,pMDReqMsg->contractName,pMDReqMsg->MaturityMonthYear);
				TPatsApiMDRejectMsg sender;
				memset(&sender,0,sizeof(TPatsApiMDRejectMsg));
				sender.type = MSGTYPE_PATSAPI_MD_REJECT;
				sender.len = sizeof(TPatsApiMDRejectMsg)-sizeof(TRawMsg);
				sender.FailCode = nStatus;
				strncpy(sender.exchangeName,pMDReqMsg->exchangeName,10);
				strncpy(sender.contractName,pMDReqMsg->contractName,10);
				strncpy(sender.MaturityMonthYear,pMDReqMsg->MaturityMonthYear,50);
				if(pPriceSource) pPriceSource->SendMsg(&sender);
			}
			else
			{ 
				TRACE_LOG("[ENGINE][THREAD]SubPrice call ptSubscribePrice OK. Exchange=%s Commodity=%s Contract=%s",pMDReqMsg->exchangeName,pMDReqMsg->contractName,pMDReqMsg->MaturityMonthYear);
			}
		}
	}
	else
	{ // unsubPrice
		this->subPriceMap.erase(key);
		if(ptSuccess!=(nStatus=ptUnsubscribePrice((ExchNamePtr)(pMDReqMsg->exchangeName),
		(ConNamePtr)(pMDReqMsg->contractName),(ConDatePtr)(pMDReqMsg->MaturityMonthYear))))
		{
			TRACE_LOG("[ENGINE][THREAD]SubPrice call ptUnsubscribePrice FAIL! code=%d Exchange=%s Commodity=%s Contract=%s",nStatus,pMDReqMsg->exchangeName,pMDReqMsg->contractName,pMDReqMsg->MaturityMonthYear);
			TPatsApiMDRejectMsg sender;
			memset(&sender,0,sizeof(TPatsApiMDRejectMsg));
			sender.type = MSGTYPE_PATSAPI_MD_REJECT;
			sender.len = sizeof(TPatsApiMDRejectMsg)-sizeof(TRawMsg);
			sender.FailCode = nStatus;
			strncpy(sender.exchangeName,pMDReqMsg->exchangeName,10);
			strncpy(sender.contractName,pMDReqMsg->contractName,10);
			strncpy(sender.MaturityMonthYear,pMDReqMsg->MaturityMonthYear,50);
			if(pPriceSource) pPriceSource->SendMsg(&sender);
		}
		else
		{ 
			TRACE_LOG("[ENGINE][THREAD]SubPrice call ptUnsubscribePrice OK. Exchange=%s Commodity=%s Contract=%s",pMDReqMsg->exchangeName,pMDReqMsg->contractName,pMDReqMsg->MaturityMonthYear);
		}
	}
}

// 下单立即失败时，汇报给上层，包括重复报单，没有交易帐号，下单api失败等
void SimPatsApiEngine::AddOrder_Reject(TPatsApiAddOrderMsg* pAOMsg)
{
	TPatsApiOrderRespMsg sender;
	memset(&sender,0,sizeof(TPatsApiOrderRespMsg));
	sender.type = MSGTYPE_PATSAPI_OR;
	sender.len = sizeof(TPatsApiOrderRespMsg)-sizeof(TRawMsg);
	sender.OrdStatus = 0; // locally rejected.
	strncpy(sender.userOrderID,pAOMsg->userOrderID,63);
	sender.dstPrice = pAOMsg->Price;
	sender.totalLots = pAOMsg->OrderQty;
	if(pDealer) pPriceSource->SendMsg(&sender);
}

void SimPatsApiEngine::AddOrder(TPatsApiAddOrderMsg* pAOMsg)
{
	bool bSuccess = true;
	int nStatus = -1;
	string userID = pAOMsg->userOrderID;
	// 1.检查重复报单
	EnterCriticalSection(&cs);
	if(user2PatsMap.count(userID)>0) bSuccess=false;
	LeaveCriticalSection(&cs);
	if(!bSuccess) 
	{
		TRACE_LOG("[ENGINE][THREAD]AddOrder FAIL! Duplicate order.");
		AddOrder_Reject(pAOMsg);
		return;
	}

	// 2.获得交易帐号
	if(0==strlen(pAOMsg->traderAccount))
	{
		TraderAcctStruct account;
		ptGetTrader(0,&account);
		if(account.Tradable != 'Y') bSuccess = false;
		memset(pAOMsg->traderAccount,0,21);
		strncpy(pAOMsg->traderAccount,account.TraderAccount,20);
		TRACE_LOG("[ENGINE][THREAD]AddOrder, No TraderAccount indicated, use username's default account:%s.",pAOMsg->traderAccount);
	}
	if(!bSuccess) 
	{
		TRACE_LOG("[ENGINE][THREAD]AddOrder FAIL! TraderAccount is NOT tradable.");
		AddOrder_Reject(pAOMsg);
		return;
	}

	/*
	// **********************
	OrderTypeStruct orderType;
	AmendTypesArray amendTypesArray;
	int count=0;
	nStatus=ptCountOrderTypes(&count);
	for(int i=0;i<count-1;i++)
	{
		nStatus=ptGetOrderType(i,&orderType,&amendTypesArray[0]);
		printf("%d:%s.\n",i,orderType.OrderType);
	}
	// **********************
	*/
	// 3，组织报单
	NewOrderStruct order;
	memset(&order,0,sizeof(NewOrderStruct));
	sprintf(order.TraderAccount,"%s",pAOMsg->traderAccount);
	sprintf(order.OrderType,"Limit");
	sprintf(order.ExchangeName,"%s",pAOMsg->exchangeName);
	sprintf(order.ContractName,"%s",pAOMsg->contractName);
	sprintf(order.ContractDate,"%s",pAOMsg->MaturityMonthYear);
	order.BuyOrSell=pAOMsg->BuySell;
	order.OpenOrClose=pAOMsg->OpenClose;
	sprintf(order.Price,"%f",pAOMsg->Price);
	order.Lots=pAOMsg->OrderQty;
	sprintf(order.Price2,"\0");
	order.LinkedOrder[0]='\0';
	order.Xref=0;
	order.XrefP=0;
	order.TriggerDate[0]='\0';
	order.TriggerTime[0]='\0';

	// 4.发送报单
	TRACE_LOG("[ENGINE][THREAD]AddOrder, Placing order using Exchange=%s Commodity=%s Contract=%s BuySell=%c Volume=%d Price=%s",order.ExchangeName,order.ContractName,order.ContractDate,order.BuyOrSell,order.Lots,order.Price);

	OrderIDStr orderID;
	if(ptSuccess==(nStatus=ptAddOrder(&order,&orderID)))
	{// success, bond userid with patsid
		TRACE_LOG("[ENGINE][THREAD]AddOrder, Call ptAddOrder OK.");
		string patsID = orderID;
		EnterCriticalSection(&cs);
		user2PatsMap.insert(make_pair(userID,patsID));
		pats2UserMap.insert(make_pair(patsID,userID));
		LeaveCriticalSection(&cs);
	}
	else
	{// fail
		TRACE_LOG("[ENGINE][THREAD]AddOrder, Call ptAddOrder FAIL! code=%d.",nStatus);
		bSuccess = false;
		AddOrder_Reject(pAOMsg);
		return;
	}
}

void SimPatsApiEngine::CancelOrder(TPatsApiCancelOrderMsg* pCOMsg)
{
	bool bSuccess = true;
	int nStatus = -1;
	string userID = pCOMsg->userOrderID;
	string patsID = "";
	// 1.获得patsID
	EnterCriticalSection(&cs);
	map<string,string>::iterator iter = user2PatsMap.find(userID);
	if(iter != user2PatsMap.end())
	{
		patsID = iter->second;
	}		
	else bSuccess=false;
	LeaveCriticalSection(&cs);
	if(!bSuccess) 
	{
		TRACE_LOG("[ENGINE][THREAD]CancelOrder, FAIL! The order to be canceled is not in this Engine.");
		return;
	}

	// 2.发送报单
	char buffPatsID[11];
	sprintf(buffPatsID,"%s",patsID.c_str());
	if(ptSuccess==(nStatus=ptCancelOrder((OrderIDPtr)buffPatsID)))
	{// success, bond userid with patsid
		TRACE_LOG("[ENGINE][THREAD]CancelOrder, Call ptCancelOrder OK. userid %s, patsid %s.",userID.c_str(),patsID.c_str());
	}
	else
	{// fail
		TRACE_LOG("[ENGINE][THREAD]CancelOrder, Call ptCancelOrder FAIL! code=%d,userid %s, patsid %s.",nStatus,userID.c_str(),patsID.c_str());
	}
}

// ptGetContract接口函数似乎有bug，容易导致调用者的栈空间错误。怀疑其在写ContractStructPtr时，有越界。
// 暂时把结构缓冲区放大到4k，可以避免该问题。
void SimPatsApiEngine::GetAllContractInfo(void)
{
	int ret,count;
	ret = ptCountContracts(&count);
	if(ret!=ptSuccess||count==0)
	{
		TRACE_LOG("[ENGINE][THREAD][warning]ptCountContracts has ret=%d, count=%d.",ret,count);
		return;
	}
	string cn,cd,en,ed,ltd;
	int legNumber;
	char tradeable;
	char securityBuff[4096];
	ContractStructPtr pContractInfo = (ContractStructPtr)securityBuff;
	//ContractStruct contractInfo;
	TRACE_LOG("[ENGINE][THREAD][info]Get %d contracts infos.",count);
	int baseCount=0;
	for(int i=0;i<count;i++)
	//for(int i=0;i<100;i++)
	{
		//memset(&contractInfo,0,sizeof(contractInfo));
		memset(securityBuff,0,4096);
		ret = ptGetContract(i,pContractInfo);
		if(ret!=ptSuccess)
		{
			TRACE_LOG("[ENGINE][THREAD][warning]ptGetContract has ret=%d when i=%d,count=%d.",ret,i,count);
			break;
		}
		cn = pContractInfo->ContractName;
		cd = pContractInfo->ContractDate;
		en = pContractInfo->ExchangeName;
		ed = pContractInfo->ExpiryDate;
		ltd = pContractInfo->LastTradeDate;
		legNumber = pContractInfo->NumberOfLegs;
		tradeable = pContractInfo->Tradable;
		if(legNumber==1&&tradeable=='Y')
		{
			TRACE_LOG("[ENGINE][THREAD][info]contract[%d] info, cn=%s,cd=%s,en=%s,ed=%s,ltd=%s,legs=%d,tradeable=%c.",
				i,cn.c_str(),cd.c_str(),en.c_str(),ed.c_str(),ltd.c_str(),legNumber,tradeable);
			baseCount++;
		}
	}
	TRACE_LOG("[ENGINE][THREAD][info]Get %d base contracts.",baseCount);
}

// 报单状态汇报
void SimPatsApiEngine::OnInnerOR(TPatsApiInnerORMsg* pMsg)
{
	string jPatsID="";
	string jOldPatsID="";
	if(strlen(pMsg->oldPatsOrderID)>0&&strcmp(pMsg->oldPatsOrderID,pMsg->patsOrderID)!=0)
	{
		jPatsID = pMsg->patsOrderID;
		jOldPatsID = pMsg->oldPatsOrderID;
		EnterCriticalSection(&cs);
		map<string,string>::iterator iter = pats2UserMap.find(jOldPatsID);
		if(iter != pats2UserMap.end())
		{
			string jUserID = iter->second;
			pats2UserMap.erase(jOldPatsID);
			pats2UserMap[jPatsID]=jUserID;
			user2PatsMap[jUserID]=jPatsID;
			TRACE_LOG("[ENGINE][THREAD]OnInnerOR, Updates patsOrderID Mapping from <%s,%s> to <%s,%s>.",jOldPatsID.c_str(),jUserID.c_str(),jPatsID.c_str(),jUserID.c_str());
		}		
		LeaveCriticalSection(&cs);
	}

	int nStatus = -1;
	OrderDetailStruct orderDetail;
	//call getOrder to get the details
	if(ptSuccess!=(nStatus=ptGetOrderByID((OrderIDPtr)(pMsg->patsOrderID),&orderDetail, 0)))
	{ // fail
		TRACE_LOG("[ENGINE][THREAD]OnInnerOR, Call ptGetOrderByID FAIL! code=%d",nStatus);
	}
	else
	{ // success, only report in these status:1 - working, 2 - rejected, 3 - partfilled, 4 - filled, 5 - cancelled, 6 - externalcancelled
		int userStatus = 0;
		string patsID = orderDetail.OrderID;
		TRACE_LOG("[ENGINE][THREAD]OnInnerOR, Order %s is in State %d(%s), has filled %d times and %d lot(s), with avg Px %s. ",
			orderDetail.OrderID,orderDetail.Status,szOrderStatusDesc[orderDetail.Status],orderDetail.NoOfFills,orderDetail.AmountFilled,orderDetail.AveragePrice);
		//if(orderDetail.Status == ptQueued)
		switch(orderDetail.Status)
		{
		case ptWorking:
			userStatus = 1;
			break;
		case ptRejected:
			userStatus = 2;
			break;
		case ptPartFilled:
			userStatus = 3;
			break;
		case ptFilled:
			userStatus = 4;
			break;
		case ptCancelled:
			userStatus = 5;
			break;
		case ptExternalCancelled:
			userStatus = 6;
			break;
		default:
			;
		}
		if(userStatus>0)
		{ // report to user
			string userID="";
			EnterCriticalSection(&cs);
			map<string,string>::iterator iter = pats2UserMap.find(patsID);
			if(iter != pats2UserMap.end())
			{
				userID = iter->second;
			}		
			LeaveCriticalSection(&cs);
			if(userID!="")
			{
				TPatsApiOrderRespMsg sender;
				memset(&sender,0,sizeof(TPatsApiOrderRespMsg));
				sender.type = MSGTYPE_PATSAPI_OR;
				sender.len = sizeof(TPatsApiOrderRespMsg)-sizeof(TRawMsg);
				sender.OrdStatus = userStatus; 
				strncpy(sender.userOrderID,userID.c_str(),63);
				sender.dstPrice = atof(orderDetail.Price);
				sender.totalLots = orderDetail.Lots;
				sender.avgPrice = atof(orderDetail.AveragePrice);
				sender.filledLots = orderDetail.AmountFilled;
				if(pDealer) pDealer->SendMsg(&sender);
			}
		}
	}
}

void SimPatsApiEngine::OnInnerFR(TPatsApiInnerFRMsg* pMsg)
{
	int nStatus = -1;
	FillStruct fillInfo;
	if(ptSuccess!=(nStatus=ptGetFillByID((FillIDPtr)(pMsg->patsFillID),&fillInfo)))
	{ // fail
		TRACE_LOG("[ENGINE][THREAD]OnInnerFR, Call ptGetFillByID FAIL! code=%d.",nStatus);
	}
	else
	{
		TRACE_LOG("[ENGINE][THREAD]OnInnerFR, Order %s has a tpye%d fill (%dlot %s) at %s %s.",fillInfo.OrderID,
			fillInfo.FillType,fillInfo.Lots,fillInfo.Price,fillInfo.DateFilled,fillInfo.TimeFilled);
		string userID="";
		string patsID = pMsg->patsOrderID;
		EnterCriticalSection(&cs);
		map<string,string>::iterator iter = pats2UserMap.find(patsID);
		if(iter != pats2UserMap.end())
		{
			userID = iter->second;
		}		
		LeaveCriticalSection(&cs);
		if(userID!=""&&fillInfo.FillType==ptNormalFill)
		{
			TPatsApiFillRptMsg sender;
			memset(&sender,0,sizeof(TPatsApiFillRptMsg));
			sender.type = MSGTYPE_PATSAPI_FR;
			sender.len = sizeof(TPatsApiFillRptMsg)-sizeof(TRawMsg);
			sender.fillType = 0; 
			strncpy(sender.userOrderID,userID.c_str(),63);
			strncpy(sender.exchOrderID,fillInfo.ExchOrderId,30);
			sender.filledPrice = atof(fillInfo.Price);
			sender.filledLots = fillInfo.Lots;
			strncpy(sender.dateFilled,fillInfo.DateFilled,8);
			strncpy(sender.timeFilled,fillInfo.TimeFilled,6);
			if(pDealer) pDealer->SendMsg(&sender);
		}
	}
}

void SimPatsApiEngine::OnInnerMsg(TPatsApiInnerMsgMsg* pMsg)
{
	int nStatus = -1;
	MessageStruct msgInfo;
	if(ptSuccess!=(nStatus=ptGetUsrMsgByID((MsgIDPtr)(pMsg->patsMsgID),&msgInfo)))
	{ // fail
		TRACE_LOG("[ENGINE][THREAD]OnInnerMsg, Call ptGetUsrMsgByID FAIL! code=%d.",nStatus);
	}
	else
	{
		//TRACE_LOG("Msg %s is [%c,%c]%s.",msgInfo.MsgID,msgInfo.IsAlert,msgInfo.Status,msgInfo.MsgText);
		//acknowledge all pending msg.
		if(msgInfo.Status=='P')
		{
			if(ptSuccess!=(nStatus=ptAcknowledgeUsrMsg((MsgIDPtr)(pMsg->patsMsgID))))
			{ // fail
				TRACE_LOG("[ENGINE][THREAD]OnInnerMsg, Call ptAcknowledgeUsrMsg FAIL! code=%d.",nStatus);
			}
		}
		//tansfer msgcontent to user.
		TPatsApiHostAllmsgMsg sender;
		memset(&sender,0,sizeof(TPatsApiHostAllmsgMsg));
		sender.type = MSGTYPE_PATSAPI_ALLMSG;
		sender.len = sizeof(TPatsApiHostAllmsgMsg)-sizeof(TRawMsg);
		sprintf(sender.content,"[%c]%s",msgInfo.IsAlert,msgInfo.MsgText);
		if(pDealer) pDealer->SendMsg(&sender);
	}
}

bool SimPatsApiEngine::OnMsg(TRawMsg *msg, IMsg *remote_src)
{
	int nStatus = -1;
	if(this->m_nEngineStatus == ES_DEAD && msg->type >= MSGTYPE_PATSAPI_BASE && msg->type < MSGTYPE_PATSAPI_INNER_BASE)
	{
		TRACE_LOG("[ENGINE][THREAD]Engine is DEAD now, ignore all user msgs.");
		return TRUE;
	}
	if ( msg->type == MSGTYPE_PATSAPI_LOGON )
	{ // 登录消息
		TPatsApiLogonRespMsg sender;
		sender.type = MSGTYPE_PATSAPI_LOGON_RESP;
		sender.len = sizeof(TPatsApiLogonRespMsg)-sizeof(TRawMsg);
		if(this->m_nEngineStatus==ES_LOGIN)
		{
			TRACE_LOG("[ENGINE][THREAD]RECV LOGON request, this engine has already logged on.");		
			sender.FailCode = 0;
			strcpy(sender.Reason,"\0");
			if(pPriceSource) pPriceSource->SendMsg(&sender);
			if(pDealer) pDealer->SendMsg(&sender);
		}
		else if(this->m_nEngineStatus==ES_LOGOUT)
		{ // logon again
			//if(ptSuccess!=(nStatus=ptReady()))
			if(ptSuccess!=(nStatus=SetupConnection()))	
			{
				TRACE_LOG("[ENGINE][THREAD]RECV LOGON request and FAIL! Relogin failed, code=%d.",nStatus);
				sender.FailCode = 100;
				sprintf(sender.Reason,"Error from dll, code is %d.",nStatus);
				if(pPriceSource) pPriceSource->SendMsg(&sender);
				if(pDealer) pDealer->SendMsg(&sender);
			}
			else
			{
				TRACE_LOG("[ENGINE][THREAD]RECV LOGON request, this engine is attempting to relogin.");
			}
		}
		else if(this->m_nEngineStatus==ES_DEAD)
		{ // 
			TRACE_LOG("[ENGINE][THREAD]RECV LOGON request and FAIL! this engine is DEAD. ");
			sender.FailCode = 102;
			strcpy(sender.Reason,"Engine's status is ES_DEAD.");
			if(pPriceSource) pPriceSource->SendMsg(&sender);
			if(pDealer) pDealer->SendMsg(&sender);
		}
		else if(this->m_nEngineStatus==ES_BORN)
		{ // 
			TRACE_LOG("[ENGINE][THREAD]RECV LOGON request and FAIL! this engine is NOT initialized. ");
			sender.FailCode = 101;
			strcpy(sender.Reason,"Engine's status is ES_BORN.");
			if(pPriceSource) pPriceSource->SendMsg(&sender);
			if(pDealer) pDealer->SendMsg(&sender);
		}
		else
		{			
			TRACE_LOG("[ENGINE][THREAD]RECV LOGON request, this engine will login after hostlink connected.");
		}
	}
	else if( msg->type == MSGTYPE_PATSAPI_LOGOUT )
	{ // 退登消息
		if(this->m_nEngineStatus!=ES_LOGIN)
		{
			TRACE_LOG("[ENGINE][THREAD]RECV LOGOUT request and FAIL! this engine NOT in login status.");
			TPatsApiLogoutRespMsg sender;
			sender.type = MSGTYPE_PATSAPI_LOGOUT_RESP;
			sender.len = sizeof(TPatsApiLogoutRespMsg)-sizeof(TRawMsg);
			sender.code = 3;
			strcpy(sender.reason,"already log out.");
			if(pPriceSource) pPriceSource->SendMsg(&sender);
			if(pDealer) pDealer->SendMsg(&sender);
		}
		else
		{
			TRACE_LOG("[ENGINE][THREAD]RECV LOGOUT request and handling...");
			this->LogOut((TPatsApiLogoutMsg*)msg);
		}

	}
	else if ( msg->type == MSGTYPE_PATSAPI_MD)
	{ // 价格订阅消息
		this->SubPrice((TPatsApiMDReqMsg*)msg);
	}
	else if ( msg->type == MSGTYPE_PATSAPI_CP)
	{ // 价格订阅消息
		this->GetPrice((TPatsApiCPReqMsg*)msg);
	}
	else if ( msg->type == MSGTYPE_PATSAPI_AO)
	{ // 下单消息
		this->AddOrder((TPatsApiAddOrderMsg*)msg);
	}
	else if ( msg->type == MSGTYPE_PATSAPI_CO)
	{ // 撤单消息
		this->CancelOrder((TPatsApiCancelOrderMsg*)msg);
	}
	// **************************INNER MSG HERE**********************************
	else if ( msg->type == MSGTYPE_PATSAPI_INNER_HOSTLINK )
	{
		TPatsApiInnerHostMsg *pMsg = (TPatsApiInnerHostMsg*)msg;
		m_HostLink_Status = pMsg->status;
		if(ptLinkConnected==m_HostLink_Status && (this->m_nEngineStatus==ES_INIT || this->m_nEngineStatus==ES_LOGOUT))
		{
			TRACE_LOG("[ENGINE][THREAD]INNERRECV HostLink Connected, then sleep 4 secs and login.");
			Sleep(4000);
			Login();
		}
	}
	else if ( msg->type == MSGTYPE_PATSAPI_INNER_PRICELINK )
	{
		TPatsApiInnerPriceMsg *pMsg = (TPatsApiInnerPriceMsg*)msg;
		m_PriceLink_Status = pMsg->status;
		if(ptLinkConnected==m_PriceLink_Status)
		{
			TRACE_LOG("[ENGINE][THREAD]INNERRECV PriceLink Connected, then re-subscribe prices.");
			this->SubPrice2PriceServer();
		}
	}
	else if ( msg->type == MSGTYPE_PATSAPI_INNER_DL)
	{ 
		TRACE_LOG("[ENGINE][THREAD]INNERRECV Data Download Completed, report logon OK to user.");
		this->m_nEngineStatus = ES_LOGIN;
		TPatsApiLogonRespMsg sender;
		sender.type = MSGTYPE_PATSAPI_LOGON_RESP;
		sender.len = sizeof(TPatsApiLogonRespMsg)-sizeof(TRawMsg);
		sender.FailCode = 0;
		strcpy(sender.Reason,"\0");
		if(pPriceSource) pPriceSource->SendMsg(&sender);
		if(pDealer) pDealer->SendMsg(&sender);
		this->NotifyEngineStatus();
		//for test only
		//this->GetAllContractInfo();
	}
	else if ( msg->type == MSGTYPE_PATSAPI_INNER_LOGON)
	{ 
		TRACE_LOG("[ENGINE][THREAD]INNERRECV LogonStatus.");
		LogonStatusStruct logonStatus;
		ptGetLogonStatus(&logonStatus);
		if(ptLogonSucceeded!=logonStatus.Status)
		{	//we got an error so report it
			TRACE_LOG("[ENGINE][THREAD]LogonStatus failed status=%d reason=%s.",logonStatus.Status,logonStatus.Reason);
			TPatsApiLogonRespMsg sender;
			sender.type = MSGTYPE_PATSAPI_LOGON_RESP;
			sender.len = sizeof(TPatsApiLogonRespMsg)-sizeof(TRawMsg);
			sender.FailCode = 1;
			strncpy(sender.Reason,logonStatus.Reason,61);
			if(pPriceSource) pPriceSource->SendMsg(&sender);
			if(pDealer) pDealer->SendMsg(&sender);
		}
		else
		{
			TRACE_LOG("[ENGINE][THREAD]Logon proceding and waiting for DLComplete, default account=%s",logonStatus.DefaultTraderAccount);
			// *********** only for fcstone gateway ************************
			TRACE_LOG("[ENGINE][THREAD]For fcstone, we pass through DLComplete.");
			this->m_nEngineStatus = ES_LOGIN;
			TPatsApiLogonRespMsg sender;
			sender.type = MSGTYPE_PATSAPI_LOGON_RESP;
			sender.len = sizeof(TPatsApiLogonRespMsg)-sizeof(TRawMsg);
			sender.FailCode = 0;
			strcpy(sender.Reason,"\0");
			if(pPriceSource) pPriceSource->SendMsg(&sender);
			if(pDealer) pDealer->SendMsg(&sender);
			this->NotifyEngineStatus();
			//**************************************************************
		}
	}
	else if ( msg->type == MSGTYPE_PATSAPI_INNER_MDUPD )
	{
		//TRACE_LOG("[ENGINE][THREAD]INNERRECV MarketUpdate.");
		TPatsApiInnerMDUpdMsg *pMsg = (TPatsApiInnerMDUpdMsg*)msg;
		PriceStruct newPrice;
		//call getPrice on this contract
		TRACE_LOG("[ENGINE][THREAD]INNERRECV MarketUpdate %s-%s@%s.",(ConNamePtr)(pMsg->contractName),(ConDatePtr)(pMsg->MaturityMonthYear),(ExchNamePtr)(pMsg->exchangeName));
		if(ptSuccess!=(nStatus=ptGetPriceForContract((ExchNamePtr)(pMsg->exchangeName),(ConNamePtr)(pMsg->contractName),(ConDatePtr)(pMsg->MaturityMonthYear),&newPrice)))
		{
			TRACE_LOG("[ENGINE][THREAD]Call ptGetPriceForContract failed %d",nStatus);
		}
		else
		{ // send price to upper
			TRACE_LOG("[ENGINE][THREAD]Price Bid %s [%d] Offer %s [%d] Last %s [%d]",newPrice.Bid.Price,newPrice.Bid.Volume,newPrice.Offer.Price,newPrice.Offer.Volume,newPrice.Last0.Price,newPrice.Last0.Volume);
			//if( (newPrice.ChangeMask&ptChangeBid) || (newPrice.ChangeMask&ptChangeOffer) )// || (newPrice.ChangeMask&ptChangeLast) )
			if((newPrice.ChangeMask&ptChangeImpliedBid) || (newPrice.ChangeMask&ptChangeImpliedOffer) || (newPrice.ChangeMask&ptChangeBid) || (newPrice.ChangeMask&ptChangeOffer))
			{
				TPatsApiMDSRespMsg sender;
				sender.type = MSGTYPE_PATSAPI_MD_SRESP;
				sender.len = sizeof(TPatsApiMDSRespMsg)-sizeof(TRawMsg);

				if( atof(newPrice.ImpliedBid.Price) < atof(newPrice.Bid.Price))
				{
					sender.bidprice = atof(newPrice.Bid.Price);
				}
				else
				{
					sender.bidprice = atof(newPrice.ImpliedBid.Price);
				}

				if( atof(newPrice.ImpliedOffer.Price) < atof(newPrice.Offer.Price) && atof(newPrice.ImpliedOffer.Price) > 0)
				{
					sender.askprice = atof(newPrice.ImpliedOffer.Price);
				}
				else
				{
					sender.askprice = atof(newPrice.Offer.Price);
				}
				
				
				sender.tradeprice = atof(newPrice.Last0.Price);
				strncpy(sender.exchangeName,pMsg->exchangeName,11);
				strncpy(sender.contractName,pMsg->contractName,11);
				strncpy(sender.MaturityMonthYear,pMsg->MaturityMonthYear,51);
				if(newPrice.ChangeMask&ptChangeImpliedBid)
				{
					sender.hour = newPrice.ImpliedBid.Hour;
					sender.min = newPrice.ImpliedBid.Minute;
					sender.sec = newPrice.ImpliedBid.Second;
				}
				else
				{
					sender.hour = newPrice.ImpliedOffer.Hour;
					sender.min = newPrice.ImpliedOffer.Minute;
					sender.sec = newPrice.ImpliedOffer.Second;
				}
				//patsapi格式名称为 contract-datetime@exchange
				/*
				TRACE_LOG("[ENGINE][THREAD]Report:contract[%s-%s@%s] Bid %s [%d][age:%d] Offer %s [%d][age:%d] Last %s [%d], at %02u:%02u:%02u.",
					(pMsg->exchangeName),(pMsg->contractName),(pMsg->MaturityMonthYear),
					newPrice.ImpliedBid.Price,newPrice.ImpliedBid.Volume,newPrice.ImpliedBid.AgeCounter,newPrice.ImpliedOffer.Price,newPrice.ImpliedOffer.Volume,newPrice.ImpliedOffer.AgeCounter,
					newPrice.Last0.Price,newPrice.Last0.Volume,sender.hour,sender.min,sender.sec);
				*/
				if(pPriceSource) pPriceSource->SendMsg(&sender);
			}
		}
	}
	else if ( msg->type == MSGTYPE_PATSAPI_INNER_OR )
	{
		TRACE_LOG("[ENGINE][THREAD]INNERRECV OrderReport.");
		this->OnInnerOR((TPatsApiInnerORMsg*)msg);
	}
	else if ( msg->type == MSGTYPE_PATSAPI_INNER_FR )
	{
		TRACE_LOG("[ENGINE][THREAD]INNERRECV FillReport.");
		this->OnInnerFR((TPatsApiInnerFRMsg*)msg);
	}
	else if ( msg->type == MSGTYPE_PATSAPI_INNER_MESSAGE )
	{
		TRACE_LOG("[ENGINE][THREAD]INNERRECV Messages.");
		this->OnInnerMsg((TPatsApiInnerMsgMsg*)msg);
	}
	else if ( msg->type == MSGTYPE_PATSAPI_INNER_FORCELOGOUT)
	{ 
		TRACE_LOG("[ENGINE][THREAD]INNERRECV ForceLogout.");
		if(this->m_nEngineStatus!= ES_DEAD)
		{
			this->m_nEngineStatus = ES_DEAD;
			this->NotifyEngineStatus();
		}
		TRACE_LOG("[ENGINE][THREAD]Report ForceLogout to user.");
		TPatsApiLogoutRespMsg sender;
		sender.type = MSGTYPE_PATSAPI_LOGOUT_RESP;
		sender.len = sizeof(TPatsApiLogoutRespMsg)-sizeof(TRawMsg);
		sender.code = 2;
		strcpy(sender.reason,"forced log out.");
		if(pPriceSource) pPriceSource->SendMsg(&sender);
		if(pDealer) pDealer->SendMsg(&sender);
		
	}
	else
	{
		TRACE_LOG("[ENGINE][THREAD]Unknown message 0x%x received from \'%s\'", 
			msg->type, remote_src ? remote_src->GetName() : "[Local]");
	}
	return TRUE;
}





