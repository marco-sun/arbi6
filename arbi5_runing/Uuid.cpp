// Uuid.cpp: implementation of the Uuid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Uuid.h"
#include <Objbase.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

string Uuid::gen()
{
	GUID *pguid = new GUID();
	CoCreateGuid(pguid);
	WCHAR *pBuf = new WCHAR[64];
	int nResult = StringFromGUID2(*pguid, pBuf, 64);
	
	char * pcBuf = new char[64];
	for (int i = 0; i < nResult; i++)
	{
        pcBuf[i] = pBuf [i];
	}

    //printf("asdfsdfsd %d %s", nResult, pcBuf);
    //wprintf(L"aaaa %s", pBuf);

    string s = pcBuf;
	delete [] pBuf;
	delete [] pcBuf;
	return s;

}