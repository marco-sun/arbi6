// Uuid.h: interface for the Uuid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UUID_H__A9467231_A305_4F21_A79E_6C8CC19D551D__INCLUDED_)
#define AFX_UUID_H__A9467231_A305_4F21_A79E_6C8CC19D551D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <string>
using namespace std;

class Uuid  
{
public:
	static string gen();

};

#endif // !defined(AFX_UUID_H__A9467231_A305_4F21_A79E_6C8CC19D551D__INCLUDED_)
