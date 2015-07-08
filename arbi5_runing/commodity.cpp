// commodity.cpp: implementation of the Commodity class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "commodity.h"
#include "ArbitrageComparablePriceExpression.h"
#include "LogStore.h"
#include <assert.h>
#include "SPInfor.h"
#include <fstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



Commodity::Commodity()
{
	memset(debug,0,16);
	sprintf(debug,"Commodity_");
}

Commodity::	Commodity(string name, 
		      enum Market market,
			  double increment)
{
	memset(debug,0,16);
	sprintf(debug,"Commodity");
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
	addCommodity("IH", ZHONGJIN, 0.2);
	addCommodity("IC", ZHONGJIN, 0.2);
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
	//ArbitrageComparablePriceExpression::add("rb", "TA", "P * 10 * 36 + 0");
	//ArbitrageComparablePriceExpression::add("rb", "IF", "P * 10 * 18 + 0");

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

    addCommodity("ZC", CBOT,0.25); //������
	ArbitrageComparablePriceExpression::add("ZC", "ZS", "P * 100 * 1 + 0");
	ArbitrageComparablePriceExpression::add("ZC", "m", "P * 50 * 6.25 + 0");

    addCommodity("ZM", CBOT,0.1); //������
	ArbitrageComparablePriceExpression::add("ZM", "m", "P * 6.25 * 100 + 0");
    //addCommodity("ZL", CBOT,); //������

    addCommodity("CT", NBOT_CT, 0.01); //���޻�
	ArbitrageComparablePriceExpression::add("CT", "CF", "P * 6.25 * 500 + 0");

    //addCommodity("SB", NBOT,); //��11��
    addCommodity("SB", NBOT_SB, 0.01); //��11��
    addCommodity("SBH5", NBOT_SB, 0.01); //��11��
	ArbitrageComparablePriceExpression::add("SB", "SR", "P * 6.25 * 1120 + 0");
	ArbitrageComparablePriceExpression::add("SB", "LSU", "P * 22.0462 * 1 + 0");
	ArbitrageComparablePriceExpression::add("SBH5", "LSU", "P * 22.0462 * 1 + 0");

    addCommodity("HG", NYMEX, 0.05); //ŦԼͭ (normal 0.05;�¼ҿ�0.0005)
	ArbitrageComparablePriceExpression::add("HG", "cu", "P * 6.25 * 500 + 0");//normal 6.25;�¼ҿ�6400
	ArbitrageComparablePriceExpression::add("HG", "LNI", "P * 250 * 1 + 0");

    addCommodity("GC", NYMEX, 0.1); //ŦԼ��
	//ArbitrageComparablePriceExpression::add("GC", "au", "P * 6.25 * 0.03215434 + 0");//chengjiajun 6.25
	ArbitrageComparablePriceExpression::add("GC", "au", "P * 6.25 * 0.03215434 + 0");

	addCommodity("CL", NYMEX, 0.01); //ŦԼԭ��
	ArbitrageComparablePriceExpression::add("CL", "TA", "P * 6.25 * 1000 + 0");

	addCommodity("PL", NYMEX, 0.1); //ŦԼ�׽�

	addCommodity("SI", NYMEX, 0.005); //ŦԼ����
	ArbitrageComparablePriceExpression::add("SI", "ag", "P * 6.25 * 32.154 + 0");
	ArbitrageComparablePriceExpression::add("SI", "rb", "P * 6.25 * 5000 + 0");
	//ArbitrageComparablePriceExpression::add("SI", "ag", "P * 6.25 * 32.154 + 0");//chengjiajun
	//ArbitrageComparablePriceExpression::add("SI", "rb", "P * 6.25 * 5000 + 0");//chengjiajun

    addCommodity("LCA", LME, 0.25); //�׶�ͭ
	ArbitrageComparablePriceExpression::add("LCA", "cu", "P * 6.25 * 25 + 0");

    addCommodity("LZS", LME, 0.25); //�׶�п
    ArbitrageComparablePriceExpression::add("LZS", "zn", "P * 6.25 * 25 + 0");

    addCommodity("LAH", LME, 0.25); //�׶���
    ArbitrageComparablePriceExpression::add("LAH", "al", "P * 6.25 * 25 + 0");

	addCommodity("ni", SHANG_HAI, 10); //�Ϻ���
	ArbitrageComparablePriceExpression::add("ni", "LNI", "P * 5 * 1 + 0");
	
	addCommodity("LNI", LME, 5); //�׶���
	ArbitrageComparablePriceExpression::add("LNI", "HG", "P * 6 * 1 + 0");
	ArbitrageComparablePriceExpression::add("LNI", "ni", "P * 6.25 * 6 + 0");

	addCommodity("LSU", LMELSU,0.25); //�׶���
	ArbitrageComparablePriceExpression::add("LSU", "SB", "P * 1 * 1 + 0");


	addCommodity("SN", LME, 0.25); //�׶���
    ArbitrageComparablePriceExpression::add("SN", "SN", "P * 1 * 1 + 0");

	addCommodity("JRU", TOCOM, 0.1); //����
	ArbitrageComparablePriceExpression::add("JRU", "ru", "P * 52 * 1.15 + 0");

	addCommodity("6A", LME, 0.0001); //��Ԫ
	addCommodity("6C", LME, 0.0001); //��Ԫ
	ArbitrageComparablePriceExpression::add("6A", "6C", "P * 10000 * 1 + 0");
	ArbitrageComparablePriceExpression::add("6C", "6A", "P * 10000 * 1 + 0");

	addCommodity("GJRU", TOCOM, 0.1); //����
	ArbitrageComparablePriceExpression::add("GJRU", "ru", "P * 61 * 1.15 + 0");

	addCommodity("KPO", BMD, 1);
	ArbitrageComparablePriceExpression::add("KPO", "p", "P * 1 * 50 + 0");
	ArbitrageComparablePriceExpression::add("KPO", "y", "P * 1 * 50 + 0");

	addCommodity("GKPO", BMD, 1);
	ArbitrageComparablePriceExpression::add("GKPO", "p", "P * 1 * 50 + 0");
	ArbitrageComparablePriceExpression::add("GKPO", "y", "P * 1 * 50 + 0");

	addCommodity("GFPO", BMD, 0.5);
	ArbitrageComparablePriceExpression::add("GFPO", "p", "P * 1 * 50 + 0");
	ArbitrageComparablePriceExpression::add("GFPO", "y", "P * 1 * 50 + 0");

	addCommodity("BPTA", BOHAI, 2);
	addCommodity("BCK", BOHAI, 2);
	addCommodity("BAG", BOHAI, 1);
	ArbitrageComparablePriceExpression::add("BAG", "ag", "P * 1 * 1.17 + 0");
	addCommodity("BRURV", BOHAI, 5);
	ArbitrageComparablePriceExpression::add("BRURV", "ru", "P * 1 * 1.17 + 1404");
	addCommodity("BSC", BOHAI, 1);
	addCommodity("BCUSH", BOHAI, 1);
	ArbitrageComparablePriceExpression::add("BCUSH", "cu", "P * 1 * 1.17 + 0");

	addCommodity("BRBWX", BOHAI, 1);

	inited = true; 
	//TRACE_LOG("HERE");
}

/*
void Commodity::initAsNeccissary()
{
	if(inited) return;
	TRACE_LOG("Commodity::initAsNeccissary");
	//inspect why list's head object was flushed.

	//p = new char[1]; //add heap protection.

	//TODO: add all commodities
	addCommodity("IF", ZHONGJIN, 0.2);

	//addCommodity("TF", ZHONGJIN, 0.01);
	addCommodity("TF", ZHONGJIN, 0.002);

    addCommodity("rb", SHANG_HAI, 1.0); //���Ƹ� RB 10��/�� 1Ԫ/��

	addCommodity("pb", SHANG_HAI, 5.0); //Ǧ PB 10��/�� 5Ԫ/��

    addCommodity("ru", SHANG_HAI, 5.0); //�� RU 5��/�� 5Ԫ/��

    //addCommodity("wr", SHANG_HAI, 1.0); //�߲� WR 10��/�� 1Ԫ/��
    addCommodity("cu", SHANG_HAI, 10.0); //����ͭ CU 5��/�� 10Ԫ/��

	addCommodity("bu", SHANG_HAI, 2.0); //���� bu 10��/�� 2Ԫ/��

    addCommodity("al", SHANG_HAI, 5.0); //�� AL 5��/�� 5Ԫ/��

    addCommodity("zn", SHANG_HAI, 5.0); //п ZN 5��/�� 5Ԫ/��

    addCommodity("au", SHANG_HAI, 0.05); //�ƽ� AU 1000��/�� 0.01Ԫ/��

	addCommodity("ag", SHANG_HAI, 1.0); //���� ag 15ǧ��/��  1Ԫ/ǧ��

    //addCommodity("fu", SHANG_HAI, 1.0); //ȼ���� FU 10��/�� 1Ԫ/��

    addCommodity("a", DA_LIAN, 1.0); //�ƴ�1�� A 10��/�� 1Ԫ/��
    
    addCommodity("m", DA_LIAN, 1.0); //���� M 10��/�� 1Ԫ/��
	
    //addCommodity("l", DA_LIAN, 5.0); //���͵��ܶȾ���ϩ L 5��/�� 5Ԫ/��
    addCommodity("y", DA_LIAN, 2.0); //���� Y 10��/�� 2Ԫ/��

    addCommodity("c", DA_LIAN, 1.0); //���� C 10��/�� 1Ԫ/��

    addCommodity("p", DA_LIAN, 2.0); //����� P 10��/�� 2Ԫ/��

	addCommodity("jm", DA_LIAN,1.0); 

	addCommodity("TC", ZHENG_ZHOU,0.2); 

    //addCommodity("v", DA_LIAN, 5.0); //������ϩ V 5��/�� 5Ԫ/��
    addCommodity("j", DA_LIAN,1.0); //��̼ 

	addCommodity("i", DA_LIAN,1.0); //����ʯ 

    //addCommodity("WT", ZHENG_ZHOU, 1.0); //Ӳ�� WT 10��/�� 1Ԫ/��
    addCommodity("WS", ZHENG_ZHOU, 1.0); //�� WS 10��/�� 1Ԫ/��

    addCommodity("CF", ZHENG_ZHOU, 5.0); //һ���� CF 5��/�� 5Ԫ/��

	addCommodity("RM", ZHENG_ZHOU, 1.0); //����  10��/�� 10Ԫ/��

	addCommodity("RS", ZHENG_ZHOU, 1.0); //����  10��/�� 10Ԫ/��

	addCommodity("FG", ZHENG_ZHOU, 1.0); //����  20��/�� 1Ԫ/��

    addCommodity("SR", ZHENG_ZHOU, 1.0); //���� SR 10��/�� 1Ԫ/��

    addCommodity("TA", ZHENG_ZHOU, 2.0); //PTA TA 5��/�� 2Ԫ/��

	addCommodity("l", DA_LIAN,5.0); //����

    //addCommodity("RO", ZHENG_ZHOU, 2.0); //������ RO 5��/�� 2Ԫ/��
    //addCommodity("ER", ZHENG_ZHOU, 1.0); //���̵� ER 10��/�� 1Ԫ/��

	addCommodity("OI", ZHENG_ZHOU, 2.0);

    //addCommodity("ZW", CBOT, ); //��С��
    addCommodity("ZS", CBOT,0.25); //����

    addCommodity("ZC", CBOT,0.25); //������

    addCommodity("ZM", CBOT,0.1); //������
    //addCommodity("ZL", CBOT,); //������

    addCommodity("CT", NBOT_CT, 0.01); //���޻�

	addCommodity("SBH3", NBOT_SB, 0.01); //��11��

	addCommodity("SBK3", NBOT_SB, 0.01); 

	addCommodity("SBN3", NBOT_SB, 0.01); 

	addCommodity("SBV3", NBOT_SB, 0.01); 

    addCommodity("HG", NYMEX, 0.05); //ŦԼͭ (normal 0.05;�¼ҿ�0.0005)

    addCommodity("GC", NYMEX, 0.1); //ŦԼ��

	addCommodity("CL", NYMEX, 0.01); //ŦԼԭ��

	addCommodity("PL", NYMEX, 0.1); //ŦԼ�׽�

	addCommodity("SI", NYMEX, 0.005); //ŦԼ����

    addCommodity("LCA", LME, 0.25); //�׶�ͭ

    addCommodity("LZS", LME, 0.25); //�׶�п

    addCommodity("LAH", LME, 0.25); //�׶���
	
	addCommodity("LNI", LME, 1); //�׶���

	addCommodity("LSU", LMELSU,0.25); //�׶���

	addCommodity("SN", LME, 0.25); //�׶���

	addCommodity("JRU", TOCOM, 0.1); //����

	addCommodity("KPO", BMD, 1);

	addCommodity("BPTA", BOHAI, 2);
	addCommodity("BCK", BOHAI, 2);
	addCommodity("BAG", BOHAI, 1);
	addCommodity("BRURV", BOHAI, 5);
	addCommodity("BSC", BOHAI, 1);
	addCommodity("BCUSH", BOHAI, 1);

	addCommodity("BRBWX", BOHAI, 1);
	addCommodity("ZERO", ZHENG_ZHOU, 1);
	addCommodity("SP", ZHENG_ZHOU, 1);

	//read commodity.txt example: rb,j,P * 50 * 1 + 0
	TRACE_LOG("Load commodity.txt");
	ifstream file("commodity.txt");
	if(file.fail()) {
		TRACE_LOG("Read commodity.txt fail");
		exit(1);
	}
	string contract1, contract2, diffExp;
	string temp;
	while(getline(file,temp))
	{
		strings pars;	
		strings::iterator it;
		int npar = 0;
		int code = 0;
		CSPPublicFun m_sPPublic;
		m_sPPublic.divide_string(temp.c_str(),pars,",",TRUE);
		for(npar = 0,it = pars.begin(); it != pars.end();it++,npar++)
		{
			switch(npar) {
				case 0:
					contract1 = (* it);
				case 1:
					contract2 = (* it);
				case 2:
					diffExp = (* it);
				default:
					break;
				}
		}
		if(contract1.length() >= 1)
		{
			TRACE_LOG("read ArbitrageComparablePriceExpression %s %s %s", contract1.c_str(), contract2.c_str(), diffExp.c_str());
			ArbitrageComparablePriceExpression::add(contract1, contract2, diffExp);
		}
	}
	inited = true; 
}
*/

void Commodity::addCommodity(string name, 
		      enum Market market,
			  double increment)	
{
	commodities.push_back(new Commodity(name, market, increment));
	//TRACE_LOG("%s  %p", name.c_str(), commodities.back());
}

Commodity * Commodity::get(string name)
{
/*
    initAsNeccissary();

    list<Commodity *>::iterator it;

    for ( it = commodities.begin(); it != commodities.end(); it++ )
	{
		if ((*it)->name.compare(name) == 0) return *it;
	}

	TRACE_LOG("Unknown Commodity %s\n", name.c_str());
	exit(1);
	return NULL;
	*/
	return findFor(name);
}

bool startWith(const char * s1, const char * s2) {
	assert(strlen(s1) < 1000);
	assert(strlen(s2) < 1000);
	char string1[1000];
	char string2[1000];
	strcpy(string1, s1);
	strcpy(string2, s2);
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
	}

}
