#pragma   warning   (disable:   4786) 

#include "LogList.h"
#include "afxmt.h"
CCriticalSection   gbmutex; 
LISTCHAR g_loglist;

CLoglist::CLoglist(void)
{
}

CLoglist::~CLoglist(void)
{
}

void CLoglist::set_log(char * plog)
{
	CSingleLock   lock(&gbmutex); 
	lock.Lock(); 
	g_loglist.push_back(plog);
	lock.Unlock(); 
}

int CLoglist::get_log(char * plog)
{
	CSingleLock   lock(&gbmutex); 
	lock.Lock(); 
	list<string>::iterator it = g_loglist.begin();
	if(it != g_loglist.end())	
	{
		strcpy_s(plog, 1024, (*it).c_str());
		g_loglist.pop_front();
		lock.Unlock();
		return 1;
	}
	lock.Unlock();
    return 0;
}
