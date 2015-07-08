#include "KnownContract.h"
#include "LogStore.h"
#include <algorithm>
#define FIX 1
#define RONHON 2
//#include <fstream>


ContractNames::ContractNames(string spName, string fixName, string ronhonName)
{
	this->spName = spName;
	this->fixName = fixName;
	this->ronhonName = ronhonName;
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

void KnownContract::add(string spName, string fixName, string ronhonName)
{
	names.push_back(new ContractNames(spName, fixName, ronhonName));
}

string KnownContract::getFixNameFor(string spName)
{
	return getNameFor(spName)->fixName;
}

string KnownContract::getRonhonNameFor(string spName)
{
	using std::replace;
	string n = getNameFor(spName)->ronhonName;
	//replace( n.begin(), n.end(), '-', ' ');
	return n;
}

ContractNames * KnownContract::getNameFor(string spName)
{
    list<ContractNames *>::iterator the_iterator = names.begin();

    while( the_iterator != names.end() ) {
		if((*the_iterator)->spName.compare(spName) == 0) return (*the_iterator);
        the_iterator++;
	}

	TRACE_LOG("Unkown name: %s",spName.c_str());
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

ContractNames * KnownContract::dumbContractNames = new ContractNames("", "", "");
