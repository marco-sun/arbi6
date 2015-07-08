// DealerHistoryLog.cpp: set and get today dealer log
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DealerHistoryLog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DealerHistoryLog::DealerHistoryLog()
{

}

DealerHistoryLog::~DealerHistoryLog()
{

}

BOOL DealerHistoryLog::SetDealerLog(CString log)
{
	CStdioFile file;   
	CString strfilename;
	GetFileName(strfilename);
	if( !file.Open(strfilename, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite) )
	{
		return FALSE;
	} 
	file.SeekToEnd();
//	file.Write("\r\n",2);
	file.Write(log, log.GetLength());
	file.Close();
	return TRUE;
}

CString DealerHistoryLog::GetDealerLog()
{
	CStdioFile file;   
	CString strRet;
	CString strfilename;
	GetFileName(strfilename);
	if( !file.Open(strfilename, CFile::modeRead))
	{
		return "";
	} 
	CString line;
	while(file.ReadString(line))
	{
		strRet = line + "\n" + strRet;
	}
	return strRet;
}


void DealerHistoryLog::GetFileName(CString &filename)
{
	int nYear;
    int nMonth; 
    int nDay; 
	CString strTime;

	CTime tTime = CTime::GetCurrentTime();
	nDay   = tTime.GetDay();
    nMonth = tTime.GetMonth();
	nYear  = tTime.GetYear();

	char tmp[1024];
	const int pos=GetModuleFileName(NULL,tmp,1024);
	int nPos = pos;			//convert the const int into int!

	while (pos)
	{
		nPos--;
		if ( tmp[nPos] == '\\')
			break;
	}
	tmp[nPos] = '\0';
	filename= (CString)tmp;
	strTime.Format("\\%d%d%d.his", nYear, nMonth, nDay);
	filename += strTime;
}