// CancelQueue.h: interface for the CancelQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CANCELQUEUE_H__C9B988A1_225C_4639_90C3_A937D133B44A__INCLUDED_)
#define AFX_CANCELQUEUE_H__C9B988A1_225C_4639_90C3_A937D133B44A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Order.h"
#include "process.h"
#include <Windows.h>
#include <list>
using namespace std;

class CancelQueue  
{
public:

	virtual ~CancelQueue();
	void add(Order * o);
	void checkQueue();
    static CancelQueue * getInstance();
	void clear();
	int size();
private:
	static CancelQueue * instance;
	CancelQueue();
	list<Order*> orders;
	bool threadStarted;
	void startThreadAsNecessary();
	CRITICAL_SECTION cs;

};

#endif // !defined(AFX_CANCELQUEUE_H__C9B988A1_225C_4639_90C3_A937D133B44A__INCLUDED_)
