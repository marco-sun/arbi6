#if !defined FixEncode_H
#define FixEncode_H

#pragma once
#include <list>
#include <map>
#include <string>
using namespace std;
#include "FixPubTools.h"
#include "fixmsgdef.h"

struct TFIXMSG
{
	string msg_tag;
	string msg_value;
};

typedef list<TFIXMSG> LISTFIXMSG;

typedef list<LISTFIXMSG> LISTFIXMSGALL;

class FixEncode //: public Dealer
{
public:
	FixEncode();
	~FixEncode();

	static string fixEncodeMsg(LISTFIXMSG listFix, string type, long msgSeqNum);
	static string generateCurtime();
    
protected:
	static TFIXMSG fix_decode_tag_value(string sparts);
	static string generateHeader(string type, long bodylen, long msgSeqNum);
	static string generateTrailer(string sbuf, long bufLen);
	static string generateCheckSum(char *buf, long bufLen);
	
};

#endif 
