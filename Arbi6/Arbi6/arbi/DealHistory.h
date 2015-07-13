// DealHistory.h: interface for the DealHistory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEALHISTORY_H__F3DD3FF3_19DD_447E_8C17_9A071D665C55__INCLUDED_)
#define AFX_DEALHISTORY_H__F3DD3FF3_19DD_447E_8C17_9A071D665C55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#pragma warning(disable : 4786)

#include <string>
#include <list>
#include <windows.h>
#include <fstream> 
using namespace std;

class DealHistory  
{
public:
	DealHistory();
	virtual ~DealHistory();
	static DealHistory * getInstance();
	list<string> listAll();
	void add(string history);
	void insertOld(string history);
	void clear();
private:
    static DealHistory * instance;
    list<string> dealHistories;
	CRITICAL_SECTION cs;
	void load();
	ofstream fout; 
};

#endif // !defined(AFX_DEALHISTORY_H__F3DD3FF3_19DD_447E_8C17_9A071D665C55__INCLUDED_)
