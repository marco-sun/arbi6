#if !defined FIXMSGDEF_H
#define FIXMSGDEF_H
#include "FixThreadMsgPool.h"


#pragma pack(1)

#define MSGSTRING_MAX_LEN	2400

#define MSGSTRING_BEGINSTRING	"FIX.4.2"

#define MSGTYPE_FIXMSG_CONNECT  (0x00005000)
struct TFixConnect : public TRawMsg
{
	I8 connect[16];
	I8 sIp[32];
	U32 nPort;
};
#define MSGTYPE_FIXMSG_SEND     (0x00005001)
struct TFixMsg : public TRawMsg
{
	U32 RawDataLength;
	I8 RawData[MSGSTRING_MAX_LEN];
};
struct TFixReconnectMsg : public TRawMsg
{
	I8 reason[32];
};
#define MSGTYPE_FIXMSG_RECEIVE    (0x00005002)
#define MSGTYPE_FIXMSG_RECONNECT  (0x00005003)
#define MSGTYPE_FIXMSG_LOGON	(0x00001001)
struct TLogonMsg : public TRawMsg
{
	U8 EncryptMethod; // Set to 0
	U8 HeartBtInt; // Set to 30
	U32 RawDataLength;
	I8 RawData[64];
	I8 ResetSeqNumFlag;
};

#define MSGTYPE_FIXMSG_LOGON_RESP	(0x00001002)
struct TLogonRespMsg : public TRawMsg
{
	U8 FailCode; // 0-OK, 
	I8 Reason[64]; //
};

#define MSGTYPE_FIXMSG_MD	(0x00001003)
struct TMDReqMsg : public TRawMsg
{
	I8  MDReqID[26];				// 207+55+time
	U8  SubscriptionRequestType;	//0 ¡§C snapshot 1 ¡§C snapshot & subscribe 2 ¡§C unsubscribe
	U8  MarketDepth;				//Recommend 0 or 1, but > 1 accepted
	U8  MDUpdateType;				//0 (full refresh) or 1 (incremental)
	U8  NoMDEntryTypes;				//Number of MDEntryType fields requested Max:5
	U8  MDEntryType[10];				//Common use:0 (Bid), 1 (Offer) and 2 (Trade)
	I8  Symbol[11];					//Commodity symbol
	I8  SecurityType[4];			//FUT or OPT
	I8  MaturityMonthYear[7];		//Format CCYYMM, if omitted will subscribe to all months for instrument
	I8  SecurityExchange[10];		//Patsystems exchange name
};

#define MSGTYPE_FIXMSG_MD_RESP	(0x00001004)
struct TMDRespMsg : public TRawMsg
{
	I8  MDReqID[26];				// 207-55-CCYYMM-time
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

#define MSGTYPE_FIXMSG_MD_REJECT	(0x00001005)
struct TMDRejectMsg : public TRawMsg
{
	I8  MDReqID[26];				// 207+55+time
	U8 FailCode;
	I8 Reason[64]; 
};

#define MSGTYPE_FIXMSG_HB	(0x00001006)
struct THBMsg : public TRawMsg
{
	I8 TestReqID[64];
};

#define MSGTYPE_FIXMSG_NOS	(0x00001007)
struct TNewOrderSingleMsg : public TRawMsg
{
	I8	CIOrdID[26];
	I8  Symbol[11];					//Commodity symbol
	I8  SecurityType[4];			//FUT or OPT
	I8  MaturityMonthYear[7];		//Format CCYYMM, if omitted will subscribe to all months for instrument
	I8  SecurityExchange[10];		//Patsystems exchange name
	U8	OrdType;					// should be 2
	I8	OpenClose[2];				// "O" for Open and "C" for Close
	U8	Side;						//  1 for Buy and 2 for Sell
	U16	OrderQty;					
	double Price;
	I8	Text[64];
};

#define MSGTYPE_FIXMSG_OR	(0x00001008)
struct TOrderRespMsg : public TRawMsg
{
	I8	 CIOrdID[26];           //tag 11
	I8	 OrigCIOrdID[26];		// 41 (pre 11).
	I8	 OrdStatus[2];          //tag 39   0...9,A..E
	I8	 ExecType[2];           //tag 150  0...9,A..E
	U32  OrderID;				//tag 37
	I8	 SecondaryOrderID[32];	//tag 198

	U32	 LastShares;            //tag 32
	U32  LeavesQty;				//tag 151
	U32  CumQty;				//tag 14
	double	LastPx;				//tag 31
	double  AvgPx;				//tag 6

	U16 year;					//	TransactTime;		//tag 60
	U8	month;
	U8	day;
	U8	hour;
	U8	min;
	U8	sec;
	I8  Text[64];				//tag 58
};

#define MSGTYPE_FIXMSG_CANCEL		(0x00001009)
struct TOrderCancelMsg : public TRawMsg
{
	I8	OrigCIOrdID[26];			// 41 ClOrdID of the order to cancel.
	U32	OrderID ;					// 37 Matches OrderId on order
	I8	CIOrdID[26];				// 11 ClOrdId must be unique within a day
	I8  Symbol[11];					// 55 Commodity symbol
	I8  SecurityType[4];			// 167 FUT or OPT
	I8  MaturityMonthYear[7];		// 200 Format CCYYMM, if omitted will subscribe to all months for instrument
	I8  SecurityExchange[10];		// 207 Patsystems exchange name
	U8	Side;						// 54 1 for Buy and 2 for Sell 54
	U16	OrderQty;					// 38
};

#define MSGTYPE_FIXMSG_CANCELREJECT	(0x0000100a)
struct TCancelRejectMsg : public TRawMsg
{
	U32  	OrderID;		// tag 37
	I8	CIOrdID[26];           // tag 11
	I8	OrigCIOrdID[26];       // tag 41
	I8	OrdStatus[2];          //tag 39  0...9,A..E
	
	U16	year;			//	TransactTime;		//tag 60
	U8	month;
	U8	day;
	U8	hour;
	U8	min;
	U8	sec;
	
	U8	reason;			//tag 102
	I8	Text[64];		//tag 58
};
#define MSGTYPE_FIXMSG_RESET_SEQ (0x00001014)
struct TResetSeqMsg : public TRawMsg
{
	U32 SeqId;
};
#define MSGTYPE_FIXMSG_LOGOUT	(0x00001012)
struct TLogoutMsg : public TRawMsg
{
	I8 Text[64];
};
#define MSGTYPE_FIXMSG_LOGOUT_RESP	(0x00001013)
struct TLogoutRespMsg : public TRawMsg
{
	U8 FailCode; // 0-OK, 
	I8 Reason[64]; //
};
#define MSGTYPE_FIXMSG_RESEND_SEQ (0x00001015)
struct TResendSeqMsg : public TRawMsg
{
	U32 beginNo; 
	U32 endNo; 
};
#define MSGTYPE_FIXMSG_GRAPFILL (0x00001016)
struct TGrapFillMsg : public TRawMsg
{
	U32 newNo; //
};

#pragma pack()

#endif 