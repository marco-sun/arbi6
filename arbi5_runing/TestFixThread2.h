#pragma once
#include "fixthread.h"
class TestFixThread2 :
	public FixThread
{
public:
	TestFixThread2(void);
	virtual ~TestFixThread2(void);
	void SetPeer(IMsg* pTarget) { m_pTarget = pTarget; }
protected:
	bool OnMsg(TRawMsg *msg, IMsg *remote_src);
	BOOL Prepare(long *pnResult);
	BOOL Cleanup(long *pnResult);
private:
	FixThreadTimer  m_local_tm;
	IMsg* m_pTarget;
};

