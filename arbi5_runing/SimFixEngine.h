#pragma once
#include "fixthread.h"
#include <list>
using namespace std;

class SimFixEngine :
	public FixThread
{
public:
	static IMsg* getPriceEngineInstance(IMsg *priceSource);
	static IMsg* getDealerEngineInstance(IMsg *dealer);
public:
	virtual ~SimFixEngine(void);
protected:
	bool OnMsg(TRawMsg *msg, IMsg *remote_src);
	BOOL Prepare(long *pnResult);
	BOOL Cleanup(long *pnResult);
private:
	SimFixEngine(void);
	void ReadIniConfig();
	void issuePrice(string MDId);
	FixThreadTimer  m_local_tm;
	FixThreadTimer  local_tm;
	IMsg* pDealer;
	IMsg* pPriceSource;
	static SimFixEngine* pStaticEngine;

	list<string> contracts;
};

