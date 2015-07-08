// SpDealer.h: interface for the SpDealer class.
#if !defined SPDEALER_H
#define SPDEALER_H

#pragma once

#include "Dealer.h"
#include <windows.h>
#include "ClientSock.h"
#include "SPInfor.h"
#include <map>
using namespace std;

class SpDealerInventory
{
public:
	SpDealerInventory(Order *pOrder);
	virtual ~SpDealerInventory();

	Order *order;
	int localRef;
	int remoteRef; //nIntOrderNo
	int placeStatus; // 0 -- new order; 1 -- received by remote; 2 -- partial dealed; 3 -- all dealed; 4 -- canceled; 5 -- aborted.
	int dealedLot;
};

class SPDealer : public Dealer
{
public:
	SPDealer();
	~SPDealer();
	void start();
	void stop();
	virtual void placeOrder(Order *order);
	virtual void cancelOrder(Order *order);
	
private:
	static void dealer_receive_tcp( void* pParams );
	int  DecodeMsgHead(char * pmsg);
	int  DealUserLoginReply(char * pmsg);
	int  DealOrderReply(char * pmsg);
	int  DealClearTradeReply(char * pmsg);

	int getCurOrderRef();
	void SPUserLogin();
	void SPAccountLogin();
	void SPUserLogout();
	void SPAccountLogout();

	void ReadIniConfig();
	int  startTcp();
	int  closeTcp();

	SpDealerInventory* addInventory(Order *order);
	bool delInventory(SpDealerInventory* pDealerInventory);
	SpDealerInventory* getInventory(string key);
	SpDealerInventory* findInventorybyLocal(int localRef);
	SpDealerInventory* findInventorybyRemote(int remoteRef);

	HANDLE hLogin;
	bool started;
	CSPPublicFun m_sPPublic;
	CTcpClinet m_cTcpClinet;
	CRITICAL_SECTION cs;
	map<string, SpDealerInventory*> dealerInventoryMap;
	int iNextOrderRef; //报单引用
};

#endif 
