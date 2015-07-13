// Order.cpp: implementation of the Order class.
//
//////////////////////////////////////////////////////////////////////


#include "Order.h"
#include "Uuid.h"
#include "CancelQueue.h"
#include "../LogStore.h"
#include <time.h>
#include "Contract.h"
//#include <iostream>
//using namespace std;

#define NEVER_CANCEL 10 * 60 * 60 * 1000

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
Order::Order()
{

}
*/
Order::Order(string contract,
      enum BuySell buySell,
      enum KaiPing kaiPing,
      int lot,
      double price)
{
	this->contract = contract;
	this->buySell = buySell;
	this->kaiPing = kaiPing;
	this->lot = lot;
	this->price = price;
	this->rejected = false;
	this->canceled = false;
	this->timeStamp = time(NULL);
	this->cancelTime = timeStamp + NEVER_CANCEL;
	this->status =  PENDING;
	checkFinished();
	InitializeCriticalSection(&cs);
}

Order* Order::create(string contract,
	      enum BuySell buySell,
	      enum KaiPing kaiPing,
	      int lot,
	      double price)
{
	Order * order = new	Order(contract, buySell, kaiPing, lot, price);
	order->id = Uuid::gen();
	order->CancelNumber = 0;
	return order;
}

Order::~Order()
{

}

void Order::setStatus(enum OrderStatus status)
{
	this->status = status;
}

enum OrderStatus Order::getStatus()
{
	return status;
}

string Order::getContract()
{
	return contract;
}

Orders::Orders()
{

}

Orders::~Orders()
{

}
Orders * Orders::get(string status)
{
	return NULL;
}

void Order::cancelAfter(int interval)
{
	this->cancelTime = this->timeStamp + interval;
	CancelQueue::getInstance()->add(this);
}

enum BuySell Order::getBuySell()
{
	return this->buySell;
}

enum KaiPing Order::getKaiPing()
{
	return this->kaiPing;
}

int Order::getLot()
{
	return this->lot;
}

double Order::getPrice()
{
	return this->price;
}

string Order::getId()
{
	return this->id;
} 

time_t Order::getTimeStamp()
{
	return timeStamp;
}

time_t Order::getCancelTime()
{
	return this->cancelTime;
}

bool Order::shouldCancel()
{
	time_t now = time(NULL);
	return now - cancelTime >= 0;
}

void Order::checkFinished()
{
	if (this->lot == dealedLot())
	{
		TRACE_LOG("Order finished lot:%d",lot);
		this->status = FINISHED;
	}
}

void Order::addDetail(OrderDetail * orderDetail)
{
	EnterCriticalSection(&cs);
	orderDetails.push_back(orderDetail);
	checkFinished();
	LeaveCriticalSection(&cs);
}

int Order::dealedLot()
{
	int deallot = 0;
	list<OrderDetail *>::iterator the_iterator = orderDetails.begin();
    while( the_iterator != orderDetails.end() ) {
        deallot+=( * the_iterator)->lot;
        the_iterator++;
	}
	return deallot;
}

bool Order::isDealed()
{
	return this->lot==dealedLot();
}

bool Order::isFinish()
{
    return isDealed() || isRejected() || isCanceled();
}

double Order::avgDealPrice()
{
	double weightedAverage = 0;
    list<OrderDetail *>::iterator the_iterator = orderDetails.begin();
    while( the_iterator != orderDetails.end() ) {
        weightedAverage += (*the_iterator)->lot * (*the_iterator)->price;
        the_iterator++;
	}
	int l = dealedLot();
	return (l == 0) ? 0 : weightedAverage / l;
}

void Order::setCancel()
{
	this->canceled = true; 
	this->status = CANCELED;
	Contract::get(this->contract)->increaseOrderCount();

}

bool Order::isCanceled()
{
	return (this->canceled || isMaxCancelNumber());
}

void Order::setRejected()
{
	this->rejected = true;
}

bool Order::isRejected()
{
	return this->rejected;
}

void Order::manuallyFinish()
{
	int l = this->lot - dealedLot();
	addDetail(new OrderDetail(this->id, l, this->price));
}

string Order::toString()
{
	//TODO: implement this
	return "";
}

void Order::increaseCancelNumber()
{
	this->CancelNumber++;
}

bool Order::isMaxCancelNumber()
{
	if(this->CancelNumber > 100)
	{
		TRACE_LOG("CancelNumber is more than 100!");
		return true;
	}
	return false;
}


///////////////////////////////////////////////////////////////////////////////
//class OrderDetail
///////////////////////////////////////////////////////////////////////////////

OrderDetail::OrderDetail(string orderId, int lot, double price)
{
	this->orderId = orderId;
	this->lot = lot;
	this->price = price;
}




