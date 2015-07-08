// Rate.h: interface for the Rate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RATE_H__52E80C78_BC7F_460C_B1F3_DB555A0E3D07__INCLUDED_)
#define AFX_RATE_H__52E80C78_BC7F_460C_B1F3_DB555A0E3D07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)

#include <map>
#include <string>
using namespace std;

typedef map<string, double> RateMap; 

class Rate  
{
public:
	Rate();
	virtual ~Rate();

	static void setValue(string currency, double value);
	static double getValue(string currency);
	static RateMap rates;

};

#endif // !defined(AFX_RATE_H__52E80C78_BC7F_460C_B1F3_DB555A0E3D07__INCLUDED_)
