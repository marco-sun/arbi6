// FixEncode.cpp: implementation of the FixEncode class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "FixEncode.h"
#include "LogStore.h"
#include <assert.h>

extern string g_senderCompID;
extern string g_targetCompID;

FixEncode::FixEncode()
{
}

FixEncode::~FixEncode()
{
}

string FixEncode::fixEncodeMsg(LISTFIXMSG listFix, string type, long msgSeqNum)
{
	string sret;
	char szTemp[MSGSTRING_MAX_LEN];
	memset(szTemp, 0, sizeof(szTemp));
	list<TFIXMSG>::iterator it ;
	char szpart[] = {0x01,0x00};
	for(it = listFix.begin(); it != listFix.end(); ++it)
	{
		strcat(szTemp, (*it).msg_tag.c_str());
		strcat(szTemp, "=");
		strcat(szTemp, (*it).msg_value.c_str());
		strncat(szTemp, szpart,1);
	}
	string sheader = generateHeader(type, strlen(szTemp), msgSeqNum);
	sheader += szTemp;
	string strailer = generateTrailer(sheader, sheader.size());

	sret = sheader + strailer;
	return sret;
}

string FixEncode::generateHeader(string type, long bodylen, long msgSeqNum)
{
	string ret;
	char szTemp[32];
	char szHeader[256];
	char szHeader2[256];
	char szpart[] = {0x01,0x00};
	memset(szTemp, 0, sizeof(szTemp));
	memset(szHeader, 0, sizeof(szHeader));
	memset(szHeader2, 0, sizeof(szHeader2));

	strcat(szHeader2, "35=");
	strcat(szHeader2, type.c_str());
	strncat(szHeader2, szpart,1);
	strcat(szHeader2, "34=");
	sprintf(szTemp, "%ld", msgSeqNum);
	strcat(szHeader2,  szTemp);
	strncat(szHeader2, szpart,1);
	strcat(szHeader2, "49=");   //??
	strcat(szHeader2, g_senderCompID.c_str() );
	strncat(szHeader2, szpart,1);
	strcat(szHeader2, "52=");
	strcat(szHeader2,  generateCurtime().c_str());
	strncat(szHeader2, szpart,1);
	strcat(szHeader2, "56=");  //???
	strcat(szHeader2, g_targetCompID.c_str() );
	strncat(szHeader2, szpart,1);

	bodylen += strlen(szHeader2);
	sprintf(szTemp, "%ld", bodylen);
	strcat(szHeader, "8=");
	strcat(szHeader, MSGSTRING_BEGINSTRING);
	strncat(szHeader, szpart,1);
	strcat(szHeader, "9=");
	strcat(szHeader, szTemp);
	strncat(szHeader, szpart,1);
	strcat(szHeader, szHeader2);

	ret = szHeader;

	return ret;
}

string FixEncode::generateTrailer(string sbuf, long bufLen)
{
	string ret;
	char szTrailer[128];
	char szpart[] = {0x01,0x00};
	memset(szTrailer, 0, sizeof(szTrailer));
	strcat(szTrailer, "10=");
	char buf[MSGSTRING_MAX_LEN];
	memset(buf, 0, sizeof(buf));
	strcpy(buf, sbuf.c_str());
	strcat(szTrailer, generateCheckSum(buf, bufLen).c_str());
	strncat(szTrailer, szpart,1);
	ret = szTrailer;
	return ret;
}

string FixEncode::generateCheckSum( char *buf, long bufLen )
{
	string ret;
	static char tmpBuf[4];
	long idx;
	unsigned int cks;
	for( idx=0L, cks=0; idx < bufLen; cks += (unsigned int)buf[ idx++ ] );
	sprintf( tmpBuf, "%03d", (unsigned int)( cks % 256 ) );

	ret = tmpBuf;
	return ret;
}

string FixEncode::generateCurtime()
{
	string ret;
	SYSTEMTIME t;
	char sztime[32];
	memset(sztime, 0, sizeof(sztime));
    GetSystemTime( &t);             // gets UTC time
	sprintf(sztime, "%d%02d%02d-%02d:%02d:%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
	ret = sztime;
	return ret;
}