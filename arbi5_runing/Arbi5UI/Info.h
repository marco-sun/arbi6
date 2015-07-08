#pragma once
#pragma warning(disable: 4786)
#include <list>
#include <map>
#include <string>
#include <vector>
using namespace std;
#include "..\order.h"
#include "..\DayTradingStrategy.h"
/*
enum STATE
{
	START,
	STOP
};*/
//Intention pair state
class IntentionPair;
class StrategyPair;
typedef struct tIPState
{	
	string sName; // 名称
	int state;    // 状态 0:初始, 1 开始, 2 停止
} TIPState;


typedef struct ttStrategyPrarm
{
	double diff;  //价差
	string compare; // >= or <= 
	enum BuySell overseaBuySell;
	enum BuySell domesticBuySell;
	int overseaVolume;
	int domesticVolume;
	int overseaAdd;
	int domesticAdd;
	int fishflag;//1 fish  2 no fish
	enum KaiPing overseaType;
	enum KaiPing domesticType;
}TStrategyPrarm;

typedef struct ttStrategyUI
{	
	char sName[32]; // 名称
	int firstLeg;
	int type; //1:only first 2 only second 3 double
	int maxPostions;
	int sgstate;    // 状态 enum STATE
	string sStrategy;
	string fistname;
	string secondname;
	TStrategyPrarm tStrategyPrarm[2];
} TStrategyUI;

typedef list<int> LISTSelectItem;
typedef list<TStrategyUI> LISTStrategyUI;
typedef map<string, TIPState*> IPStateMap; 

typedef struct ttStrategyInfo
{
//	IntentionPair *pIntentionPair;
	DayTradingStrategy *dts;
//	TwoLegStrategy *ts1;
//	TwoLegStrategy *ts2;

	TStrategyUI tStrategyUI;
} TStrategyInfo;

typedef list<TStrategyInfo*> StrategyInfoList;

typedef struct tIPInfo
{	
	string sName; //key
	string firstname;  //外盘名称
	string secondname; //内盘名称
	int	firstVol;	//第一盘（外盘）手数
	int secondVol;  //第二盘（内盘）手数
//	double dMaxDiff;
//	double dMinDiff;
//	double dSellMaxDiff;
//	double dSellMinDiff;
//	IntentionPair *pIntentionPair;
	StrategyInfoList strategyInfoList;
} TIPInfo;

typedef struct tIPUI
{
	string sName;
	string fistbuy;
	string fistsell;
	string buydiff;
	string kc;
	string lastPriceDiff;
	string selldiff;
	string pc;
	string secondbuy;
	string secondsell;
	string buyMaxdiff;
	string buyMindiff;
	string sellMaxdiff;
	string sellMindiff;
}TIPUI;

typedef std::vector<TIPUI> LISTIPUI;

typedef map<string, TIPInfo*> IPInfoMap; 

static void CopyStructStrategyUI(TStrategyUI *tOutStrategyUI, TStrategyUI tInStrategyUI)
{
	strcpy_s(tOutStrategyUI->sName, 32, tInStrategyUI.sName);
	tOutStrategyUI->firstLeg = tInStrategyUI.firstLeg;
	tOutStrategyUI->type = tInStrategyUI.type;
	tOutStrategyUI->maxPostions = tInStrategyUI.maxPostions;
	tOutStrategyUI->sgstate = tInStrategyUI.sgstate;
	tOutStrategyUI->sStrategy = tInStrategyUI.sStrategy;

	tOutStrategyUI->fistname = tInStrategyUI.fistname;
	tOutStrategyUI->secondname = tInStrategyUI.secondname;
	for (int i = 0; i<2; i++)
	{
		tOutStrategyUI->tStrategyPrarm[i].diff = tInStrategyUI.tStrategyPrarm[i].diff;
		tOutStrategyUI->tStrategyPrarm[i].compare = tInStrategyUI.tStrategyPrarm[i].compare;
		tOutStrategyUI->tStrategyPrarm[i].overseaBuySell = tInStrategyUI.tStrategyPrarm[i].overseaBuySell;
		tOutStrategyUI->tStrategyPrarm[i].domesticBuySell = tInStrategyUI.tStrategyPrarm[i].domesticBuySell;
		tOutStrategyUI->tStrategyPrarm[i].overseaVolume = tInStrategyUI.tStrategyPrarm[i].overseaVolume;
		tOutStrategyUI->tStrategyPrarm[i].domesticVolume = tInStrategyUI.tStrategyPrarm[i].domesticVolume;
		tOutStrategyUI->tStrategyPrarm[i].overseaAdd = tInStrategyUI.tStrategyPrarm[i].overseaAdd;
		tOutStrategyUI->tStrategyPrarm[i].domesticAdd = tInStrategyUI.tStrategyPrarm[i].domesticAdd;
		tOutStrategyUI->tStrategyPrarm[i].overseaType = tInStrategyUI.tStrategyPrarm[i].overseaType;
		tOutStrategyUI->tStrategyPrarm[i].domesticType = tInStrategyUI.tStrategyPrarm[i].domesticType;
		tOutStrategyUI->tStrategyPrarm[i].fishflag = tInStrategyUI.tStrategyPrarm[i].fishflag;
	}
}

static BOOL GetCUPID(CString &strID)
{
	unsigned long s1,s2;
	unsigned char vendor_id[]="------------------";
	CString str1,str2,str3;
	__asm
	{
		xor eax,eax
			cpuid
			mov dword ptr vendor_id,ebx
			mov dword ptr vendor_id[+4],edx
			mov dword ptr vendor_id[+8],ecx
	}
	str1.Format("%s",vendor_id);
	__asm  //取得CPU ID的高32位
	{
		mov eax,01h
			xor edx,edx
			cpuid
			mov s2,eax
	}
	str2.Format("%08X-",s2);
	
	__asm //取得CPU ID的低64位
	{
		mov eax,03h
			xor ecx,ecx
			xor edx,edx
			cpuid 
			mov s1,edx
			mov s2,ecx
	}
	
	str3.Format("%08X-%08X\n",s1,s2);
	str2+=str3;
	strID = str2;

	return TRUE;
}

static BOOL DecodeKey(CString strOldKey, CString &strDecode)
{
	int nkey[17];
	int nID[16];
	CString szTemp;
	char szEncode[10];
	memset(szEncode,0,10);

	nkey[0] = 1;
	nkey[1] = 9;
	nkey[2] = 7;
	nkey[3] = 8;
	nkey[4] = 0;
	nkey[5] = 8;
	nkey[6] = 0;
	nkey[7] = 1;

	nkey[9]  = 0;
	nkey[10] = 8;
	nkey[11] = 0;
	nkey[12] = 1;
	nkey[13] = 1;
	nkey[14] = 9;
	nkey[15] = 7;
	nkey[16] = 8;

    if (strOldKey.GetLength() < 26)
    {
		return FALSE;
    }
	int i;
	for (i = 0; i < 8; i++)
	{
		szTemp = strOldKey.Mid(i,1);
		if((strOldKey[i] >= '0') && (strOldKey[i] <= '9'))
		{
			nID[i] = atoi(szTemp);
			if (nID[i] < 10 )
			{
				if (nID[i] < nkey[i])
				{
					nID[i] = nID[i] + 10 - nkey[i];
				}
				else
				{
					nID[i] = nID[i] - nkey[i];
				}
				_itoa_s(nID[i],szEncode, 10,10);
				strDecode = strDecode + (CString)szEncode;
			}
		}
		else
		{
			strDecode = strDecode + szTemp;
		}
	}
	strDecode = strDecode + "-";
	for (i = 9; i < 17; i++)
	{
		szTemp = strOldKey.Mid(i,1);
		if((strOldKey[i] >= '0') && (strOldKey[i] <= '9')) 
		{
			nID[i] = atoi(szTemp);
			if (nID[i] < 10 )
			{
				nID[i] = nID[i] + nkey[i];
				if (nID[i] >= 10)
				{
					nID[i] = nID[i] - 10;
				}
				_itoa_s(nID[i],szEncode, 10,10);
				strDecode = strDecode + (CString)szEncode;
			}
		}
		else
		{
			strDecode = strDecode + szTemp;
		}
	}
	szTemp = strOldKey.Mid(18,8);
	strDecode = strDecode + "-" + szTemp;
	
	return TRUE;
}