// DealHistory.cpp: implementation of the DealHistory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DealHistory.h"
#include "Persistence.h"

#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

string todayFile();

DealHistory::DealHistory()
{
	InitializeCriticalSection(&cs);
	fout.open(todayFile().c_str(), ios::app);
}

DealHistory::~DealHistory()
{

}

/*
void DealHistory::load()
{
	Persistence::loadHistory();

}
*/

DealHistory * DealHistory::instance = NULL;

DealHistory * DealHistory::getInstance()
{
	if(instance == NULL) instance = new DealHistory();	
	return instance;
}

list<string> DealHistory::listAll()
{
	Persistence::loadAsNecessary();
	return dealHistories;
}

void DealHistory::add(string history)
{ 

	EnterCriticalSection(&cs);
    dealHistories.push_back(history);
	fout << history << endl;
	LeaveCriticalSection(&cs);
}

void DealHistory::insertOld(string history)
{
	EnterCriticalSection(&cs);
    dealHistories.push_back(history);
	LeaveCriticalSection(&cs);
}

void DealHistory::clear()
{
	dealHistories.clear();
}