// SpPriceSource.h: interface for the SpPriceSource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPPRICESOURCE_H__6D32529D_4B22_41CC_AB9C_6D7A6C6700FC__INCLUDED_)
#define AFX_SPPRICESOURCE_H__6D32529D_4B22_41CC_AB9C_6D7A6C6700FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable : 4786)
#include <list>
using namespace std;
#include "PriceSource.h"
#include "ClientSock.h"
#include "SPInfor.h"
#include <windows.h>

typedef struct tmsg
{	
	char InstrumentID[32];
} Tmsg;

typedef list<Tmsg> LISTCHARM;


class SpPriceSource : public PriceSource  
{
public:
	SpPriceSource();
	virtual ~SpPriceSource();

	virtual void start();
	virtual void add(string ID);

	void ReadIniConfig();
	int  startTcp();
	int  DecodeMsgHead(char * pmsg);
	void SPPrcSnapReq(char * szInstrumentID);
	int  DealSnapPrice(char * pmsg);

	CTcpClinet m_cTcpClinet;
	CSPPublicFun m_sPPublic;
	CRITICAL_SECTION csPrice;
private:
	
	bool started;

public:
//not used now.
	int  closeTcp();
	void SPUserLogin();
	void SPAccountLogin();
	void SPUserLogout();
	void SPAccountLogout();
	int  DealUserLoadReply(char * pmsg);
	int  DealAccountLoadReply(char * pmsg);
   
};

#endif // !defined(AFX_SPPRICESOURCE_H__6D32529D_4B22_41CC_AB9C_6D7A6C6700FC__INCLUDED_)
