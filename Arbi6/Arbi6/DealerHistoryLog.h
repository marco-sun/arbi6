// DealerHistoryLog.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEALERHISTORYLOG_H__2242222A_2356_4DB7_B5EA_61B7D306F0FA__INCLUDED_)
#define AFX_DEALERHISTORYLOG_H__2242222A_2356_4DB7_B5EA_61B7D306F0FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DealerHistoryLog  
{
public:
	DealerHistoryLog();
	virtual ~DealerHistoryLog();

	BOOL SetDealerLog(CString log);
	CString GetDealerLog();
protected:
	void GetFileName(CString &filename);
};

#endif // !defined(AFX_DEALERHISTORYLOG_H__2242222A_2356_4DB7_B5EA_61B7D306F0FA__INCLUDED_)
