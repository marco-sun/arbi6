// Order.h: interface for the Order class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORDER_H__AC2ECB3C_2554_4A18_AF26_89EF3EDDF273__INCLUDED_)
#define AFX_ORDER_H__AC2ECB3C_2554_4A18_AF26_89EF3EDDF273__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable : 4786)
#include <string>
#include <list>
#include "Windows.h"
using namespace std;

enum KaiPing
{
    KAI_CANG,
	PING_CANG,
	PING_JIN
};

enum BuySell
{
	SELL,
	BUY
};

enum OrderStatus
{
	PENDING,
	FINISHED,
	CANCELED
};

class OrderDetail
{
public:
	OrderDetail(string orderId, int lot, double price);
	string orderId;
	int lot;
	double price;
};


class Order  
{
public:
	//Order();
	
	virtual ~Order();
	string getContract();
	enum BuySell getBuySell();
	enum KaiPing getKaiPing();
	int getLot();
	double getPrice();
	time_t getTimeStamp();
	time_t getCancelTime();

	enum OrderStatus getStatus();
	void cancelAfter(int interval);

	static Order* create(string contract,
	      enum BuySell buySell,
	      enum KaiPing kaiPing,
	      int lot,
	      double price);
	string getId(); 
	void addDetail(OrderDetail * orderDetail);
	bool isDealed();
	double avgDealPrice();
	void setRejected();
	bool isRejected();
	bool isFinish();

	void setCancel();
	bool isCanceled();

	void manuallyFinish();
	bool shouldCancel();

	string toString();
	void increaseCancelNumber();
	bool isMaxCancelNumber();
private:
	Order(string contract,
	      enum BuySell buySell,
	      enum KaiPing kaiPing,
	      int lot,
	      double price);
	string id;
	string contract;
	enum BuySell buySell;
	enum KaiPing kaiPing;
	int lot;
	double price;
	int CancelNumber;
	time_t timeStamp;
	time_t cancelTime;
	enum OrderStatus status;
	CRITICAL_SECTION cs;

	string strategyId;
	list<OrderDetail * > orderDetails;
	bool canceled;
	bool rejected;
	int dealedLot();
	void setStatus(enum OrderStatus status);
	void checkFinished();
};

class Orders  
{
public:
	Orders();
	virtual ~Orders();

	Orders * get(string status);

};

#endif // !defined(AFX_ORDER_H__AC2ECB3C_2554_4A18_AF26_89EF3EDDF273__INCLUDED_)
