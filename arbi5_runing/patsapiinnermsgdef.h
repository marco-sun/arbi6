#if !defined PATSAPIINNERMSGDEF_H
#define PATSAPIINNERMSGDEF_H
#include "FixThreadMsgPool.h"
#pragma pack(1)

// **************以下为engine内部消息定义**********************
#define MSGTYPE_PATSAPI_INNER_BASE	(0x00006100)

#define MSGTYPE_PATSAPI_INNER_DL	(MSGTYPE_PATSAPI_INNER_BASE+1)

#define MSGTYPE_PATSAPI_INNER_LOGON	(MSGTYPE_PATSAPI_INNER_BASE+2)

#define MSGTYPE_PATSAPI_INNER_HOSTLINK	(MSGTYPE_PATSAPI_INNER_BASE+3)
struct TPatsApiInnerHostMsg : public TRawMsg
{
	U8 status;
};

#define MSGTYPE_PATSAPI_INNER_MDUPD	(MSGTYPE_PATSAPI_INNER_BASE+4)
struct TPatsApiInnerMDUpdMsg : public TRawMsg
{
	I8	exchangeName[11];
	I8  contractName[11];			
	I8  MaturityMonthYear[51];	
};

#define MSGTYPE_PATSAPI_INNER_OR	(MSGTYPE_PATSAPI_INNER_BASE+5)
struct TPatsApiInnerORMsg : public TRawMsg
{
	I8 patsOrderID[11];
	I8 oldPatsOrderID[11];
};

#define MSGTYPE_PATSAPI_INNER_FR	(MSGTYPE_PATSAPI_INNER_BASE+6)
struct TPatsApiInnerFRMsg : public TRawMsg
{
	I8 patsOrderID[11];
	I8 patsFillID[71];
};

#define MSGTYPE_PATSAPI_INNER_MESSAGE	(MSGTYPE_PATSAPI_INNER_BASE+7)
struct TPatsApiInnerMsgMsg : public TRawMsg
{
	I8 patsMsgID[11];
};

#define MSGTYPE_PATSAPI_INNER_FORCELOGOUT	(MSGTYPE_PATSAPI_INNER_BASE+8)

#define MSGTYPE_PATSAPI_INNER_PRICELINK	(MSGTYPE_PATSAPI_INNER_BASE+9)
struct TPatsApiInnerPriceMsg : public TRawMsg
{
	U8 status;
};

#pragma pack()

#endif 