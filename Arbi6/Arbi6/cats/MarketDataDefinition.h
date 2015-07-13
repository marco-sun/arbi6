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

/* 按1字节对齐 */
#pragma pack (1)

typedef struct tagRealMarketData {
	char symbol[32];			//商品代码（每次都传）
	int32 date;  				//YYYYMMDD
	int32 time;  				//HHMMSSmmm 行情时间
	
	unsigned char stopMark;  	//停牌标志 (1 停牌)（每次都传）
	int32 cjbs;  				//深交所发布 行情成交笔数
	char futureCode[8]; 		//期货行情 期货交易品种，如AL IF CU...
	char stkName[16];  			//股票名称

	uint32 lastPrice;  			//当前价格（×10000）
	uint32 openPrice;  			//开盘价（×10000）
	uint32 highPrice;  			//最高价（×10000）
	uint32 lowPrice;  			//最低价（×10000）
	uint32 prevClosePrice;		//昨日收盘（×10000）
	uint32 closePrice;  		//今日收盘（×10000）
	uint32 highLimited; 		//涨停板价（×10000）
	uint32 lowLimited; 			//跌停板价（×10000）
	uint32 settlement; 			//期货行情 今结算价（×10000）
	uint32 prevSettlement; 		//期货行情 前结算价（×10000）

	uint32 bidPrice1;  			//申买价1（×10000）
	uint32 askPrice1;  			//申卖价1（×10000）
	uint32 bidPrice2;  			//申买价2（×10000）
	uint32 askPrice2;  			//申卖价2（×10000）
	uint32 bidPrice3;  			//申买价3（×10000）
	uint32 askPrice3;  			//申卖价3（×10000）
	uint32 bidPrice4;  			//申买价4（×10000）
	uint32 askPrice4;  			//申卖价4（×10000）
	uint32 bidPrice5;  			//申买价5（×10000）
	uint32 askPrice5;  			//申卖价5（×10000）
	uint32 bidPrice6;  			//申买价6（×10000）
	uint32 askPrice6;  			//申卖价6（×10000）
	uint32 bidPrice7;  			//申买价7（×10000）
	uint32 askPrice7;  			//申卖价7（×10000）
	uint32 bidPrice8;  			//申买价8（×10000）
	uint32 askPrice8;  			//申卖价8（×10000）
	uint32 bidPrice9;  			//申买价9（×10000）
	uint32 askPrice9;  			//申卖价9（×10000）
	uint32 bidPrice10;  		//申买价10（×10000）
	uint32 askPrice10;  		//申卖价10（×10000）

	int64 turnOver;  			//总成交金额
	uint32 peRatio1;  			//深交所发布 行情市盈率1（×10000）
	uint32 peRatio2;  			//深交所发布 行情市盈率2（×10000）
	uint32 prevDelta;  			//期货行情 昨虚实度（×10000）
	uint32 curDelta;  			//期货行情 今虚实度（×10000）
	uint32 openInterest; 		//期货行情 持仓量
	uint32 prevOpenInterest;	//期货行情 前持仓量
	int64 volume;  				//总成交量

	uint32 bidVol1;  			//申买量1
	uint32 askVol1;  			//申卖量1
	uint32 bidVol2;  			//申买量2
	uint32 askVol2;  			//申卖量2
	uint32 bidVol3;  			//申买量3
	uint32 askVol3;  			//申卖量3
	uint32 bidVol4;  			//申买量4
	uint32 askVol4;  			//申卖量4
	uint32 bidVol5;  			//申买量5
	uint32 askVol5;  			//申卖量5
	uint32 bidVol6;  			//申买量6
	uint32 askVol6;  			//申卖量6
	uint32 bidVol7;  			//申买量7
	uint32 askVol7;  			//申卖量7
	uint32 bidVol8;  			//申买量8
	uint32 askVol8;  			//申卖量8
	uint32 bidVol9;  			//申买量9
	uint32 askVol9;  			//申卖量9
	uint32 bidVol10;  			//申买量10
	uint32 askVol10;  			//申卖量10

	uint32 curVol;				//现量
	int32 IOPV;					//IOPV预估净值（×10000）
	int32 yieldToMaturity;		//到期收益率（×10000）
} STRealMarketData;
typedef struct tagRealMarketData STRealMarketData_Depth;

typedef struct tagRealMarketData_Tick
{
	char	symbol[32];		//商品代码（每次都传）
	int32	date;  			//YYYYMMDD
	int32 	time;			//成交时间(HHMMSSmmm)
	uint32 	index;			//成交编号
	uint32	price;			//成交价格
	uint32 	volume;			//成交数量
	uint32	turnover;		//成交金额
} STRealMarketData_Tick;

typedef struct tagRealMarketData_OrderQueue
{
	char	symbol[32];		//商品代码（每次都传）
	int32	date;  			//YYYYMMDD
	int32 	time;			//订单编号(HHMMSSmmm)
	char    side;			//买卖方向('1':Bid '2':Ask)
	uint32	price;			//成交价格
	uint32 	orderCnt;		//订单数量
	uint32 	abItems;		//明细个数
	uint32 	abVolume[200];	//订单明细
} STRealMarketData_OrderQueue;

typedef struct tagRealMarketData_IndexData
{
	char	symbol[32];		//商品代码（每次都传）
	int32	date;  			//YYYYMMDD
	int32   time;			//时间(HHMMSSmmm)
	int32	openIndex;		//今开盘指数
	int32 	highIndex;		//最高指数
	int32 	lowIndex;		//最低指数
	int32 	lastIndex;		//最新指数
	int64	totalVolume;	//参与计算相应指数的交易数量
	int64	turnover;		//参与计算相应指数的成交金额
	int32	preCloseIndex;	//前盘指数
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