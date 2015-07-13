#pragma once
#include "../arbi/dealer.h"
class CatsDealer : public Dealer
{
public:
	CatsDealer(void);
	virtual ~CatsDealer(void);
	// 交易网关的启动入口,同步函数，一般情况下，dealer不需要进行登录操作，因为PriceSource已经登录
	void start();
	//Dealer接口的实现方法
	virtual void placeOrder(Order *order);
	virtual void cancelOrder(Order *order);

private:
	void ReadIniConfig();
	Order* getOrder(string orderid);
	bool isStarted;
};

