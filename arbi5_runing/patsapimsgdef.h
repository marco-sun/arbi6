#if !defined PATSAPIMSGDEF_H
#define PATSAPIMSGDEF_H
#include "FixThreadMsgPool.h"


//**************************************************************
// 消息名				消息定义					状态		方向
// 登录请求消息			MSGTYPE_PATSAPI_LOGON		废弃		入
// 登录结果消息			MSGTYPE_PATSAPI_LOGON_RESP	正常		出
// 价格订阅消息			MSGTYPE_PATSAPI_MD			正常		入
// 价格变化通知			MSGTYPE_PATSAPI_MD_SRESP	正常		出
// 价格订阅失败			MSGTYPE_PATSAPI_MD_REJECT	正常		出
// 下单请求消息			MSGTYPE_PATSAPI_AO			正常		入
// 报单状态通知			MSGTYPE_PATSAPI_OR			正常		出
// 报单成交通知			MSGTYPE_PATSAPI_FR			正常		出
// 撤单请求消息			MSGTYPE_PATSAPI_CO			正常		入
// 退登请求消息			MSGTYPE_PATSAPI_LOGOUT		正常		入
// 退登结果消息			MSGTYPE_PATSAPI_LOGOUT_RESP	正常		出
// 引擎状态通知			MSGTYPE_PATSAPI_ENGINE_STATUS	正常	出
// 服务器状态通知		TPatsApiHostAllmsgMsg		正常		出
//**************************************************************

#pragma pack(1)

// ***************以下为engine与用户侧的消息定义****************
#define MSGTYPE_PATSAPI_BASE	(0x00006000)
#define MSGSTRING_MAX_LEN	2400

// 该接口中此消息可忽略，engine会在连接建立后自动登录
#define MSGTYPE_PATSAPI_LOGON	(MSGTYPE_PATSAPI_BASE+1)
struct TPatsApiLogonMsg : public TRawMsg
{
	I8 hostIP[16];
	I8 priceIP[16];
	U16 hostPort;
	U16 pricePort;
	I8 uid[256];
	I8 pwd[256];
};

// engine通过该消息向用户侧返回登录结果
#define MSGTYPE_PATSAPI_LOGON_RESP	(MSGTYPE_PATSAPI_BASE+2)
struct TPatsApiLogonRespMsg : public TRawMsg
{
	U8 FailCode; // 0-OK, 
	I8 Reason[61]; //
};

// 用户侧通过该消息订阅具体合约的价格信息
#define MSGTYPE_PATSAPI_MD	(MSGTYPE_PATSAPI_BASE+3)
struct TPatsApiMDReqMsg : public TRawMsg
{
	U8  requestType;	//0 - subscribe, other - unsubscribe
	U8  method;						//0 - simple(default, only bid,offer,LastTrade), or 1 - full depth
	I8	exchangeName[11]; // CME
	I8  contractName[11]; // Crude Oil			
	I8  MaturityMonthYear[51]; // SEP07		
};

// engine通过该消息向用户侧通知价格变化(simple版)
#define MSGTYPE_PATSAPI_MD_SRESP	(MSGTYPE_PATSAPI_BASE+4)
struct TPatsApiMDSRespMsg : public TRawMsg
{
	I8	exchangeName[11];
	I8  contractName[11];			
	I8  MaturityMonthYear[51];		

	U16 year;
	U8	month;
	U8	day;
	U8	hour;
	U8	min;
	U8	sec;
	double bidprice;
	double askprice;
	double tradeprice;
};

// engine通过该消息向用户侧通知价格订阅失败了
#define MSGTYPE_PATSAPI_MD_REJECT	(MSGTYPE_PATSAPI_BASE+5)
struct TPatsApiMDRejectMsg : public TRawMsg
{
	I8	exchangeName[11];
	I8  contractName[11];			
	I8  MaturityMonthYear[51];	

	U8 FailCode;
	I8 Reason[64]; 
};

// 用户侧通过该消息下单
#define MSGTYPE_PATSAPI_AO	(MSGTYPE_PATSAPI_BASE+6)
struct TPatsApiAddOrderMsg : public TRawMsg
{
	I8	traderAccount[21]; //置空串，则engine使用登入帐号默认绑定的第一个交易帐号
	I8	userOrderID[64]; // 用户侧的报单编号，可以使用日期时间加上报单的特征字符等
	I8	exchangeName[11]; 
	I8  contractName[11];			
	I8  MaturityMonthYear[51];
	I8	OpenClose;				// 'O' for Open and 'C' for Close
	I8	BuySell;				//  'B' for Buy and 'S' for Sell
	U16	OrderQty;					
	double Price;
};

// engine通过该消息向用户侧汇报报单的状态变化
#define MSGTYPE_PATSAPI_OR	(MSGTYPE_PATSAPI_BASE+7)
struct TPatsApiOrderRespMsg : public TRawMsg
{
	I8	userOrderID[64];
	I8	 OrdStatus;          // 0 - rejected locally, 1 - working, 2 - rejected, 3 - partfilled, 4 - filled, 5 - cancelled, 6 - externalcancelled
	U32	 filledLots;
	U32  totalLots;	
	double	dstPrice;
	double  avgPrice;
};

// engine通过该消息向用户侧汇报报单成交消息
#define MSGTYPE_PATSAPI_FR	(MSGTYPE_PATSAPI_BASE+8)
struct TPatsApiFillRptMsg : public TRawMsg
{
	I8	userOrderID[64];
	I8	exchOrderID[31];  // 交易所报单编号
	U8	fillType;	// 0 - normal fill, 1 - external fill, 2 - netted fill
	U32	filledLots;
	double	filledPrice;
	I8 dateFilled[9]; //CCYYMMDD
	I8 timeFilled[7]; //HHMMSS
};

// 用户侧通过该消息下达撤单命令
#define MSGTYPE_PATSAPI_CO	(MSGTYPE_PATSAPI_BASE+9)
struct TPatsApiCancelOrderMsg : public TRawMsg
{
	I8	userOrderID[64];
};

// 用户侧通过该消息下达退登命令
#define MSGTYPE_PATSAPI_LOGOUT	(MSGTYPE_PATSAPI_BASE+10)
struct TPatsApiLogoutMsg : public TRawMsg
{
	I8	logoutType; // 0 - partial logout, 1 - total logout
};

// engine通过该消息向用户汇报退登结果
#define MSGTYPE_PATSAPI_LOGOUT_RESP	(MSGTYPE_PATSAPI_BASE+11)
struct TPatsApiLogoutRespMsg : public TRawMsg
{
	I8	code; // 0 - partial logout OK, 1 - total logout OK, 2 - Force logout, 3 - already logout.
	I8	reason[64]; //reserved.  
};

// engine通过该消息向用户汇报引擎状态
#define MSGTYPE_PATSAPI_ENGINE_STATUS	(MSGTYPE_PATSAPI_BASE+12)
struct TPatsApiEngineStatusMsg : public TRawMsg
{
	I32	status;
};

// engine通过该消息向用户汇报服务器通知
#define MSGTYPE_PATSAPI_ALLMSG	(MSGTYPE_PATSAPI_BASE+13)
struct TPatsApiHostAllmsgMsg : public TRawMsg
{
	I8	content[512];
};

// 用户侧通过该消息请求具体合约的当前价格信息
#define MSGTYPE_PATSAPI_CP	(MSGTYPE_PATSAPI_BASE+14)
struct TPatsApiCPReqMsg : public TRawMsg
{
	I8	exchangeName[11]; // CME
	I8  contractName[11]; // Crude Oil			
	I8  MaturityMonthYear[51]; // SEP07		
};

#pragma pack()

#endif 