#pragma once

#include <string>
#include <list>
using namespace std;

class ContractNames
{
public:
	ContractNames(string spName, string fixName, string ronhonName, string patsapiName);
	~ContractNames(void);
	string spName;
	string fixName;
	string ronhonName;
	string patsapiName;
};

class KnownContract
{
public:
	KnownContract(void);
	~KnownContract(void);
	static void add(string spName,string fixName,string ronhonName, string patsapiName);

	//TODO: remove this
	static string getSpNameFor(string anyName);
	static string getFixCommodityFor(string spName);
	static string getFixYearMonthFor(string spName);
	static string getFixExchangeFor(string spName);

	static list<ContractNames *> names;
	static string getFixNameFor(string spName);
	static string getRonhonNameFor(string spName);
	static string getPatsApiNameFor(string spName);

private:
	//TODO: remove this
	static string partOneOf(string fixName);
    static string partTwoOf(string fixName);
    static string partThreeOf(string fixName);
	static ContractNames * getNameFor(string spName);
	static ContractNames * dumbContractNames;

};

