// Contract.h: interface for the Contract class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTRACT_H__737E5966_E11C_4F38_87E2_03D1B9DC35DD__INCLUDED_)
#define AFX_CONTRACT_H__737E5966_E11C_4F38_87E2_03D1B9DC35DD__INCLUDED_

//#pragma warning(disable : 4786)

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Price.h"
#include "Commodity.h"

#include <string>
#include <list>
#include <map>
#include <fstream> 
#include "Windows.h"
using namespace std;

typedef map<string, Price*> PriceMap; 

typedef enum
{
	COMMODITY,
	MONTH,
	MARKET
} NAME_OF;


class Contract  
{
public:
	char debug[16];

	virtual ~Contract();

	static Contract * get(string name);
	static Contract * get(string comodityName, string exchangeName, string CCYYMM);
	
	//TODO: remove this
	string getComodityName();
	string getExchangeName();
	string getCCYYMM();
	
	Price getPrice();
	string getName();
	string getFixName();
	string getRonhonName();
	string getPatsApiName();
	void setPrice(time_t t, double ask, double bid, double lastPrice);
	void setPrice(time_t t, double ask, double bid);
	void setPrice(double ask, double bid);
	bool isDomestic();
	bool isOversea();
	bool isNearThan(Contract * c);
	double getArbitrageComparablePriceFor(string askBid, string comparedContract);
	bool hasValidPrice();
	bool inTrading(time_t t);
    Commodity * getCommodity();
	void setOrderCount(int count);
	bool allowOrder();
	void increaseOrderCount();
	void increaseOrderCount(int i);
	int getOrderCount();

	static string getDateFileName();
	static void writeToFile(string name);
	static void openFile();
	static ofstream writeFile;
	static void loadOrderCount();
	char getExchangeNumberForYiSheng();

private:
	Contract(string spName);
	string spName;
	string fixName;
	string ronhonName;
	string patsapiName;
	Price price;
	CRITICAL_SECTION cs;
	Commodity * commodity;
	int orderCount;

    static Contract * findInList(string name);
	static list<Contract *> contracts;

    void initPriceAsNeccissary(string name);
	bool beyondOrderCountLimit();
	bool sugar();
	Price sugarPrice();

	//string getNameOf(NAME_OF n);
	//string nameOf[3];

	//void parseName();
	//void initNameOf();
	bool match(string name);
};

#endif // !defined(AFX_CONTRACT_H__737E5966_E11C_4F38_87E2_03D1B9DC35DD__INCLUDED_)
