// MockAsyncPriceSource.h: interface for the MockAsyncPriceSource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOCKASYNCPRICESOURCE_H__42B3586A_F79E_4D08_9A5E_BFD76C322674__INCLUDED_)
#define AFX_MOCKASYNCPRICESOURCE_H__42B3586A_F79E_4D08_9A5E_BFD76C322674__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PriceSource.h"
#include <windows.h>
#include <list>
using namespace std;

class MockAsyncPriceSource : public PriceSource  
{
public:
	MockAsyncPriceSource();
	virtual ~MockAsyncPriceSource();
	
	virtual	void start();
	virtual void add(string contract);
	void issuePrice();

	list<string> contracts;
	static void threadPrice( void* pParams ) ;

private:
	double ask[60];
	double bid[60];
	int index;
	CRITICAL_SECTION cs;

};



#endif // !defined(AFX_MOCKASYNCPRICESOURCE_H__42B3586A_F79E_4D08_9A5E_BFD76C322674__INCLUDED_)
