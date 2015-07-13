#pragma once
#include "../arbi/dealer.h"
class CatsDealer : public Dealer
{
public:
	CatsDealer(void);
	virtual ~CatsDealer(void);
	// �������ص��������,ͬ��������һ������£�dealer����Ҫ���е�¼��������ΪPriceSource�Ѿ���¼
	void start();
	//Dealer�ӿڵ�ʵ�ַ���
	virtual void placeOrder(Order *order);
	virtual void cancelOrder(Order *order);

private:
	void ReadIniConfig();
	Order* getOrder(string orderid);
	bool isStarted;
};

