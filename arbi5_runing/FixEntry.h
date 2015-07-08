#pragma once
#include "Fixthread.h"
#include "FixPrice.h"
#include "FixDealer.h"
#include "FixDecode.h"
#include "FixTcpTrans.h"


typedef map<string, int> MDEntryTypeIDMap; 

class FixEntry :
	public FixThread
{
public:
	static IMsg* getPriceEngineInstance(IMsg *priceSource);
	static IMsg* getDealerEngineInstance(IMsg *dealer);
public:
	virtual ~FixEntry(void);
	
protected:
	bool OnMsg(TRawMsg *msg, IMsg *remote_src);
	BOOL Prepare(long *pnResult);
	BOOL Cleanup(long *pnResult);

	long m_msgSeqNum;
	long m_RemoteSeq;
private:
	FixEntry();
	// **** cfg *********
	void ReadIniConfig();
	int m_cfgPort;
	char m_cfgIP[16];
	char m_cfgSenderCompID[64];
	char m_cfgTargetCompID[64];
	char m_cfgaccount[21];
	// ******************
	void Init();
	void SetPeer(IMsg* pTarget) { m_pTarget = pTarget; }
	static FixEntry* pStaticEngine;

	bool m_hasLogon;
	FixThreadTimer  m_local_tm;
	IMsg* m_pTarget;
	IMsg* m_pPriceTarget;
	IMsg* m_pDealerTarget;
	FixTcpTrans m_tcp_trans;

	MDEntryTypeIDMap m_EntryTypeIDMap;

	void SendResendSeqMsg(int beginNo, int endNo);
	void FixMsgDeal(string sbody);
	void DealLogOnACKMsg(LISTFIXMSG listFix);
	void DealLogOutACKMsg(LISTFIXMSG listFix);
	void DealRejectMsg(LISTFIXMSG listFix);
	void DealMDACKMsg(LISTFIXMSG listFix);
	void DealHBMsg(LISTFIXMSG listFix);
	void DealTRMsg(LISTFIXMSG listFix);
	void DealMDRejectMsg(LISTFIXMSG listFix);
	void DealCancelReject(LISTFIXMSG listFix);
	void DealResendSeq(LISTFIXMSG listFix);
	void DealGrapFill(LISTFIXMSG listFix);

	void DealOrderReport(LISTFIXMSG listFix);

	int FindMDEntryType(string sEntryID);
	void SendMsgToGW(string smsg);

	void initOrderReport(TOrderRespMsg &tOrderResp);
	void initCancelReject(TCancelRejectMsg &tCancelReject);

	void getMDResptime(string stime, U16 &year, U8 &month, U8 &day, U8 &hour, U8 &min, U8 &sec);
};

