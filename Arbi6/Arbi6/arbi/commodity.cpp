// commodity.cpp: implementation of the Commodity class.
//
//////////////////////////////////////////////////////////////////////

#include "commodity.h"
#include "ArbitrageComparablePriceExpression.h"
#include "../LogStore.h"
#include <assert.h>
#include <fstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



Commodity::Commodity()
{
	memset(debug,0,16);
	sprintf_s(debug,16,"Commodity_");
}

Commodity::	Commodity(string name, 
		      enum Market market,
			  double increment)
{
	memset(debug,0,16);
	sprintf_s(debug,16,"Commodity");
	this->name = name;
	this->market = market;
	this->increment = increment;
	this->exchange = Exchange::getInstance(market);
	assert(exchange != NULL);
}

Commodity::~Commodity()
{

}

bool Commodity::inited = false;

list<Commodity*> Commodity::commodities;


void Commodity::initAsNeccissary()
{
	if(inited) return;
	TRACE_LOG("Commodity::initAsNeccissary");
	//inspect why list's head object was flushed.

	//p = new char[1]; //add heap protection.

	//TODO: add all commodities
	addCommodity("IF", ZHONGJIN, 0.2);
	ArbitrageComparablePriceExpression::add("IF", "510300", "P * 1 * 1 + 0");
	addCommodity("IH", ZHONGJIN, 0.2);
	ArbitrageComparablePriceExpression::add("IH", "510050", "P * 1 * 1 + 0");
	addCommodity("IC", ZHONGJIN, 0.2);
	ArbitrageComparablePriceExpression::add("IC", "510500", "P * 1 * 1 + 0");
	//ArbitrageComparablePriceExpression::add("IF", "rb", "P * 300 * 1 + 0");
	//addCommodity("TF", ZHONGJIN, 0.01);
	addCommodity("TF", ZHONGJIN, 0.002);

    addCommodity("rb", SHANG_HAI, 1.0); //���Ƹ� RB 10��/�� 1Ԫ/��
	ArbitrageComparablePriceExpression::add("rb", "j", "P * 40 * 1 + 0");
	ArbitrageComparablePriceExpression::add("rb", "i", "P * 10 * 5 + 0");
	ArbitrageComparablePriceExpression::add("rb", "ru", "P * 6 * 1 + 0");
	ArbitrageComparablePriceExpression::add("rb", "ag", "P * 3 * 10 + 0");
	ArbitrageComparablePriceExpression::add("rb", "SI", "P * 150 * 1 + 0");
	ArbitrageComparablePriceExpression::add("rb", "FG", "P * 10 * 5 + 0");
	ArbitrageComparablePriceExpression::add("rb", "bb", "P * 2 * 10 + 0");
	ArbitrageComparablePriceExpression::add("rb", "510500", "P * 1 * 1 + 0");
	ArbitrageComparablePriceExpression::add("rb", "510300", "P * 1 * 1 + 0");
	ArbitrageComparablePriceExpression::add("rb", "510050", "P * 1 * 1 + 0");

	addCommodity("pb", SHANG_HAI, 5.0); //Ǧ PB 10��/�� 5Ԫ/��
	//ArbitrageComparablePriceExpression::add("rb", "j", "P * 50 * 1 + 0");

    addCommodity("ru", SHANG_HAI, 5.0); //�� RU 5��/�� 5Ԫ/��
	ArbitrageComparablePriceExpression::add("ru", "rb", "P * 1 * 1 + 0");
	ArbitrageComparablePriceExpression::add("ru", "bb", "P * 10 * 1 + 0");

    //addCommodity("wr", SHANG_HAI, 1.0); //�߲� WR 10��/�� 1Ԫ/��
    addCommodity("cu", SHANG_HAI, 10.0); //����ͭ CU 5��/�� 10Ԫ/��
	ArbitrageComparablePriceExpression::add("cu", "LCA", "P * 20 * 1 + 0");
	ArbitrageComparablePriceExpression::add("cu", "HG", "P * 20 * 1 + 0");
	ArbitrageComparablePriceExpression::add("cu", "bb", "P * 5 * 1 + 0");

	addCommodity("bu", SHANG_HAI, 2.0); //���� bu 10��/�� 2Ԫ/��

    addCommodity("al", SHANG_HAI, 5.0); //�� AL 5��/�� 5Ԫ/��
    ArbitrageComparablePriceExpression::add("al", "LAH", "P * 20 * 1 + 0");
	//ArbitrageComparablePriceExpression::add("al", "LAH", "P * 25 * 1 + 0");
	//ArbitrageComparablePriceExpression::add("al", "LAH", "P * 25 * 1 + 0");

    addCommodity("zn", SHANG_HAI, 5.0); //п ZN 5��/�� 5Ԫ/��
    ArbitrageComparablePriceExpression::add("zn", "LZS", "P * 20 * 1 + 0");

    addCommodity("au", SHANG_HAI, 0.05); //�ƽ� AU 1000��/�� 0.01Ԫ/��

	addCommodity("ag", SHANG_HAI, 1.0); //���� ag 15ǧ��/��  1Ԫ/ǧ��
	ArbitrageComparablePriceExpression::add("ag", "SI", "P * 1 * 1 + 0");
	ArbitrageComparablePriceExpression::add("ag", "rb", "P * 2 * 15 + 0");

    //addCommodity("fu", SHANG_HAI, 1.0); //ȼ���� FU 10��/�� 1Ԫ/��

    addCommodity("a", DA_LIAN, 1.0); //�ƴ�1�� A 10��/�� 1Ԫ/��
	ArbitrageComparablePriceExpression::add("a", "m", "P * 1 * 10 + 0");
    //addCommodity("b", DA_LIAN, 1.0); //�ƴ�2�� B 10��/�� 1Ԫ/��
    addCommodity("m", DA_LIAN, 1.0); //���� M 10��/�� 1Ԫ/��
	ArbitrageComparablePriceExpression::add("m", "ZS", "P * 140 * 1 + 0");
	//ArbitrageComparablePriceExpression::add("m", "y", "P * 1 * 5 + 0");
	//ArbitrageComparablePriceExpression::add("m", "y", "P * 1 * 3 + 0");
	ArbitrageComparablePriceExpression::add("m", "y", "P * 1 * 20 + 0");
	//ArbitrageComparablePriceExpression::add("m", "j", "P * 1 * 50 + 0");
	ArbitrageComparablePriceExpression::add("m", "p", "P * 1 * 20 + 0");
	ArbitrageComparablePriceExpression::add("m", "ZC", "P * 1 * 70 + 0");
	ArbitrageComparablePriceExpression::add("m", "ZM", "P * 90 * 1 + 0");
	ArbitrageComparablePriceExpression::add("m", "a", "P * 1 * 13 + 0");
	ArbitrageComparablePriceExpression::add("m", "OI", "P * 1 * 2 + 0"); //yue
	//ArbitrageComparablePriceExpression::add("m", "RM", "P * 1 * 10 + 0");

	addCommodity("fb", DA_LIAN, 0.05);
	ArbitrageComparablePriceExpression::add("fb", "FG", "P * 1 * 1500 + 0");
	ArbitrageComparablePriceExpression::add("fb", "bb", "P * 500 * 2 + 0");

	addCommodity("bb", DA_LIAN, 0.05);
	ArbitrageComparablePriceExpression::add("bb", "TA", "P * 500 * 1 + 0");
	ArbitrageComparablePriceExpression::add("bb", "rb", "P * 500 * 1 + 0");
	ArbitrageComparablePriceExpression::add("bb", "cu", "P * 500 * 4 + 0");
	ArbitrageComparablePriceExpression::add("bb", "fb", "P * 500 * 1 + 0");
	ArbitrageComparablePriceExpression::add("bb", "y",  "P * 500 * 1 + 0");
	ArbitrageComparablePriceExpression::add("bb", "TC",  "P * 500 * 3 + 0");
	ArbitrageComparablePriceExpression::add("bb", "ru",  "P * 500 * 2 + 0");
	
    //addCommodity("l", DA_LIAN, 5.0); //���͵��ܶȾ���ϩ L 5��/�� 5Ԫ/��
    addCommodity("y", DA_LIAN, 2.0); //���� Y 10��/�� 2Ԫ/��
	//ArbitrageComparablePriceExpression::add("y", "p", "P * 1 * 100 + 0");
	//ArbitrageComparablePriceExpression::add("y", "m", "P * 1 * 1 + 0");
	ArbitrageComparablePriceExpression::add("y", "m", "P * 1 * 10 + 0");
	//ArbitrageComparablePriceExpression::add("y", "p", "P * 1 * 1 + 0");
	ArbitrageComparablePriceExpression::add("y", "KPO", "P * 1 * 15 + 0");
	ArbitrageComparablePriceExpression::add("y", "GKPO", "P * 1 * 15 + 0");
	ArbitrageComparablePriceExpression::add("y", "OI", "P * 1 * 1 + 0");
	ArbitrageComparablePriceExpression::add("y", "RM", "P * 1 * 10 + 0");
	ArbitrageComparablePriceExpression::add("y", "bb", "P * 10 * 1 + 0");
	ArbitrageComparablePriceExpression::add("y", "FG", "P * 10 * 1 + 0");
	ArbitrageComparablePriceExpression::add("y", "jd", "P * 10 * 1 + 0");

    addCommodity("c", DA_LIAN, 1.0); //���� C 10��/�� 1Ԫ/��

    addCommodity("p", DA_LIAN, 2.0); //����� P 10��/�� 2Ԫ/��
	//ArbitrageComparablePriceExpression::add("p", "y", "P * 1 * 130 + 0");
	ArbitrageComparablePriceExpression::add("p", "m", "P * 1 * 10 + 0");
	ArbitrageComparablePriceExpression::add("p", "RM", "P * 1 * 1 + 0");
	ArbitrageComparablePriceExpression::add("p", "KPO", "P * 1 * 20 + 0");
	ArbitrageComparablePriceExpression::add("p", "GKPO", "P * 1 * 20 + 0");
	ArbitrageComparablePriceExpression::add("p", "ZS", "P * 1 * 60 + 0");
	ArbitrageComparablePriceExpression::add("p", "OI", "P * 6 * 10 + 0");
	//ArbitrageComparablePriceExpression::add("p", "y", "P * 1 * 1 + 0");

	addCommodity("jm", DA_LIAN, 0.5); 
	ArbitrageComparablePriceExpression::add("jm", "jm", "P * 1 * 1 + 0");
	ArbitrageComparablePriceExpression::add("jm", "j", "P * 2 * 60 + 0");
	ArbitrageComparablePriceExpression::add("jm", "FG", "P * 60 * 2 + 0");
	ArbitrageComparablePriceExpression::add("jm", "i", "P * 240 * 1 + 0");
	ArbitrageComparablePriceExpression::add("jm", "TC", "P * 2 * 60 + 0");

	addCommodity("TC", ZHENG_ZHOU,0.2); 
	ArbitrageComparablePriceExpression::add("TC", "j", "P * 2 * 1 + 0");
	ArbitrageComparablePriceExpression::add("TC", "jm", "P * 1 * 200 + 0");
	ArbitrageComparablePriceExpression::add("TC", "FG", "P * 1 * 200 + 0");
	ArbitrageComparablePriceExpression::add("TC", "bb", "P * 2 * 200 + 0");

    //addCommodity("v", DA_LIAN, 5.0); //������ϩ V 5��/�� 5Ԫ/��

	addCommodity("jd", DA_LIAN,1.0); //egg 
	ArbitrageComparablePriceExpression::add("jd", "OI", "P * 10 * 2 + 0");
	ArbitrageComparablePriceExpression::add("jd", "y", "P * 10 * 2 + 0");

    addCommodity("j", DA_LIAN, 0.5); //��̼ 
	ArbitrageComparablePriceExpression::add("j", "TC", "P * 1 * 1 + 0");
	ArbitrageComparablePriceExpression::add("j", "rb", "P * 100 * 1 + 0");
	ArbitrageComparablePriceExpression::add("j", "jm", "P * 1 * 100 + 0");
	//ArbitrageComparablePriceExpression::add("j", "m", "P * 1 * 100 + 0");
	ArbitrageComparablePriceExpression::add("j", "i", "P * 1 * 100 + 0");
	ArbitrageComparablePriceExpression::add("j", "TA", "P * 1 * 100 + 0");
	//ArbitrageComparablePriceExpression::add("j", "i", "P * 1 * 200 + 0");

	addCommodity("i", DA_LIAN, 0.5); //����ʯ 
	ArbitrageComparablePriceExpression::add("i", "rb", "P * 100 * 3 + 0");
	ArbitrageComparablePriceExpression::add("i", "j", "P * 100 * 2 + 0");
	ArbitrageComparablePriceExpression::add("i", "jm", "P * 100 * 3 + 0");
	//ArbitrageComparablePriceExpression::add("i", "j", "P * 1 * 300 + 0");

	
	addCommodity("pp", DA_LIAN,1.0); //egg 
	ArbitrageComparablePriceExpression::add("pp", "TA", "P * 5 * 3 + 0");
	ArbitrageComparablePriceExpression::add("pp", "MA", "P * 5 * 2 + 0");
	addCommodity("hc", DA_LIAN,2.0); //egg 

    //addCommodity("WT", ZHENG_ZHOU, 1.0); //Ӳ�� WT 10��/�� 1Ԫ/��
    addCommodity("WS", ZHENG_ZHOU, 1.0); //ǿ�� WS 10��/�� 1Ԫ/��

    addCommodity("CF", ZHENG_ZHOU, 5.0); //һ���� CF 5��/�� 5Ԫ/��
	ArbitrageComparablePriceExpression::add("CF", "CT", "P * 5 * 4 + 0");
	ArbitrageComparablePriceExpression::add("CF", "TA", "P * 5 * 1 + 0");

	addCommodity("RM", ZHENG_ZHOU, 1.0); //����  10��/�� 10Ԫ/��
	ArbitrageComparablePriceExpression::add("RM", "p", "P * 1 * 2 + 0");
	ArbitrageComparablePriceExpression::add("RM", "y", "P * 1 * 30 + 0");
	ArbitrageComparablePriceExpression::add("RM", "OI", "P * 1 * 30 + 0");

	addCommodity("RS", ZHENG_ZHOU, 1.0); //����  10��/�� 10Ԫ/��

	addCommodity("FG", ZHENG_ZHOU, 1.0); //����  20��/�� 1Ԫ/��
	//ArbitrageComparablePriceExpression::add("FG", "FG", "P * 1 * 1 + 0");
	ArbitrageComparablePriceExpression::add("FG", "rb", "P * 20 * 7 + 0");
	ArbitrageComparablePriceExpression::add("FG", "fb", "P * 1 * 80 + 0");
	ArbitrageComparablePriceExpression::add("FG", "jm", "P * 5 * 20 + 0");
	ArbitrageComparablePriceExpression::add("FG", "y",  "P * 20 * 3 + 0");
	ArbitrageComparablePriceExpression::add("FG", "OI",  "P * 20 * 3 + 0");
	ArbitrageComparablePriceExpression::add("FG", "TC",  "P * 20 * 5 + 0");

    addCommodity("pp", DA_LIAN, 1.0); //PP SR 10��/�� 1Ԫ/��
    addCommodity("MA", ZHENG_ZHOU, 1.0); //�״� SR 10��/�� 1Ԫ/��
    addCommodity("ME", ZHENG_ZHOU, 1.0); //�״� SR 10��/�� 1Ԫ/��
	ArbitrageComparablePriceExpression::add("ME", "pp", "P * 2 * 50 + 0");
	ArbitrageComparablePriceExpression::add("ME", "TA", "P * 1 * 50 + 0");
	ArbitrageComparablePriceExpression::add("pp", "ME", "P * 5 * 5 + 0");

    addCommodity("SR", ZHENG_ZHOU, 1.0); //���� SR 10��/�� 1Ԫ/��
	ArbitrageComparablePriceExpression::add("SR", "SB", "P * 1 * 35 + 0");

    addCommodity("SF", ZHENG_ZHOU, 2.0); //���� SF 5��/�� 2Ԫ/��
    addCommodity("SM", ZHENG_ZHOU, 2.0); //�̹� SM 5��/�� 2Ԫ/��


    addCommodity("TA", ZHENG_ZHOU, 2.0); //PTA TA 5��/�� 2Ԫ/��
	//ArbitrageComparablePriceExpression::add("TA", "CL", "P * 75 * 1 + 0");
	ArbitrageComparablePriceExpression::add("TA", "bb", "P * 2 * 5 + 0");
	ArbitrageComparablePriceExpression::add("TA", "CF", "P * 10 * 1 + 0");
	//ArbitrageComparablePriceExpression::add("TA", "CL", "P * 80 * 1 + 0");
	ArbitrageComparablePriceExpression::add("TA", "CL", "P * 85 * 1 + 0");
	//ArbitrageComparablePriceExpression::add("TA", "CL", "P * 85 * 1 + 0");
	//ArbitrageComparablePriceExpression::add("TA", "CL", "P * 90 * 1 + 0");
	//ArbitrageComparablePriceExpression::add("TA", "rb", "P * 5 * 34 + 0");
	ArbitrageComparablePriceExpression::add("TA", "l", "P * 5 * 2 + 0");
	ArbitrageComparablePriceExpression::add("TA", "pp", "P * 5 * 5 + 0");
	ArbitrageComparablePriceExpression::add("TA", "ME", "P * 5 * 4 + 0");
	ArbitrageComparablePriceExpression::add("TA", "MA", "P * 5 * 1 + 0");
	ArbitrageComparablePriceExpression::add("TA", "j", "P * 5 * 4 + 0");

    addCommodity("MA", ZHENG_ZHOU, 1.0); //PTA TA 5��/�� 2Ԫ/��
	ArbitrageComparablePriceExpression::add("MA", "TA", "P * 10 * 1 + 0");
	ArbitrageComparablePriceExpression::add("MA", "pp", "P * 10 * 3 + 0");

	addCommodity("l", DA_LIAN,5.0); //����
	ArbitrageComparablePriceExpression::add("l", "TA", "P * 5 * 1 + 0");

    //addCommodity("RO", ZHENG_ZHOU, 2.0); //������ RO 5��/�� 2Ԫ/��
    //addCommodity("ER", ZHENG_ZHOU, 1.0); //���̵� ER 10��/�� 1Ԫ/��

	addCommodity("OI", ZHENG_ZHOU, 2.0);
	ArbitrageComparablePriceExpression::add("OI", "y", "P * 1 * 1 + 0");
	//ArbitrageComparablePriceExpression::add("OI", "p", "P * 1 * 3 + 0");
	ArbitrageComparablePriceExpression::add("OI", "m", "P * 1 * 1 + 0");
	ArbitrageComparablePriceExpression::add("OI", "RM", "P * 1 * 10 + 0");
	ArbitrageComparablePriceExpression::add("OI", "FG", "P * 1 * 10 + 0");
	ArbitrageComparablePriceExpression::add("OI", "jd", "P * 1 * 10 + 0");
	ArbitrageComparablePriceExpression::add("OI", "p", "P * 5 * 10 + 0");

    //addCommodity("ZW", CBOT, ); //��С��
    addCommodity("ZS", CBOT,0.25); //����
	ArbitrageComparablePriceExpression::add("ZS", "ZC", "P * 50 * 1 + 0");
	ArbitrageComparablePriceExpression::add("ZS", "m", "P * 50 * 6.25 + 0");
	ArbitrageComparablePriceExpression::add("ZS", "p", "P * 50 * 6.25 + 0");

	addCommodity("510050", SH,0.001); //��֤50
	ArbitrageComparablePriceExpression::add("510050", "IH", "P * 1000 * 1 + 0");
	ArbitrageComparablePriceExpression::add("510050", "rb", "P * 1000 * 1 + 0");
	addCommodity("510500", SH,0.001); //��֤500
	ArbitrageComparablePriceExpression::add("510500", "IC", "P * 1000 * 1 + 0");
	ArbitrageComparablePriceExpression::add("510500", "rb", "P * 1000 * 1 + 0");

	addCommodity("510300", SH,0.001); //����300
	ArbitrageComparablePriceExpression::add("510300", "IF", "P * 1000 * 1 + 0");
	ArbitrageComparablePriceExpression::add("510300", "rb", "P * 1000 * 1 + 0");

	inited = true; 
}

void Commodity::addCommodity(string name, 
		      enum Market market,
			  double increment)	
{
	commodities.push_back(new Commodity(name, market, increment));
}

Commodity * Commodity::get(string name)
{
	return findFor(name);
}

bool startWith(const char * s1, const char * s2) {
	assert(strlen(s1) < 1000);
	assert(strlen(s2) < 1000);
	char string1[1000];
	char string2[1000];
	strcpy_s(string1, s1);
	strcpy_s(string2, s2);
	char * cp1 = string1;
	char * cp2 = string2;

	while(*cp2 != '\0')
	{
		if (*cp1 == *cp2)
		{
			cp1++;
			cp2++;
		} else {
			return false;
		}
	}
    return true;
}

Commodity * Commodity::findFor(string contract)
{
    initAsNeccissary();
	
    list<Commodity *>::iterator it;

    for ( it = commodities.begin(); it != commodities.end(); it++ )
	{
		if (startWith(contract.c_str(), (*it)->name.c_str())) return *it;
	}
	TRACE_LOG("commodities.size = %d", commodities.size());
	TRACE_LOG("Unknown Commodity for %s\n", contract.c_str());
    for ( it = commodities.begin(); it != commodities.end(); it++ )
	{
		TRACE_LOG("%p  %s ",(*it),(*it)->name.c_str());
	}
	assert(false);
	exit(1);
	return NULL;
}

double Commodity::calcArbitrageComparablePriceFor(double p, Commodity * comparedCommodity)
{
    ArbitrageComparablePriceExpression * exp = ArbitrageComparablePriceExpression::find(this->name, comparedCommodity->name);
	return exp->getValueFor(p);
}

bool Commodity::isDomestic()
{
	if(market == SHANG_HAI
	   || market == ZHENG_ZHOU
	   || market == DA_LIAN
	   || market == ZHONGJIN) return true;
	return false;
}

bool Commodity::isOversea()
{
	return !isDomestic();
}

string Commodity::getName()
{
	return name;
}

bool Commodity::inTrading(time_t t)
{
	return this->exchange->inTrading(t);
}

char Commodity::getExchangeNumberForYiSheng()
{
	switch (market)
	{
		case SHANG_HAI:
			 return '3';
		case ZHENG_ZHOU :
			return '1';
		case DA_LIAN :
			return '2';
		case ZHONGJIN :
			return '5';
		default:
			return '0';
	}

}
