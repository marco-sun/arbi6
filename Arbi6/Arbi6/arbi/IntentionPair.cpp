// IntentionPair.cpp: implementation of the IntentionPair class.
//
//////////////////////////////////////////////////////////////////////

#include "IntentionPair.h"
#include "Persistence.h"
#include "ImportExpression.h"
#include "../LogStore.h"
#include "../utils/Utils.h"
#include <assert.h>
#include <process.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void writeIntentionPairData(void* pParams)
{
	IntentionPair *s=(IntentionPair *) pParams;
	while(1)
	{
		s->writeToFile();
		//Sleep(1 * 50);
		Sleep(1 * 1000);
	}
}


IntentionPair::IntentionPair(string name, string contractA, string contractB, string diffExp, int lotA, int lotB,  double refParaOne, double refParaTwo, enum IntentionOrder orderBy)
{
	this->name = name;
	dataFile.open(getDateFileName(),ios::app);
	if(dataFile.fail()) {
		TRACE_LOG("open %s fail" ,getDateFileName().c_str());
		exit(1);
	}
	TRACE_LOG("open %s successful" ,getDateFileName().c_str());
	set(contractA, contractB, diffExp, lotA, lotB, refParaOne, refParaTwo, orderBy);
	_beginthread(writeIntentionPairData, 0, (void *)this);
}

string IntentionPair::getDateFileName()
{
	time_t t = time(NULL);
	struct tm tmp;
	int errorNo = localtime_s(&tmp, &t);
    return name + "-" + Utils::intToString(tmp.tm_year + 1900) + Utils::intToString(tmp.tm_mon + 1) + Utils::intToString(tmp.tm_mday) + ".dat";
}

IntentionPair::~IntentionPair()
{

}

bool IntentionPair::exist(string name)
{
    list<IntentionPair *>::iterator the_iterator = intentionPairs.begin();

    while( the_iterator != intentionPairs.end() ) {
        if((*the_iterator)->name.compare(name) == 0) return true;
        the_iterator++;
	}

	return false;
}

IntentionPair * IntentionPair::create(string name, string contractA, string contractB, string diffExp, int lotA, int lotB)
{
	/*
    IntentionPair * ip = add(name, contractA, contractB, diffExp, lotA, lotB);
	Persistence::saveIntentions();
	return ip;
	*/
	return create(name, contractA, contractB, diffExp, lotA, lotB, 0, 0, ONE);
}

IntentionPair * IntentionPair::create(string name, string contractA, string contractB, string diffExp, int lotA, int lotB, double refParaOne, double refParaTwo, enum IntentionOrder orderBy)
{
    IntentionPair * ip = add(name, contractA, contractB, diffExp, lotA, lotB, refParaOne, refParaTwo, orderBy);
	Persistence::saveIntentions();
	return ip;
}

IntentionPair * IntentionPair::add(string name, string contractA, string contractB, string diffExp, int lotA, int lotB)
{
	return add(name, contractA, contractB, diffExp, lotA, lotB, 0, 0, ONE);
}


IntentionPair * IntentionPair::add(string name, string contractA, string contractB, string diffExp, int lotA, int lotB, double refParaOne, double refParaTwo, enum IntentionOrder orderBy)
{
	if(exist(name)) return NULL;

	IntentionPair * ip =new IntentionPair(name, contractA, contractB, diffExp, lotA, lotB, refParaOne, refParaTwo, orderBy);
	intentionPairs.push_back(ip);
	return ip;
}

IntentionPair * IntentionPair::get(string name)
{

	IntentionPair * ip = NULL;

    list<IntentionPair *>::iterator the_iterator = intentionPairs.begin();

    while( the_iterator != intentionPairs.end() ) {
        if((*the_iterator)->name.compare(name) == 0) ip = (*the_iterator);
        the_iterator++;
	}

	return ip;
}

bool startWith(const char * s1, const char * s2);

IntentionPair * IntentionPair::findFor(string strategyId)
{
	IntentionPair * ip = NULL;

    list<IntentionPair *>::iterator the_iterator = intentionPairs.begin();

    while( the_iterator != intentionPairs.end() ) {
        if(startWith(strategyId.c_str(), (*the_iterator)->name.c_str())) ip = (*the_iterator);
        the_iterator++;
	}

	return ip;
}

void IntentionPair::clear()
{
	intentionPairs.clear();
}

void IntentionPair::set(string contractA, string contractB, string firstDiffExp, int lotA, int lotB, double refParaOne, double refParaTwo, enum IntentionOrder orderBy)
{
	this->contractA = Contract::get(contractA);
	this->contractB = Contract::get(contractB);
	diffExp = new DiffExpression(firstDiffExp);
	this->lotA = lotA;
	this->lotB = lotB;
	this->refParaOne = refParaOne;
	this->refParaTwo = refParaTwo;
	this->orderBy = orderBy;
}

Contract * IntentionPair::getContract(string leg)
{
	assert(leg == "contractA" || leg == "contractB");
	if(leg == "contractA") return contractA;
	return contractB;
}

double IntentionPair::getFirstDiff()
{   
	return diffExp->getValue();
}

double IntentionPair::getSecondDiff()
{
    return diffExp->getReverseValue();
}

double IntentionPair::getlastPriceDiff()
{
	return diffExp->getLastValue();
}

double IntentionPair::getFirstBid()
{
	return contractA->getPrice().getBid();
}

double IntentionPair::getFirstAsk()
{
	return contractA->getPrice().getAsk();
}

double IntentionPair::getSecondBid()
{
	return contractB->getPrice().getBid();
}

double IntentionPair::getSecondAsk()
{
	return contractB->getPrice().getAsk();
}

double IntentionPair::getFirstRatio()
{
	return getFirstAsk() / getSecondBid();
}

double IntentionPair::getSecondRatio()
{
	return getFirstBid() / getSecondAsk();
}

string IntentionPair::toString()
{
	return name + " "
		   + contractA->getName() + " " 
		   + contractB->getName() + " " 
		   + diffExp->toString() + " "
		   + Utils::intToString(lotA) + " "
		   + Utils::intToString(lotB);
}

list<IntentionPair *> IntentionPair::intentionPairs;

list<IntentionPair *> IntentionPair::listAll()
{
	Persistence::loadAsNecessary();
	return intentionPairs;
}

list<IntentionPair *> IntentionPair::sortListAll(enum Sequence SortBy)
{
	//TRACE_LOG("IntentionPair sortListAll %d", SortBy);
	list<IntentionPair *>  sortIntentionPair = intentionPairs;	
	list<IntentionPair *>::iterator the_iterator = sortIntentionPair.begin();
    while( the_iterator != sortIntentionPair.end() ) {
		(*the_iterator)->sortValue();
        the_iterator++;
	}
	if (SortBy == UP)
		sortIntentionPair.sort(greater<IntentionPair *>());
	else
		sortIntentionPair.sort(less<IntentionPair *>());
	//TRACE_LOG("sortIntentionPair first: %s", sortIntentionPair.front()->getContractA().c_str());
	return sortIntentionPair;
	//TODO:sortIntentionPair.sort()
}

string IntentionPair::getName()
{
	return name;
}

string IntentionPair::getContractA()
{
	return contractA->getName();
}

string IntentionPair::getContractB()
{
	return contractB->getName();
}

int IntentionPair::getLotA()
{
	return this->lotA;
}
	
int IntentionPair::getLotB()
{
	return this->lotB;
}

double IntentionPair::importProfit()
{
	Persistence::loadAsNecessary();
	if(startWith(contractA->getName().c_str(), "m"))
	{
		ImportExpression * e = ImportExpression::find2("ZS");
		if(e == NULL)
			return 0;
		double a;
		a = Contract::get(e->getCommodity())->getPrice().getAsk();
		a = ((((a + 50) * 0.3674) + 60) * 6.4 * (1 + 0.03) * (1 + 0.13) * (1 + 0) + 120);
		return e->calc(contractA->getPrice().getAsk(), contractB->getPrice().getAsk(), a);
	}
	ImportExpression * e = ImportExpression::find(contractB->getName());
	if (e == NULL)
		return 0;
	return e->calc(contractB->getPrice().getAsk(), contractA->getPrice().getBid());
}

double IntentionPair::sortValue()
{
	this->importValue = (importProfit() - refParaOne) / (contractA->getPrice().getBid());
	return importValue;
}

void IntentionPair::writeToFile()
{
	if (getFirstDiff() < -1000000)
		return;
	struct tm local;
	time_t t;
	t = time(NULL);
	int errorNo = localtime_s(&local, &t);
	dataFile << local.tm_hour << " " << local.tm_min << " " << local.tm_sec << " " 
		     << getFirstDiff() << " "
             << getSecondDiff() << " "
			 << contractA->getPrice().getLastPrice() << " "
			 << contractB->getPrice().getLastPrice() << endl;
}



