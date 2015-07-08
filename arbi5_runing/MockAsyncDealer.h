// MockAsyncDealer.h: interface for the MockAsyncDealer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOCKASYNCDEALER_H__8CF38E5B_2F7B_4CB9_A0EE_BD3E49C0AD84__INCLUDED_)
#define AFX_MOCKASYNCDEALER_H__8CF38E5B_2F7B_4CB9_A0EE_BD3E49C0AD84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dealer.h"
#include <windows.h>
#include <map>
using namespace std;

typedef struct _inventory
{
	Order *order;
	int status;  //0 -- order wait for deal; 1 -- order wait for cancel.
} Inventory;

class MockAsyncDealer : public Dealer  
{
public:
	MockAsyncDealer();
	virtual ~MockAsyncDealer();

	void placeOrder(Order *order);
	void cancelOrder(Order *order);
    
private:
	static void threadDeal( void* pParams );
	void reportDeal(string orderID);
	map<string, Inventory> inventoryMap;
	CRITICAL_SECTION cs;
};

typedef struct _DealerThreadParam
{
	string orderID;
	MockAsyncDealer* obj;
} DealerThreadParam;

#endif // !defined(AFX_MOCKASYNCDEALER_H__8CF38E5B_2F7B_4CB9_A0EE_BD3E49C0AD84__INCLUDED_)
