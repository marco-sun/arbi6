#pragma once
#include "Fixthread.h"
#include "FixClientSock.h"
class FixTcpTrans :
	public FixThread
{
public:
	FixTcpTrans(void);
	virtual ~FixTcpTrans(void);
	void SetPeer(IMsg* pTarget) { m_pTarget = pTarget; }

	FixTcpClient m_cTcpClinet;
	IMsg* m_pTarget;
protected:
	bool OnMsg(TRawMsg *msg, IMsg *remote_src);
	BOOL Prepare(long *pnResult);
	BOOL Cleanup(long *pnResult);
private:
	FixThreadTimer  m_local_tm;
	
};

