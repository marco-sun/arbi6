#pragma once
#include <list>
#include <string>
using namespace std;
typedef list<string> LISTCHAR;

extern LISTCHAR g_loglist;

class CLoglist
{
public:
	CLoglist(void);
	~CLoglist(void);
	void set_log(char * plog);
	int  get_log(char * plog);
};
