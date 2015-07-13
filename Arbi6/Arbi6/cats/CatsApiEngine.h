#pragma once
#include "catsapi.h"
#include "catsapi_acc.h"
#include "catsapi_algo.h"
#include "catsapi_md.h"
#include "catsapi_icf.h"
#include "catsapi_trade.h"
#include "MarketDataDefinition.h"
#include "../arbi/order.h"
#include <set>
#include <map>
using namespace std;

// Engine State 0-born(start state), 1-intialised, 2-log in, 3-log out(end state)
#define CATS_ES_BORN (0) 
#define CATS_ES_INIT (1) 
#define CATS_ES_LOGIN (2) 
#define CATS_ES_LOGOUT (3) 

class CatsApiEngine;

class CatsOrderRef
{
public:
	CatsApiEngine* pEngine;
	Order* pOrder;
};

class CatsOrderInventory
{
public:
	CatsOrderInventory(Order *pOrder);
	virtual ~CatsOrderInventory();
	Order *order;
	string orderNo;
	int status; 
};

// 报单状态记录
class CatsOrderInfo
{
public:
	CatsOrderInfo()
	{
		orderNo = "";
		stock = "";
		tradeSide = "";
		orderType = "";
		orderTime = "";
	}
	string orderNo;
	string stock;
	int orderQty;
	string tradeSide;
	double orderPrice;
	string orderType;
	int status; // 0-新建 1-部分成交 2-完全成交 3-部分撤单 4-全部撤单 5-订单拒绝
	int filledQty;
	int cancelQty;
	double avgPrice;
	string orderTime;
};

// 成交记录
class CatsTradeInfo
{
public:
	CatsTradeInfo()
	{
		tradeNo = "";
		orderNo = "";
		stock = "";
		quantity = "";
		tradeSide = "";
		price = "";
		tradeType = "";
		tradeTime = "";
	}
	string tradeNo;
	string orderNo;
	string stock;
	string quantity;
	string tradeSide;
	string price;
	string tradeType;
	string tradeTime;
};

class CatsApiEngine
{
public:
	static CatsApiEngine* getEngineInstance();
public:
	virtual ~CatsApiEngine(void);
	void add(string stock);
	bool placeOrder(Order* order);
	bool cancelOrder(Order* order);

	//*************回调函数******************
public:
	static void CATSAPIReadString( CATSHANDLE hCats, const char* key, string& outStr );
	static void BizSubCommonCallback(void* pArg);
	static void BizSubMDCallback(void* pArg);
	static void CATSAPIOnTrdReconnected(void *);
	static void CATSAPIOnTrdDisconnected(void *);
	static void CATSAPIOnHqReconnected(void *);
	static void CATSAPIOnHqDisconnected(void *);
	static void OnCATSConnected(void* pArg);
	static void OnCATSUserLogin(void* pArg);
	static void OnTradeAcctLogin(void* pArg);
	static void OnSingleOrder(void* pArg);
	static void OnCancelOrder(void* pArg);
	static void PubMarketDataCallback(void* pArg);
	static void PubOrderUpdateCallback(void* pArg);
	static void PubTradeUpdateCallback(void* pArg);

private:
	CatsApiEngine(void);
	void ReadIniConfig();
	bool init();
	bool logonCATS();
	bool logonAccount();
	bool listenStock(string stock);
	//bool subAssetInfo();
	bool subOrderUpdate();
	bool subTrade();
	bool insertOrder(string stock, string orderQty, string orderPrice, string tradeSide);
	bool cancelOrder(string orderNo);
	void cancelOrderResp(Order* pOrder);
	void orderResp(Order* pOrder, string orderNo);
	void subscribeBatch();
	bool uninit();
	void updateOrderInfo(CatsOrderInfo& orderInfo);
	void updateTradeInfo(CatsTradeInfo& tradeInfo);

private:
	// Status of the Engine
	// Engine State 0-born(start state), 1-intialised, 2-log in, 3-log out(end state)
	int m_nEngineStatus;
	// SubAsset, SubOrder, SubTrade
	string m_initStep; 
	// 存放还未初始化完成时就要求订阅的合约，待初始化结束后一并订阅
	set<string> m_contracts; 

	//CATS config
	string m_sTradingServerAddr;
	string m_sTradingServerPort;
	string m_sHqServerAddr;
	string m_sHqServerPort;
	string m_sCatsToken;
	string m_sCatsAcct;
	string m_sCatsPwd;
	string m_sAcctType;
	string m_sAcctId;
	string m_sAcctPwd;
	//CATS Handle
	void* m_hHandle; 

	static CatsApiEngine* pStaticEngine;

	CatsOrderInventory* addInventory(string orderNo, Order *order);
	bool delInventory(CatsOrderInventory* pDealerInventory);
	CatsOrderInventory* getInventory(string orderNo);
	CatsOrderInventory* findInventorybyOrderId(string orderId);
	CRITICAL_SECTION cs;
	//map<string,Order*>	cats2UserMap; // orderNo -> User Order
	map<string,string>	user2CatsMap; // User OrderId -> OrderNo
	map<string, CatsOrderInventory*> orderInventoryMap; // key is OrderNo


};

