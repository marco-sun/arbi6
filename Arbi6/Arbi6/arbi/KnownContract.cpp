#include "KnownContract.h"
#include "../LogStore.h"
#include <algorithm>
#define FIX 1
#define RONHON 2

ContractNames::ContractNames(string spName, string fixName, string ronhonName, string patsapiName)
{
	this->spName = spName;
	this->fixName = fixName;
	this->ronhonName = ronhonName;
	this->patsapiName = patsapiName;
}

ContractNames::~ContractNames(void)
{
}

KnownContract::KnownContract(void)
{
}


KnownContract::~KnownContract(void)
{
}

void KnownContract::add(string spName, string fixName, string ronhonName, string patsapiName)
{
	names.push_back(new ContractNames(spName, fixName, ronhonName, patsapiName));
}

/*TODO: */string KnownContract::getSpNameFor(string anyName)
{
	return getNameFor(anyName)->spName;
}
/**/

string KnownContract::getFixNameFor(string anyName)
{
	return getNameFor(anyName)->fixName;
}

string KnownContract::getPatsApiNameFor(string anyName)
{
	return getNameFor(anyName)->patsapiName;
}

string KnownContract::getRonhonNameFor(string anyName)
{
	using std::replace;
	//TRACE_LOG("getRonhonNameFor 's spName is: %s",anyName.c_str());
	string n = getNameFor(anyName)->ronhonName;
	//TRACE_LOG("Knowcontract-get spName is : %s",anyName.c_str());
	//replace( n.begin(), n.end(), '-', ' ');
	return n;
}

ContractNames * KnownContract::getNameFor(string anyName)
{
	//TRACE_LOG("enter ContractNames part of getNameFor,the para spName is:%s ",anyName.c_str());
    list<ContractNames *>::iterator the_iterator = names.begin();
	//TRACE_LOG("list is ok!");
    while( the_iterator != names.end() ) {
		ContractNames * cn = (*the_iterator);
		if(cn->spName.compare(anyName) == 0 ||
			cn->fixName.compare(anyName) == 0 ||
			cn->ronhonName.compare(anyName) == 0 ||
			cn->patsapiName.compare(anyName) == 0) 						{
			return (*the_iterator);
		}
        the_iterator++;
	}

	TRACE_LOG("Unkown name: %s",anyName.c_str());
	exit(1);

	return dumbContractNames;	
}


string KnownContract::getFixCommodityFor(string spName)
{
	return partOneOf(getFixNameFor(spName));
}

string KnownContract::getFixYearMonthFor(string spName)
{
	return partTwoOf(getFixNameFor(spName));
}

string KnownContract::getFixExchangeFor(string spName)
{
	return partThreeOf(getFixNameFor(spName));
}

string KnownContract::partOneOf(string fixName)
{
	int pos = fixName.find("-");
	return fixName.substr(0,pos);
}

string KnownContract::partTwoOf(string fixName)
{
	if(fixName.find("LME") != string::npos) return fixName.substr(3,2);
	return fixName.substr(3,6);
}

string KnownContract::partThreeOf(string fixName)
{
	int start = fixName.find("@") + 1;
	return fixName.substr(start, fixName.length() - start);
}

list<ContractNames *> KnownContract::names;

ContractNames * KnownContract::dumbContractNames = new ContractNames("", "", "", "");
