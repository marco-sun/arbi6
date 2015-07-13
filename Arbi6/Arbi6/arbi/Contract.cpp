// Contract.cpp: implementation of the Contract class.
//
//////////////////////////////////////////////////////////////////////

#include "Contract.h"
#include <assert.h>
#include "PriceSource.h"
#include "../LogStore.h"
#include "../utils/Utils.h"
#include "KnownContract.h"

#define ORDER_COUNT_LIMIT 450

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ofstream Contract::writeFile;


Contract::Contract(string spName)
{
	memset(debug,0,16);
	sprintf_s(debug,16,"ContractObject");
    this->spName = spName;
	InitializeCriticalSection(&cs);
	this->commodity = Commodity::findFor(spName);
	//TRACE_LOG("commodity findfor ok");
	orderCount = 0;
	if(this->isDomestic()) {
		TRACE_LOG("enter isDomestic part");
		fixName = "";
	    ronhonName = "";
		patsapiName = "";
	} else {
		//TRACE_LOG("begin getRonhonNameFor");
	    fixName = KnownContract::getFixNameFor(spName);
	    ronhonName = KnownContract::getRonhonNameFor(spName);
		patsapiName =  KnownContract::getPatsApiNameFor(spName);
		TRACE_LOG("spName = %s", spName.c_str());
		TRACE_LOG("patsapiName = %s", patsapiName.c_str());
	}
}

Contract::~Contract()
{

}

list<Contract *> Contract::contracts;

Contract * Contract::get(string name)
{
	if(findInList(name) == NULL)
	{
		contracts.push_back(new Contract(name));
		PriceSource::listen(name);
	}

	Contract * c = findInList(name);

	assert( c != NULL );
	return c;
}

bool Contract::match(string name)
{
	return spName.compare(name) == 0
		   || fixName.compare(name) == 0
		   || ronhonName.compare(name) == 0
		   || patsapiName.compare(name) == 0;
}

Contract * Contract::findInList(string name)
{
    list<Contract *>::iterator the_iterator = contracts.begin();

    while( the_iterator != contracts.end() ) {
        if((*the_iterator)->match(name)) return (*the_iterator);
        the_iterator++;
	}

	return NULL;

}

//TODO:: remove this
Contract * Contract::get(string comodityName, string exchangeName, string CCYYMM)
{
	string s;
	s = comodityName + CCYYMM + "@" + exchangeName;
	return get(s);
}

string Contract::getComodityName()
{
	string cn = commodity->getName();
	TRACE_LOG("commodity->getName() %s", cn.c_str());
	return cn;
}

string Contract::getExchangeName()
{
	return "";
	//return getNameOf(MARKET);
}

string Contract::getCCYYMM()
{
	return "";
	//return getNameOf(MONTH);
}

Price Contract::getPrice()
{
	if(!this->inTrading(time(NULL)) && sugar()) return sugarPrice();

	EnterCriticalSection(&cs);
	Price p;
	p=price;
	//TRACE_LOG("%s:%f",this->name.c_str(),p.getAsk());
	//TRACE_LOG("The time is %d",time(NULL));
	LeaveCriticalSection(&cs);
	return p;
}

bool startWith(const char * s1, const char * s2);

Price Contract::sugarPrice()
{
	if (spName.compare("SBH4") == 0)
		return Price::SBH4;
	else if (spName.compare("SBN4") == 0)
		return Price::SBN4;
	else // (spName.compare("SBK4") == 0)
		return Price::SBK4;

}

bool Contract::sugar()
{
	return startWith(spName.c_str(), "SB");
}


void Contract::setPrice(time_t t, double ask, double bid)
{
	EnterCriticalSection(&cs);
	price.set(t, ask, bid, 0);
	TRACE_LOG("The time is %ld",time(NULL));
    LeaveCriticalSection(&cs);
}

void Contract::setPrice(time_t t, double ask, double bid, double lastPrice)
{
	EnterCriticalSection(&cs);
	price.set(t, ask, bid, lastPrice);
	//TRACE_LOG("Ask bid lastprice are: %f  %f  %f",ask,bid,lastPrice);
    LeaveCriticalSection(&cs);
}


void Contract::setPrice(double ask, double bid)
{
    setPrice(0, ask, bid);
}

bool Contract::isDomestic()
{
	return getCommodity()->isDomestic();
}

bool Contract::isOversea()
{
	return getCommodity()->isOversea();
}

bool Contract::isNearThan(Contract * c)
{
	return this->spName.compare(c->spName) < 0;
}

Commodity * Contract::getCommodity()
{
    return this->commodity;
    //return Commodity::findFor(name);
}

double Contract::getArbitrageComparablePriceFor(string askBid, string comparedContract)
{
	Price p = getPrice();
	Commodity * c = getCommodity();
	Commodity * comparedCommodity = Commodity::findFor(comparedContract);
	TRACE_LOG("getArbitrageComparablePriceFor %s" , spName.c_str());
    TRACE_LOG("getArbitrageComparablePriceFor %s" , comparedContract.c_str());
	assert(c != NULL);
	assert(comparedCommodity != NULL);
    return c->calcArbitrageComparablePriceFor(p.get(askBid), comparedCommodity);
}

string Contract::getName()
{
	return spName;
}

string Contract::getFixName()
{
	return fixName;
}

string Contract::getRonhonName()
{
	return ronhonName;
}

string Contract::getPatsApiName()
{
	return patsapiName;
}

bool Contract::hasValidPrice()
{
	return getPrice().isValid();
}

bool Contract::inTrading(time_t t)
{
	Commodity * c = getCommodity();
	return c->inTrading(t);
}

void Contract::setOrderCount(int count)
{
	//TODO: add lock
	this->orderCount = count;
}

bool Contract::allowOrder()
{
	//TODO: add lock
	bool ret = beyondOrderCountLimit() ? false : true;
	return ret;
}

void Contract::increaseOrderCount()
{
	//TODO: add lock
	TRACE_LOG("%s increaseOrderCount",spName.c_str());
	Contract::writeToFile(this->spName);
	this->orderCount++;
	if (orderCount % 50 == 0)
	{
		TRACE_LOG("Contract: %s orderCount = %d!",this->getName().c_str(),orderCount);
	}
	if(this->beyondOrderCountLimit())
	{
		TRACE_LOG("%s Beyond order count limit.", this->spName.c_str());
	}

}

void Contract::increaseOrderCount(int i)
{
	this->orderCount += i;
}

bool Contract::beyondOrderCountLimit()
{
	return this->isDomestic() && orderCount >= ORDER_COUNT_LIMIT;
}

string Contract::getDateFileName()
{
	time_t t = time(NULL);
	struct tm tmp;
	int errorNo = localtime_s(&tmp, &t);
    return "orderCount-"  + Utils::intToString(tmp.tm_year + 1900) + Utils::intToString(tmp.tm_mon + 1) + Utils::intToString(tmp.tm_mday) + ".dat";
}

void Contract::openFile()
{
	writeFile.open(getDateFileName(),ios::app);
	if(writeFile.fail()) {
		TRACE_LOG("open %s fail" ,getDateFileName().c_str());
		return;
	}
	TRACE_LOG("open %s successful" ,getDateFileName().c_str());
}

void Contract::writeToFile(string name)
{
	writeFile << name << endl;
	TRACE_LOG("%s writeToFile",name.c_str());
}

void Contract::loadOrderCount()
{
	TRACE_LOG("loadOrderCount.");
	ifstream file(getDateFileName());
    if(file.fail()) {
		TRACE_LOG("Read %s fail",getDateFileName().c_str());
		openFile();
		return;
	}

	string contractName;
	while(!file.eof()) 
	{
		file >> contractName;
		if (contractName.length() > 1)
		{
			Contract * c = Contract::get(contractName);
			c->increaseOrderCount(1);
		}
	}
	file.close();
	openFile();
}

int Contract::getOrderCount()
{
	return orderCount;
}
/*
string Contract::getNameOf(NAME_OF n)
{
	assert(nameOf[n].compare("") != 0);
	return nameOf[n];
}

void Contract::parseName()
{
	initNameOf();
	if (!isFixFormat()) return;
	nameOf[COMMODITY] = spName.substr(0,2);
	nameOf[MONTH] = spName.substr(2, 6);
	nameOf[MARKET] = spName.substr(9,3);
}

void Contract::initNameOf()
{
	nameOf[COMMODITY] = "";
	nameOf[MONTH] = "";
	nameOf[MARKET] = "";
}
*/
char Contract::getExchangeNumberForYiSheng()
{
	return  this->commodity->getExchangeNumberForYiSheng();
}



