// Persistence.h: interface for the Persistence class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PERSISTENCE_H__FE233FA2_8B23_4075_9DB3_60EAAAA45C6E__INCLUDED_)
#define AFX_PERSISTENCE_H__FE233FA2_8B23_4075_9DB3_60EAAAA45C6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class Persistence  
{
public:
	Persistence();
	virtual ~Persistence();
	static void load();
	static void loadHistory();
	static void loadIntentions();
	static void saveIntentions();
    static void saveTwoLeg();
	static void loadTwoLegStrategy();
	static void saveDayTrading();
	static void loadDayTrading();
	static void loadImportExpression();
	static void loadAsNecessary();
	static bool isfirstLoad;
	static void loadOrderCount();
	static void loadKnownContract();
	static void MacAdress();
};

#endif // !defined(AFX_PERSISTENCE_H__FE233FA2_8B23_4075_9DB3_60EAAAA45C6E__INCLUDED_)
