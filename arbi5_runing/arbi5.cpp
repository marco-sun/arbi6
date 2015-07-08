// arbi5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Contract.h"
#include "IntentionPair.h"
#include "ArbitrageComparablePriceExpression.h"
#include "Strategy.h"
#include "Commodity.h"
#include "DayTradingStrategy.h"
#include "DiffExpression.h"
#include "LogPriceStrategy.h"
#include "ShootStrategy.h"
#include "FishStrategy.h"
#include "PriceSource.h"
#include "CancelQueue.h"
#include "Persistence.h"
#include "DealHistory.h"
#include "LogStore.h"
#include "Exchange.h"
#include "ImportExpression.h"
#include "KnownContract.h"

#include <time.h>
#include <iostream> 
#include <assert.h>



bool doubleEqual(double a, double b);


void testPrice()
{
	cout << "testPrice()" << endl;

	Contract::get("rb1112")->setPrice(0, 4790.0, 4785.0);
	assert(doubleEqual(4790.0, Contract::get("rb1112")->getPrice().getAsk()));
	assert(doubleEqual(4785.0, Contract::get("rb1112")->getPrice().getBid()));
}

void testIntentionPair()
{
	cout << "testIntentionPair()" << endl;

	Contract::get("JRUU1")->setPrice(400.0, 399.0);
    Contract::get("ru1109")->setPrice(33002.0, 33001.0);

    
    IntentionPair * p = IntentionPair::create("J-R","ru1109", "JRUU1", "ru1109.ask-JRUU1.bid", 1, 1);
	assert(IntentionPair::get("J-R") != NULL);

	assert(doubleEqual(399.0, IntentionPair::get("J-R")->getContract("contractB")->getPrice().getBid()));
	assert(doubleEqual(400.0, IntentionPair::get("J-R")->getContract("contractB")->getPrice().getAsk()));
	assert(doubleEqual(33001.0, IntentionPair::get("J-R")->getContract("contractA")->getPrice().getBid()));
	assert(doubleEqual(33002.0, IntentionPair::get("J-R")->getContract("contractA")->getPrice().getAsk()));

	assert(doubleEqual((33002.0*1*1+0)-(399.0*0.079*1000+0), IntentionPair::get("J-R")->getFirstDiff()));
	assert(doubleEqual((33001.0*1*1+0)-(400.0*0.079*1000+0), IntentionPair::get("J-R")->getSecondDiff()));


}

void testImportExpression()
{
	cout << "testImportExpression()" << endl;
	assert(ImportExpression::listAll().size() == 10);

	ImportExpression * e = ImportExpression::find("JRU");
	printf("%.8f\n",e->calc(276.4, 33001.0));
	assert(doubleEqual(6223.73264, e->calc(276.4, 33001.0)));
}

void testIntentionPairOrder()
{
	cout << "testIntentionPairOrder()" << endl;
	IntentionPair::clear();
	Contract::get("JRUU1")->setPrice(276.4, 399.0);
    Contract::get("ru1109")->setPrice(33002.0, 33001.0);
	Contract::get("LCA3M")->setPrice(7639.0, 7640.0);
    Contract::get("cu1203")->setPrice(53780.0, 53790.0);

	double REF_PARA_ONE = 0;
	double REF_PARA_TWO = 0;
	enum IntentionOrder ORDER_BY = ONE;
    
	IntentionPair * p1 = IntentionPair::create("cu-LCA3M","cu1203", "LCA3M", "cu1203.ask-LCA3M.bid", 1, 1, REF_PARA_ONE, REF_PARA_TWO, ORDER_BY);
    IntentionPair * p = IntentionPair::create("J-R-a","ru1109", "JRUU1", "ru1109.ask-JRUU1.bid", 1, 1, REF_PARA_ONE, REF_PARA_TWO, ORDER_BY);
	assert(p != NULL);
	printf("%.8f \n",p->importProfit());
	printf("%.8f \n",p->sortValue());
	assert(doubleEqual(6223.73264, p->importProfit()));
	assert(doubleEqual(0.18859224, p->sortValue()));
	cout << IntentionPair::listAll().front()->getContractA() << endl;
	cout << IntentionPair::sortListAll(UP).front()->getContractA() << endl;
	assert(IntentionPair::sortListAll(UP).front()->getContractA().compare("ru1109") == 0);
	assert(IntentionPair::sortListAll(DOWN).front()->getContractA().compare("cu1203") == 0);
	IntentionPair::clear();
}

void testIntentions()
{
	cout << "testIntentions()" << endl;
	Persistence::load();
	IntentionPair* ip = IntentionPair::get("j1201-rb1201");
    assert(ip != NULL);

	assert(ip->toString().compare("j1201-rb1201 rb1201 j1201 rb1201.ask-j1201.bid 5 1") == 0);
	//IntentionPair::create("ru1201-JRUZ1", "ru1201", "JRUZ1","ru1201.ask-JRUZ1.bid");
	Persistence::load();
	//assert(IntentionPair::listAll().size() == 1);
}

void testDiffExperssion()
{
	cout << "testDiffExperssion()" << endl;

	//Commodity::get("JRU")->setArbitrageComparablePriceExpression("P * 0.079 * 1000 + 0");
	//Commodity::get("ru")->setArbitrageComparablePriceExpression("P * 1 * 1 + 0");

	Contract::get("JRUU1")->setPrice(400.0, 399.0);
    Contract::get("ru1109")->setPrice(33002.0, 33001.0);
	
	DiffExpression * de = new DiffExpression("ru1109.ask-JRUU1.bid");

	assert(doubleEqual((33002.0*1*1+0)-(399.0*0.079*1000+0), de->getValue()));
    de->set("ru1109 . bid - JRUU1 . ask");
	assert(doubleEqual((33001.0*1*1+0)-(400.0*0.079*1000+0), de->getValue()));


    DiffExpression * de2 = de->flip("ru1109");
	assert(de2->toString().compare("ru1109.ask-JRUU1.ask") == 0);

    DiffExpression * de3 = de->flip("JRUU1");
	assert(de3->toString().compare("ru1109.bid-JRUU1.bid") == 0);


	de->set("j1201.bid-rb1201.ask");

	assert(doubleEqual(-7000, de->getValue("j1201", 1980, "rb1201", 4100)));

}

void testArbitrageComparablePriceExpression()
{
	cout << "testArbitrageComparablePriceExpression()" << endl;
    ArbitrageComparablePriceExpression exp;
	exp.set("P * 0.079 * 1000 + 0");
	assert(doubleEqual(399.0 * 0.079 * 1000 + 0, exp.getValueFor(399.0)));

	ArbitrageComparablePriceExpression * acpe = NULL;
	acpe = ArbitrageComparablePriceExpression::find("SBH3", "SR");
	assert(doubleEqual(399.0 * 6.4 * 1120 + 0, acpe->getValueFor(399.0)));
}


void testCondition()
{
	cout << "testCondition()" << endl;
	Contract::get("JRUU1")->setPrice(400.0, 399.0);
    Contract::get("ru1109")->setPrice(33002.0, 33001.0);

	Condition * condition = new Condition("ru1109 . ask - JRUU1 . bid", "<=", 1000);
	assert(!condition->isMeet());

    condition->set("ru1109 . ask - JRUU1 . bid", ">=", 1000);
	assert(condition->isMeet());

}

void testHoldingControlForDayTradingStrategy()
{
	cout << "testHoldingControlForDayTradingStrategy()" << endl;
    DayTradingStrategy dayTradingStrategy;

    TwoLegStrategy * mockKaiCangStrategy = new TwoLegStrategy();
	TwoLegStrategy * mockPingCangStrategy = new TwoLegStrategy();
    mockKaiCangStrategy->setHoldingOperation(INCREASE);
    mockPingCangStrategy->setHoldingOperation(DECREASE);

    dayTradingStrategy.setKaiCangStrategy(mockKaiCangStrategy);
    dayTradingStrategy.setPingCangStrategy(mockPingCangStrategy);

	assert(dayTradingStrategy.getHoldingCount() == 0);
    mockKaiCangStrategy->trig();
	assert(dayTradingStrategy.getHoldingCount() == 1);
    mockPingCangStrategy->trig();
	assert(dayTradingStrategy.getHoldingCount() == 0);

}

void testCommodity()
{
	cout << "testCommodity()" << endl;
	Commodity * comparedCommodity = Commodity::findFor("j1201");
    assert(doubleEqual(100 * 50 * 1 + 0, Commodity::get("rb")->calcArbitrageComparablePriceFor(100, comparedCommodity)));

    comparedCommodity = Commodity::findFor("au1201");
    assert(doubleEqual(100 * 6.4 * 0.03215434 + 0, Commodity::get("GC")->calcArbitrageComparablePriceFor(100, comparedCommodity)));

    comparedCommodity = Commodity::findFor("PLF2");
	assert(comparedCommodity != NULL);
    assert(doubleEqual(100 * 1 * 1 + 0, Commodity::get("GC")->calcArbitrageComparablePriceFor(100, comparedCommodity)));

}

void testContract()
{
	cout << "testContract()" << endl;
	Contract::get("JRUU1")->setPrice(400.0, 399.0);

	assert(doubleEqual(400.0*0.079*1000+0, Contract::get("JRUU1")->getArbitrageComparablePriceFor("ask", "ru1201")));
	assert(doubleEqual(399.0*0.079*1000+0, Contract::get("JRUU1")->getArbitrageComparablePriceFor("bid", "ru1201")));
}

void testFixRonhonName()
{
	cout<< "testFixContract()" << endl;
	Contract * c1 = Contract::get("HGF3");
	assert(c1->getFixName().compare("HG-F3@NYMEX") == 0);
	cout << c1->getRonhonName() << endl;
	assert(c1->getRonhonName().compare("NYMEX-HG-JAN13") == 0);

	Contract * c2 = Contract::get("LCA3M");
	assert(c2->getFixName().compare("LCA-3M@LME") == 0);
	cout << c2->getRonhonName() << endl;
	assert(c2->getRonhonName().compare("LME-LCA-3M") == 0);

}

void testTrigPeriodly()
{
    cout << "testTrigPeriodly()" << endl;
	Contract::get("JRUU1")->setPrice(400.0, 399.0);
	LogPriceStrategy s;
    new LogPriceStrategy(1);
	s.set("JRUU1");
	s.start();
	Sleep(1 * 1000);
    s.start();
	Sleep(1 * 1000);
}

void testStrategy()
{
	cout << "testStrategy()" << endl;
	Leg legA;
    legA.set("2639", SELL, KAI_CANG, "JRUU1", 1, 0);
    Leg legB;
	legB.set("10110882", BUY, KAI_CANG, "ru1109", 1, 0);
	Condition * condition = new Condition("ru1109 . ask - JRUU1 . bid", "<=", 1000);

	TwoLegStrategy * tls = TwoLegStrategy::create("J-R-1");
	tls->setLegA(legA);
    tls->setLegB(legB);
	tls->setCondition(*condition);
    tls->getOrders()->get("pending");

}

void testShootStrategyKaiCang()
{
	cout << "testShootStrategyKaiCang()" << endl;

	DealHistory::getInstance()->clear();

    ShootStrategy * s = (ShootStrategy *)TwoLegStrategy::createShootStrategy("J-R-2");
	s->setLegA(new Leg("2639", SELL, KAI_CANG, "JRUU1", 1, 0));
    s->setLegB(new Leg("10110882", BUY, KAI_CANG, "ru1109", 1, 0));
	s->setCondition(new Condition("ru1109 . ask - JRUU1 . bid", ">=", 1000));
	s->setHoldingLimit(0, 5);

	Contract::get("JRUU1")->setPrice(400.0, 399.0);
    Contract::get("ru1109")->setPrice(33002.0, 33001.0);

	s->work();
    s->trig();

    Order * o = s->getFirstOrder();
	assert(o != NULL);
	assert(o->getContract().compare("JRUU1") == 0);
	o->addDetail(new OrderDetail(o->getId(), o->getLot(), o->getPrice()));


	s->trig();
    o = s->getSecondOrder();
	assert(o != NULL);
	assert(o->getContract().compare("ru1109") == 0);
	o->addDetail(new OrderDetail(o->getId(), o->getLot(), o->getPrice()));

    s->trig();
	assert(s->getHoldingCount() == 1);

    DealHistory * dh = DealHistory::getInstance();
	assert(dh->listAll().size() == 1);
	string history = dh->listAll().back();
	cout << history << endl;
	//assert(history.compare("J-R-2 于 10时31分00秒 成交差价:1481.00") == 0);

	o = s->getFirstOrder();
	assert(o == NULL);
    o = s->getSecondOrder();
    assert(o == NULL);

    s->trig();

    o = s->getFirstOrder();
	assert(o != NULL);
	assert(o->getContract().compare("JRUU1") == 0);
	o->addDetail(new OrderDetail(o->getId(), o->getLot(), o->getPrice()));

	s->trig();
    o = s->getSecondOrder();
	assert(o != NULL);
	assert(o->getContract().compare("ru1109") == 0);
	o->setCancel();

	
	s->trig();
	
	assert(s->isSecondLegMissed());
	s->missedLegHandled();
	assert(dh->listAll().size() == 2);

	cout << dh->listAll().back() << endl;

	o = s->getFirstOrder();
	assert(o == NULL);
    o = s->getSecondOrder();
    assert(o == NULL);
}

void testLegShoot()
{
	cout << "testLegShoot()" << endl;

	Contract::get("JRUU1")->setPrice(400.0, 399.0);
	Leg l;
	l.set("2639", SELL, KAI_CANG, "JRUU1", 1, 0);
    Order * o = l.shoot(0);

	assert(o->getContract().compare("JRUU1") == 0);
	assert(o->getBuySell() == SELL);
	assert(o->getKaiPing() == KAI_CANG);
	assert(o->getLot() == 1);
	assert(doubleEqual(o->getPrice(), 399));

}

void testShootStrategyPingCang()
{
}

void testShootStrategyYiCang()
{
}

void testFishStrategyKaiCang3()
{
	cout << "testFishStrategyKaiCang3()" << endl;


    FishStrategy * s = (FishStrategy *)TwoLegStrategy::createFishStrategy("J-R-10");

	s->setLegA(new Leg("2639", SELL, KAI_CANG, "JRUU1", 1, 0));
    s->setLegB(new Leg("10110882", BUY, KAI_CANG, "ru1109", 1, 5));
	s->setCondition(new Condition("ru1109 . ask - JRUU1 . bid", "<=", -2600));
	s->setHoldingLimit(0, 5);

	Contract::get("JRUU1")->setPrice(450.0, 399.0);
    Contract::get("ru1109")->setPrice(33002.0, 33001.0);

	s->work();
    s->trig();
    Order * o = s->getFirstOrder();
	assert(o != NULL);
	assert(o->getContract().compare("JRUU1") == 0);
	cout << o->getPrice() <<endl;
	assert(doubleEqual(450.7, o->getPrice()));
}

void testFishStrategyKaiCang2()
{
	cout << "testFishStrategyKaiCang2()" << endl;

    FishStrategy * s = (FishStrategy *)TwoLegStrategy::createFishStrategy("R-R-3");

	s->setLegA(new Leg("10110882", BUY, KAI_CANG, "ru1109", 1, 0));
    s->setLegB(new Leg("10110882", SELL, KAI_CANG, "ru1110", 1, 5));
	s->setCondition(new Condition("ru1109.ask-ru1110.bid", "<=", -1000));
	s->setHoldingLimit(0, 5);

	Contract::get("ru1110")->setPrice(34010.0, 34005.0);
    Contract::get("ru1109")->setPrice(33010.0, 33005.0);

	s->work();
    s->trig();
    Order * o = s->getFirstOrder();
	assert(o != NULL);
	assert(o->getContract().compare("ru1109") == 0);
	//cout << o->getPrice() << endl;
	assert(doubleEqual(33005.0, o->getPrice()));
	o->addDetail(new OrderDetail(o->getId(), o->getLot(), o->getPrice()));
}

void testFishStrategyKaiCang()
{
	cout << "testFishStrategyKaiCang()" << endl;

    FishStrategy * s = (FishStrategy *)TwoLegStrategy::createFishStrategy("J-R-3");

	s->setLegA(new Leg("2639", SELL, KAI_CANG, "JRUU1", 1, 0));
    s->setLegB(new Leg("10110882", BUY, KAI_CANG, "ru1109", 1, 5));
	s->setCondition(new Condition("ru1109 . ask - JRUU1 . bid", "<=", -2500));
	s->setHoldingLimit(0, 5);

	Contract::get("JRUU1")->setPrice(450.0, 399.0);
    Contract::get("ru1109")->setPrice(33002.0, 33001.0);

	s->work();
    s->trig();
    Order * o = s->getFirstOrder();
	assert(o != NULL);
	assert(o->getContract().compare("JRUU1") == 0);
	//cout << o->getPrice() <<endl;
	assert(doubleEqual(449.9, o->getPrice()));
	o->addDetail(new OrderDetail(o->getId(), o->getLot(), o->getPrice()));

	s->trig();
    o = s->getSecondOrder();
	assert(o != NULL);
	assert(o->getContract().compare("ru1109") == 0);
    assert(doubleEqual(33002 + 5 * 5, o->getPrice()));
	o->addDetail(new OrderDetail(o->getId(), o->getLot(), o->getPrice()));

    s->trig();
	assert(s->getHoldingCount() == 1);
	o = s->getFirstOrder();
	assert(o == NULL);
	o = s->getSecondOrder();
	assert(o == NULL);


	//Withdraw fish because of condition
	CancelQueue * cq = CancelQueue::getInstance();
	cq->checkQueue();
	assert(cq->size() == 0);

	s->trig();
    o = s->getFirstOrder();
	assert(o != NULL);

	Contract::get("JRUU1")->setPrice(450.0, 399.0);
    Contract::get("ru1109")->setPrice(33002.0 + 100, 33001.0 + 100);

	s->trig();
    assert(cq->size() == 1);
	o->setCancel();

	s->trig();
    o = s->getFirstOrder();
	assert(o == NULL);

	//Withdraw fish because of not first
	cq->checkQueue();
	assert(cq->size() == 0);

	Contract::get("JRUU1")->setPrice(450.0, 399.0);
    Contract::get("ru1109")->setPrice(33002.0, 33001.0);

	s->trig();
    o = s->getFirstOrder();
	assert(o != NULL);
	assert(doubleEqual(449.9, o->getPrice()));

	Contract::get("JRUU1")->setPrice(449.8, 399.0);
    Contract::get("ru1109")->setPrice(33002.0, 33001.0);

	s->trig();
	cout << cq->size() << endl;
	assert(cq->size() == 1);

	o->setCancel();
	cq->checkQueue();

	s->trig();
    o = s->getFirstOrder();
	assert(o == NULL);

	s->trig();
    o = s->getFirstOrder();
	assert(o != NULL);
	assert(doubleEqual(449.7, o->getPrice()));


	//Second leg price change more than 0.2%
	assert(cq->size() == 0);

	Contract::get("JRUU1")->setPrice(449.7, 399.0);
    Contract::get("ru1109")->setPrice(33002.0, 33001.0);

	s->trig();
    o = s->getFirstOrder();
	assert(o != NULL);

	Contract::get("JRUU1")->setPrice(449.7, 399.0);
    Contract::get("ru1109")->setPrice(33002.0 - 70, 33001.0 - 70);

	s->trig();
 	assert(cq->size() == 1);
	o->setCancel();
	cq->checkQueue();

	s->trig();
    o = s->getFirstOrder();
	assert(o == NULL);

	s->trig();
    o = s->getFirstOrder();
	assert(o != NULL);
	assert(doubleEqual(449.6, o->getPrice()));

}

void testFishStrategyPingCang()
{
}

void testFishStrategyYiCang()
{
}

void testDayTradingYiCang()
{
}

void testIntentionPairWithPriceSource()
{

	cout << "testIntentionPairWithPriceSource()" << endl;
	PriceSource::init();
	IntentionPair * ip[5];
	ip[0]= IntentionPair::create("J-rb","rb1201", "j1201", "j1201.bid-rb1201.ask", 1, 5);
	ip[1]  = IntentionPair::create("ru1201-JRUZ1", "ru1201", "JRUZ1","ru1201.ask-JRUZ1.bid", 1, 1);
	ip[2]  = IntentionPair::create("cu1112-cu1111", "cu1201", "cu1111","cu1201.ask-cu1111.bid", 1, 4);
	ip[3]  = IntentionPair::create("au1201-au1111", "au1201", "au1111","au1201.ask-au1111.bid", 1, 3);
	ip[4]  = IntentionPair::create("rb1201-rb1111", "rb1201", "rb1111","rb1201.ask-rb1111.bid", 1, 1);
    //ip->set("cu1112", "LCA3M", "cu1112 . bid - LCA3M . ask");
	//ip->set("au1112", "GCZ1", "au1112 . bid - GCZ1 . ask");
	//ip->set("TA201", "CLZ1", "TA201 . bid - CLZ1 . ask");
	//ip->set("rb1201", "j1201", "j1201 . bid - rb1201 . ask");
	Sleep(5000);
	time_t t1 = time(NULL);
	cout << t1 << endl;
for(int j = 0 ; j < 5 ;j++)
{
	TRACE_LOG("start");
	for(int i = 0; i < 5; i++)
	{
		cout << ip[i]->getFirstDiff() << ":" << ip[i]->getSecondDiff() << ":";
		cout << ip[i]->getFirstAsk() << ":" << ip[i]->getFirstBid() << endl;
	}
	TRACE_LOG("end");
	Sleep(500);
}
	time_t t2 = time(NULL);
	cout << t2 << endl;
	cout << t2 - t1 << endl;
	//time_t t2=time(NULL);
	//cout << t2 << endl;
	//cout << t2-t1 << endl;

/*
	cout << "testIntentionPairWithPriceSource()" << endl;
	PriceSource::init();
	IntentionPair * ip = IntentionPair::create("J-rb","rb1201", "j1201", "j1201.bid-rb1201.ask");
	//ip->set("ru1201", "JRUZ1", "ru1201 . ask - JRUZ1 . bid");
    //ip->set("cu1112", "LCA3M", "cu1112 . bid - LCA3M . ask");
	//ip->set("au1112", "GCZ1", "au1112 . bid - GCZ1 . ask");
	//ip->set("TA201", "CLZ1", "TA201 . bid - CLZ1 . ask");
	//ip->set("rb1201", "j1201", "j1201 . bid - rb1201 . ask");
	Sleep(2000);
	for(int i = 0; i < 10; i++)
	{
		cout << ip->getFirstDiff() << ":" << ip->getSecondDiff() << ":";
	    cout << ip->getFirstAsk() << ":" << ip->getFirstBid() << ":";
		cout << ip->getSecondAsk() << ":" << ip->getSecondBid()<<endl;
		Sleep(500);
	}
*/
}

void testOrder()
{
	cout << "testOrder()" << endl;
	Order * order = Order::create("rb1201", BUY, KAI_CANG, 5, 4800.0);
	//cout << order->getId() << endl;
    order->addDetail(new OrderDetail(order->getId(), 1, 4800.0));
	assert(!order->isDealed());
    order->addDetail(new OrderDetail(order->getId(), 4, 4802.5));
    assert(order->isDealed());
    assert(doubleEqual(4802, order->avgDealPrice()));
	/*
	Order * o[5000];
	for(int i = 0;i<5000;i++)
	{
		o[i] = Order::create("rb1201", BUY, KAI_CANG, 5, 4800.0);
		assert(o[i]->getStatus() != FINISHED); 
	}
	*/
}

void testCancelOrder()
{
	cout << "testCancelOrder()" << endl;
	Order * order = Order::create("rb1201", BUY, KAI_CANG, 5, 4800.0);
    order->addDetail(new OrderDetail(order->getId(), 1, 4800.0));
    order->setCancel();
	assert(order->isCanceled());
	assert(!order->isDealed());

    order->manuallyFinish();
	assert(order->isCanceled());
    assert(order->isDealed());
    assert(doubleEqual(4800, order->avgDealPrice()));
}

void testCancelQueue()
{
	cout << "testCancelQueue()" << endl;
	Order * order = Order::create("rb1201", BUY, KAI_CANG, 5, 4800.0);

	CancelQueue * cq = CancelQueue::getInstance();
	cq->clear();
	cq->add(order);
	order->setCancel();
    cq->checkQueue();
	assert(cq->size() == 0);

    order = Order::create("rb1201", BUY, KAI_CANG, 5, 4800.0);
	cq->add(order);
	order->setRejected();
    cq->checkQueue();
	assert(cq->size() == 0);
}

void testCancelQueue2()
{
    cout << "testCancelQueue2()" << endl;

	DealHistory::getInstance()->clear();

    ShootStrategy * s = (ShootStrategy *)TwoLegStrategy::createShootStrategy("J-R-5");
	s->setLegA(new Leg("2639", SELL, KAI_CANG, "JRUU1", 1, 0));
    s->setLegB(new Leg("10110882", BUY, KAI_CANG, "ru1109", 1, 0));
	s->setCondition(new Condition("ru1109 . ask - JRUU1 . bid", ">=", 1000));
	s->setHoldingLimit(0, 5);

	Contract::get("JRUU1")->setPrice(400.0, 399.0);
    Contract::get("ru1109")->setPrice(33002.0, 33001.0);

	s->work();
    s->trig();

    Order * o = s->getFirstOrder();
	assert(o != NULL);
	assert(o->getContract().compare("JRUU1") == 0);
    
	Sleep(3000);
}

void testPersistence()
{
	cout << "testPersistence()" << endl;

	TwoLegStrategy::clear();
	TwoLegStrategy * ts = TwoLegStrategy::create("JRU-ru-2");
	ts->setLegA(new Leg("2639", SELL, KAI_CANG, "JRUU1", 1, 0));
    ts->setLegB(new Leg("10110882", BUY, KAI_CANG, "ru1109", 1, 0));
	ts->setCondition(new Condition("ru1109 . ask - JRUU1 . bid", ">=", 1000));
	ts->setHoldingLimit(0, 5);
	
	cout << ts->toString() << endl;
	assert(ts->toString().compare("JRU-ru-2 2639 SELL KC JRUU1 1 0 10110882 BUY KC ru1109 1 0 ru1109.ask-JRUU1.bid >= 1000.00 0 5 0") == 0);

	TwoLegStrategy * ts1 = TwoLegStrategy::create("JRU-ru-3");
	ts1->setLegA(new Leg("2639", SELL, KAI_CANG, "JRUU1", 1, 0));
    ts1->setLegB(new Leg("10110882", BUY, KAI_CANG, "ru1109", 1, 0));
	ts1->setCondition(new Condition("ru1109 . ask - JRUU1 . bid", ">=", 1000));
	ts1->setHoldingLimit(0, 5);
	ts1->setHoldingOperation(DECREASE);
	cout << ts1->toString() << endl;
	assert(ts1->toString().compare("JRU-ru-3 2639 SELL KC JRUU1 1 0 10110882 BUY KC ru1109 1 0 ru1109.ask-JRUU1.bid >= 1000.00 0 5 1") == 0);
	cout << ts1->toDesc() << endl;
	assert(ts1->toDesc().compare("[>=1000.00开仓抛JRUU1 1手 加0 开仓买ru1109 1手 加0]") == 0);

	assert(ts1->getLegA().getContract().compare("JRUU1") == 0);

	ts->save();

	TwoLegStrategy::clear();
	Persistence::loadTwoLegStrategy();

	assert(TwoLegStrategy::listAll().size() == 2);

	list<TwoLegStrategy*> lts = TwoLegStrategy::listAll();
	assert(lts.front()->toString().compare("JRU-ru-2 2639 SELL KC JRUU1 1 0 10110882 BUY KC ru1109 1 0 ru1109.ask-JRUU1.bid >= 1000.00 0 5 0") == 0);
	assert(lts.back()->toString().compare("JRU-ru-3 2639 SELL KC JRUU1 1 0 10110882 BUY KC ru1109 1 0 ru1109.ask-JRUU1.bid >= 1000.00 0 5 1") == 0);


}

void testUISenario()
{
	cout << "testUISenario()" << endl;

    IntentionPair * ip;
	ip = IntentionPair::create("j-rb", "rb1201", "j1201", "j1201.bid-rb1201.ask", 5, 1);
	assert(ip != NULL);

	IntentionPair::create("ru-JRU", "ru1201", "JRUZ1", "ru1201 . ask - JRUZ1 . bid", 1, 1);
    IntentionPair::create("cu-LCA", "cu1112", "LCA3M", "cu1112 . bid - LCA3M . ask", 4, 1);
	IntentionPair::create("au-GC", "au1112", "GCZ1", "au1112 . bid - GCZ1 . ask", 3 , 1);
	IntentionPair::create("TA-CL", "TA201", "CLZ1", "TA201 . bid - CLZ1 . ask", 1 , 1);

    assert(IntentionPair::listAll().size() >= 5);

    double firstDiff = ip->getFirstDiff();
	double secondDiff = ip->getSecondDiff();
	double firstBid = ip->getFirstBid();
	double firstAsk = ip->getFirstAsk();
	double secondBid = ip->getSecondBid();
	double secondAsk = ip->getSecondAsk();
    double firstRatio = ip->getFirstRatio();
	double secondRatio = ip->getSecondRatio();



	TwoLegStrategy * ts = TwoLegStrategy::create("JRU-ru-1");
	assert(ts != NULL);

	assert(TwoLegStrategy::listAll().size() >= 1);
	assert(TwoLegStrategy::getTwoLegStrategy("JRU-ru-1") != NULL);

	ts->setLegA(new Leg("2639", SELL, KAI_CANG, "JRUU1", 1, 0));
    ts->setLegB(new Leg("10110882", BUY, KAI_CANG, "ru1109", 1, 0));
	ts->setCondition(new Condition("ru1109 . ask - JRUU1 . bid", ">=", 1000));
	ts->setHoldingLimit(0, 5);
	ts->save();

	ts->start();

	if(ts->isSecondLegMissed()) {
		cout << "Second leg missed";
		ts->missedLegHandled();
	}

    if(ts->isStop()) cout << "Strategy is stopped";

	ts->stop();

	DealHistory::getInstance()->listAll();

}

void testDesmotic()
{
	cout << "testDesmotic()" << endl;
	Persistence::loadTwoLegStrategy();
	assert(TwoLegStrategy::listAll().size() == 1);
	TwoLegStrategy * ts = TwoLegStrategy::listAll().front();
	ts->start();
	Sleep(10 * 1000);
}

void testDayTradingStrategy()
{
	cout << "testDayTradingStrategy()" << endl;
	assert(DayTradingStrategy::listAll().size() == 0);
	DayTradingStrategy * dts = DayTradingStrategy::create("JRU-ru-1");
	TwoLegStrategy * tlsA = dts->createStrategyA();
	//cout << tlsA->getId() << endl;
	assert(tlsA->getId().compare("JRU-ru-1-A") == 0);
	tlsA->setLegA(new Leg("2639", SELL, KAI_CANG, "JRUU1", 1, 0));
    tlsA->setLegB(new Leg("10110882", BUY, KAI_CANG, "ru1109", 1, 0));
	tlsA->setCondition(new Condition("ru1109 . ask - JRUU1 . bid", ">=", 1000));
	dts->setHoldingLimit(0, 5);
	assert(tlsA->getHighHoldingLimit() == 5);
    assert(tlsA->getLowHoldingLimit() == 0);
    tlsA->setHoldingOperation(INCREASE);
	assert(dts->toString().compare("JRU-ru-1 JRU-ru-1-A NULL") == 0);
	assert(dts->toDesc().compare("套数5 [>=1000.00开仓抛JRUU1 1手 加0 开仓买ru1109 1手 加0] [NULL] 成交套数:0") == 0);

	TwoLegStrategy * tlsB = dts->createStrategyB();
	assert(tlsB->getId().compare("JRU-ru-1-B") == 0);

	tlsB->setLegA(new Leg("2639", BUY, PING_JIN, "JRUU1", 1, 0));
    tlsB->setLegB(new Leg("10110882", SELL, PING_JIN, "ru1109", 1, 0));
	tlsB->setCondition(new Condition("ru1109 . ask - JRUU1 . bid", "<=", 700));
	assert(tlsB->getHighHoldingLimit() == 5);
    assert(tlsB->getLowHoldingLimit() == 0);
    tlsB->setHoldingOperation(DECREASE);

	assert(dts->toString().compare("JRU-ru-1 JRU-ru-1-A JRU-ru-1-B") == 0);
	assert(dts->toDesc().compare("套数5 [>=1000.00开仓抛JRUU1 1手 加0 开仓买ru1109 1手 加0] [<=700.00平今买JRUU1 1手 加0 平今抛ru1109 1手 加0] 成交套数:0") == 0);

	dts->save();

	Persistence::load();
	assert(DayTradingStrategy::listAll().size() == 1);

	dts = DayTradingStrategy::listAll().front();
	//dts->start();
	dts->remove();

}

time_t makeTime(int hour, int minute, int second) {

	struct tm  when;
    time_t now;


	time( &now );
	localtime_s( &when, &now );

	when.tm_hour = hour;
	when.tm_min = minute;
	when.tm_sec = second;

	return mktime(&when);
}

void testTimeControl(TwoLegStrategy * s)
{
	s->setLegA(new Leg("2639", SELL, KAI_CANG, "JRUU1", 1, 0));
    s->setLegB(new Leg("10110882", BUY, KAI_CANG, "ru1109", 1, 0));
	s->setCondition(new Condition("ru1109 . ask - JRUU1 . bid", ">=", 1000));
	s->setHoldingLimit(0, 1);

	assert(!s->inTrading(makeTime(9, 0, TIME_OFFSET)));
    assert(s->inTrading(makeTime(9, 0, TIME_OFFSET + 1)));
	assert(s->inTrading(makeTime(10, 14, 60 - TIME_OFFSET - 1)));
    assert(!s->inTrading(makeTime(10, 14, 60 - TIME_OFFSET)));
    assert(!s->inTrading(makeTime(10, 30, TIME_OFFSET)));
    assert(s->inTrading(makeTime(10, 30, TIME_OFFSET + 1)));
    assert(s->inTrading(makeTime(11, 29, 60 - TIME_OFFSET - 1)));
    assert(!s->inTrading(makeTime(11, 29, 60 - TIME_OFFSET)));
    assert(!s->inTrading(makeTime(13, 30, TIME_OFFSET)));
    assert(s->inTrading(makeTime(13, 30, TIME_OFFSET + 1)));
    assert(s->inTrading(makeTime(14, 29, 60 - TIME_OFFSET - 1)));
    assert(!s->inTrading(makeTime(14, 29, 60 - TIME_OFFSET)));


	Contract::get("JRUU1")->setPrice(400.0, 399.0);
    Contract::get("ru1109")->setPrice(33002.0, 33001.0);
	
	time_t stopTime = makeTime(9, 0, TIME_OFFSET);
	time_t workTime = makeTime(9, 0, TIME_OFFSET + 1);

	s->work();
	s->trig(workTime);	
	Order * o = s->getFirstOrder();
	assert(o != NULL);
	o->addDetail(new OrderDetail(o->getId(), o->getLot(), o->getPrice()));

	s->trig(stopTime);
	o = s->getSecondOrder();
	assert(o != NULL);
	o->addDetail(new OrderDetail(o->getId(), o->getLot(), o->getPrice()));
	
	s->trig(stopTime);

    s->trig(stopTime);
	assert(s->isHalt());

	// halt work transform
	s->trig(workTime);
	s->trig(stopTime);

	assert(s->isHalt());
	
	s->trig(workTime);

	assert(s->isWork());
}


void testShootStrategyTimeControl()
{
	cout << "testShootStrategyTimeControl()" << endl;
	ShootStrategy * s = (ShootStrategy *)TwoLegStrategy::createShootStrategy("J-R-3");
	testTimeControl(s);
}

void testFishStrategyTimeControl()
{
	cout << "testFishStrategyTimeControl()" << endl;
	FishStrategy * s = (FishStrategy *)TwoLegStrategy::createFishStrategy("J-R-4");
	testTimeControl(s);
}

void testTimeControl2()
{
	cout << "testTimeControl2()" << endl;
	TimeInterval *ti = new TimeInterval(9, 0, 0, 10, 15, 0);
	time_t t = makeTime(10, 0, 0);
	assert(ti->include(t));

    t += ONE_DAY;
    assert(ti->include(t));
}

void testExchange()
{
	cout << "testExchange()" << endl;
	Exchange * e = Exchange::getInstance(TOCOM);
	assert(e != NULL);
	assert(!e->inTrading(makeTime(8, 0, 2)));
	assert(e->inTrading(makeTime(8, 0, 3)));
	assert(e->inTrading(makeTime(14, 29, 57)));
	assert(!e->inTrading(makeTime(14, 29, 58)));

}

void testCorrectConditon()
{
	cout << "testCorrectConditon()" << endl;
	ShootStrategy * s = (ShootStrategy *)TwoLegStrategy::createShootStrategy("J-R-4");
	s->setLegA(new Leg("2639", SELL, KAI_CANG, "JRUU1", 1, 0));
    s->setLegB(new Leg("10110882", BUY, KAI_CANG, "ru1109", 1, 0));
	s->setCondition(new Condition("ru1109 . bid - JRUU1 . ask", ">=", 1000));
	s->setHoldingLimit(0, 5);
	s->save();
	cout << s->getCondition().toString() <<endl;
	assert(s->getCondition().toString().compare("ru1109.ask-JRUU1.bid >= 1000.00 ") == 0);

	s = (ShootStrategy *)TwoLegStrategy::createShootStrategy("cu1112-cu1201-1");
	s->setLegA(new Leg("10110882", SELL, KAI_CANG, "cu1112", 1, 0));
    s->setLegB(new Leg("10110882", BUY, KAI_CANG, "cu1201", 1, 0));
	s->setCondition(new Condition("cu1112 . ask - cu1201 . bid", ">=", 300));
	s->setHoldingLimit(0, 5);
	s->save();
	cout << s->getCondition().toString() <<endl;
	assert(s->getCondition().toString().compare("cu1112.bid-cu1201.ask >= 300.00 ") == 0);
}

void testOrderCountControl()
{
	cout << "testOrderCountControl()" << endl;

	Contract * c = Contract::get("ru1109");
	c->setOrderCount(449);
	assert(c->allowOrder());
	c->increaseOrderCount();
    assert(!c->allowOrder());
	c->setOrderCount(0);

    c = Contract::get("JRUU1");
	c->setOrderCount(449);
	assert(c->allowOrder());
	c->increaseOrderCount();
    assert(c->allowOrder());
	c->setOrderCount(0);
}

void testOrderCountControl2()
{
	cout << "testOrderCountControl2()" << endl;
	Contract::get("JRUU1")->setPrice(400.0, 399.0);
    Contract::get("ru1109")->setPrice(33002.0, 33001.0);


	ShootStrategy * s = (ShootStrategy *)TwoLegStrategy::createShootStrategy("J-R-5");
	s->setLegA(new Leg("2639", SELL, KAI_CANG, "JRUU1", 1, 0));
    s->setLegB(new Leg("10110882", BUY, KAI_CANG, "ru1109", 1, 0));
	s->setCondition(new Condition("ru1109 . bid - JRUU1 . ask", ">=", 1000));
	s->setHoldingLimit(0, 500);

	s->work();
	for(int i = 1; i <= 450; i++)
	{
		//cout << i <<endl;
		s->trig();
		Order * o = s->getFirstOrder();
	    assert(o != NULL);
	    o->addDetail(new OrderDetail(o->getId(), o->getLot(), o->getPrice()));
		s->trig();
		o = s->getSecondOrder();
	    assert(o != NULL);
        o->addDetail(new OrderDetail(o->getId(), o->getLot(), o->getPrice()));
        s->trig();
		assert(s->getHoldingCount() == i);
	}

		s->trig();
		Order * o = s->getFirstOrder();
	    assert(o == NULL);

}

void testFishShoot()
{
	cout << "testFishShoot()" << endl;

    TwoLegStrategy * s = (TwoLegStrategy *)TwoLegStrategy::create("J-R-8");
	s->setLegA(new Leg("2639", SELL, KAI_CANG, "JRUU1", 1, 0));
    s->setLegB(new Leg("10110882", BUY, KAI_CANG, "ru1109", 1, 0));
	s->setCondition(new Condition("ru1109 . ask - JRUU1 . bid", ">=", 1000));
	s->setHoldingLimit(0, 5);
	s->setFish(false);

	s->start();
	s->stop();


	s->setFish(true);
	s->start();
	s->stop();

}

void testLoadOrderCount()
{
	cout << "testLoadOrderCount()" << endl;
	Contract::get("JRUU1")->setOrderCount(0);
    FishStrategy * s = (FishStrategy *)TwoLegStrategy::createFishStrategy("J-R-7");

	s->setLegA(new Leg("2639", SELL, KAI_CANG, "JRUU1", 1, 0));
    s->setLegB(new Leg("10110882", BUY, KAI_CANG, "ru1109", 1, 5));
	s->setCondition(new Condition("ru1109 . ask - JRUU1 . bid", "<=", 2500));
	s->setHoldingLimit(0, 5);

	Contract::get("JRUU1")->setPrice(400.0, 399.0);
    Contract::get("ru1109")->setPrice(33002.0, 33001.0);

	s->work();
    s->trig();
	Order * o = s->getFirstOrder();
	assert(o != NULL);

	Contract::get("JRUU1")->setPrice(300, 299.0);
    Contract::get("ru1109")->setPrice(33002.0, 33001.0);
	s->trig();
	o->setCancel();

	assert(Contract::get("JRUU1")->getOrderCount() == 1);
	Contract::get("JRUU1")->setOrderCount(0);
	Persistence::load();

	cout << Contract::get("JRUU1")->getOrderCount() << endl; 
	assert(Contract::get("JRUU1")->getOrderCount() == 2);
}

void testKnownContract()
{
	cout << "testKnownContract()" << endl;
	Persistence::load();
	//cout << KnownContract::getFixCommodityFor("ZRF3") << endl;
	assert(KnownContract::getFixCommodityFor("ZRF3").compare("ZR") == 0);
	assert(KnownContract::getFixYearMonthFor("ZRF3").compare("201301") == 0);
	assert(KnownContract::getFixExchangeFor("ZRF3").compare("CME_CBOT") == 0);
	//cout << KnownContract::getRonhonNameFor("ZRF3") << endl;
	assert(KnownContract::getRonhonNameFor("ZRF3").compare("CME_CBT-RICE-JAN13") == 0);

	assert(KnownContract::getFixCommodityFor("SN3M").compare("SN") == 0);
	assert(KnownContract::getFixYearMonthFor("SN3M").compare("3M") == 0);
	assert(KnownContract::getFixExchangeFor("SN3M").compare("LME") == 0);
	assert(KnownContract::getRonhonNameFor("SN3M").compare("LME-SN-3M") == 0);
	assert(KnownContract::getRonhonNameFor("GCZ2").compare("CME CMX GLD DEC12") == 0);
}

void functionTest()
{
	cout << "here" << endl;
    Dealer::useRealDealer = false;
	PriceSource::useDumbPriceSource();
	cout << "here" << endl;
	list<IntentionPair *> listIntenPair = IntentionPair::listAll();
	cout << "here" << endl;

	testImportExpression();
	testPrice();
	testCommodity();
	testArbitrageComparablePriceExpression();
	testHoldingControlForDayTradingStrategy();
	testContract();
	testFixRonhonName();


	testCondition();
	//testTrigPeriodly();
	testStrategy();
    testLegShoot();

	//testIntentionPairWithPriceSource();
	testOrder();
    testCancelOrder();
	testCancelQueue();


    testDiffExperssion();

	//testUISenario();

	//testIntentionPairWithPriceSource();

	testIntentions();
	testIntentionPair();
	testIntentionPairOrder();
	

	testPersistence();
	//testDesmotic();


	testDayTradingStrategy();

	testExchange();
	
	testShootStrategyTimeControl();
	testFishStrategyTimeControl();
	testTimeControl2();

	testCorrectConditon();
	testOrderCountControl();
    //testOrderCountControl2();

    testFishShoot();
	
	//testShootStrategyKaiCang(); //Only work in JRU trading time
	//testFishStrategyKaiCang();
	//testFishStrategyKaiCang2();
	//testFishStrategyKaiCang3();
	//testLoadOrderCount();

	//testCancelQueue2();

	testKnownContract();

}

void  Function( void* pParams )
{
	while(1)
	{
		Commodity::findFor("rb1201");
		Commodity::findFor("j1201");
		Commodity::findFor("rb1205");
		Commodity::findFor("j1205");
		Sleep(50);
	}
}

void testThread()
{
	cout << "testThread()" << endl;

	for(int i = 1; i <=50; i++)
	{
		_beginthread(Function, 0, NULL);
	}
	getchar();
}

void testWithNOUI()
{
	Dealer::useRealDealer = true;
	PriceSource::useRealPriceSource();
	list<IntentionPair *> listIntenPair = IntentionPair::listAll();
	list<DayTradingStrategy*> lts = DayTradingStrategy::listAll();
	list<DayTradingStrategy*> ::iterator itt ;
	for(itt = lts.begin(); itt != lts.end(); ++itt)// list
	{
		(*itt)->start();
	}


	getchar();
}

void testPatsApi()
{ // TODO: ZHOUJIE
	cout<< "testPatsApiContract()" << endl;
	Persistence::loadKnownContract();

	Contract * c2 = Contract::get("SBH3");
	assert(c2->getPatsApiName().compare("SUGAR11-MAR13@NYBOT") == 0);
}

int main(int argc, char* argv[])
{
	//
		//testIntentions();
	cout << "run" << endl; 
	//functionTest();
	//testWithNOUI();

    //testThread();
	testPatsApi();
	return 0;
}

