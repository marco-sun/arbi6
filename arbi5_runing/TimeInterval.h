// TimeInterval.h: interface for the TimeInterval class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEINTERVAL_H__09015C5A_2392_49CE_A2EF_4313CEF9839C__INCLUDED_)
#define AFX_TIMEINTERVAL_H__09015C5A_2392_49CE_A2EF_4313CEF9839C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TimeInterval  
{
public:
	TimeInterval();
	virtual ~TimeInterval();
private:
	long from;
	long to;

};

#endif // !defined(AFX_TIMEINTERVAL_H__09015C5A_2392_49CE_A2EF_4313CEF9839C__INCLUDED_)
