/************************************************************************/
/* @library		SpecialEventStruct                                      */
/* @file		SpecialEventStruct.h									 */
/*              SpecialEventStruct.h, v 1.0.0, 2013/05/09 12:00:00      */
/* @description															 */
/* @author		yangjunfeng <junfeng.0228@gmail.com>			         */
/************************************************************************/

#ifndef _MARKET_DATA_DEFINITION_H
#define _MARKET_DATA_DEFINITION_H

#include "IntTypes.h"

#pragma pack(push)

/* ��1�ֽڶ��� */
#pragma pack (1)

typedef struct tagRealMarketData {
	char symbol[32];			//��Ʒ���루ÿ�ζ�����
	int32 date;  				//YYYYMMDD
	int32 time;  				//HHMMSSmmm ����ʱ��
	
	unsigned char stopMark;  	//ͣ�Ʊ�־ (1 ͣ��)��ÿ�ζ�����
	int32 cjbs;  				//������� ����ɽ�����
	char futureCode[8]; 		//�ڻ����� �ڻ�����Ʒ�֣���AL IF CU...
	char stkName[16];  			//��Ʊ����

	uint32 lastPrice;  			//��ǰ�۸񣨡�10000��
	uint32 openPrice;  			//���̼ۣ���10000��
	uint32 highPrice;  			//��߼ۣ���10000��
	uint32 lowPrice;  			//��ͼۣ���10000��
	uint32 prevClosePrice;		//�������̣���10000��
	uint32 closePrice;  		//�������̣���10000��
	uint32 highLimited; 		//��ͣ��ۣ���10000��
	uint32 lowLimited; 			//��ͣ��ۣ���10000��
	uint32 settlement; 			//�ڻ����� �����ۣ���10000��
	uint32 prevSettlement; 		//�ڻ����� ǰ����ۣ���10000��

	uint32 bidPrice1;  			//�����1����10000��
	uint32 askPrice1;  			//������1����10000��
	uint32 bidPrice2;  			//�����2����10000��
	uint32 askPrice2;  			//������2����10000��
	uint32 bidPrice3;  			//�����3����10000��
	uint32 askPrice3;  			//������3����10000��
	uint32 bidPrice4;  			//�����4����10000��
	uint32 askPrice4;  			//������4����10000��
	uint32 bidPrice5;  			//�����5����10000��
	uint32 askPrice5;  			//������5����10000��
	uint32 bidPrice6;  			//�����6����10000��
	uint32 askPrice6;  			//������6����10000��
	uint32 bidPrice7;  			//�����7����10000��
	uint32 askPrice7;  			//������7����10000��
	uint32 bidPrice8;  			//�����8����10000��
	uint32 askPrice8;  			//������8����10000��
	uint32 bidPrice9;  			//�����9����10000��
	uint32 askPrice9;  			//������9����10000��
	uint32 bidPrice10;  		//�����10����10000��
	uint32 askPrice10;  		//������10����10000��

	int64 turnOver;  			//�ܳɽ����
	uint32 peRatio1;  			//������� ������ӯ��1����10000��
	uint32 peRatio2;  			//������� ������ӯ��2����10000��
	uint32 prevDelta;  			//�ڻ����� ����ʵ�ȣ���10000��
	uint32 curDelta;  			//�ڻ����� ����ʵ�ȣ���10000��
	uint32 openInterest; 		//�ڻ����� �ֲ���
	uint32 prevOpenInterest;	//�ڻ����� ǰ�ֲ���
	int64 volume;  				//�ܳɽ���

	uint32 bidVol1;  			//������1
	uint32 askVol1;  			//������1
	uint32 bidVol2;  			//������2
	uint32 askVol2;  			//������2
	uint32 bidVol3;  			//������3
	uint32 askVol3;  			//������3
	uint32 bidVol4;  			//������4
	uint32 askVol4;  			//������4
	uint32 bidVol5;  			//������5
	uint32 askVol5;  			//������5
	uint32 bidVol6;  			//������6
	uint32 askVol6;  			//������6
	uint32 bidVol7;  			//������7
	uint32 askVol7;  			//������7
	uint32 bidVol8;  			//������8
	uint32 askVol8;  			//������8
	uint32 bidVol9;  			//������9
	uint32 askVol9;  			//������9
	uint32 bidVol10;  			//������10
	uint32 askVol10;  			//������10

	uint32 curVol;				//����
	int32 IOPV;					//IOPVԤ����ֵ����10000��
	int32 yieldToMaturity;		//���������ʣ���10000��
} STRealMarketData;
typedef struct tagRealMarketData STRealMarketData_Depth;

typedef struct tagRealMarketData_Tick
{
	char	symbol[32];		//��Ʒ���루ÿ�ζ�����
	int32	date;  			//YYYYMMDD
	int32 	time;			//�ɽ�ʱ��(HHMMSSmmm)
	uint32 	index;			//�ɽ����
	uint32	price;			//�ɽ��۸�
	uint32 	volume;			//�ɽ�����
	uint32	turnover;		//�ɽ����
} STRealMarketData_Tick;

typedef struct tagRealMarketData_OrderQueue
{
	char	symbol[32];		//��Ʒ���루ÿ�ζ�����
	int32	date;  			//YYYYMMDD
	int32 	time;			//�������(HHMMSSmmm)
	char    side;			//��������('1':Bid '2':Ask)
	uint32	price;			//�ɽ��۸�
	uint32 	orderCnt;		//��������
	uint32 	abItems;		//��ϸ����
	uint32 	abVolume[200];	//������ϸ
} STRealMarketData_OrderQueue;

typedef struct tagRealMarketData_IndexData
{
	char	symbol[32];		//��Ʒ���루ÿ�ζ�����
	int32	date;  			//YYYYMMDD
	int32   time;			//ʱ��(HHMMSSmmm)
	int32	openIndex;		//����ָ��
	int32 	highIndex;		//���ָ��
	int32 	lowIndex;		//���ָ��
	int32 	lastIndex;		//����ָ��
	int64	totalVolume;	//���������Ӧָ���Ľ�������
	int64	turnover;		//���������Ӧָ���ĳɽ����
	int32	preCloseIndex;	//ǰ��ָ��
} STRealMarketData_IndexData;

typedef struct tagDerivedMarketData_PriceListItem
{
	char	symbol[32];
	int32	date;
	int32	time;
	uint32	price;
	uint32	volume;
	double	bidRate;
} STDerivedMarketData_PriceListItem;

typedef struct tagDerivedMarketData_ZBCJ
{
	char	symbol[32];
	int32	date;
	int32	time;
	uint32	price;
	uint32	volume;
	int32	prop;
	int32	increPosition;
} STDerivedMarketData_ZBCJ;

#pragma pack(pop)

#endif /* _MARKET_DATA_DEFINITION_H */