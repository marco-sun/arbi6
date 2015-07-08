#if !defined PATSAPIMSGDEF_H
#define PATSAPIMSGDEF_H
#include "FixThreadMsgPool.h"


//**************************************************************
// ��Ϣ��				��Ϣ����					״̬		����
// ��¼������Ϣ			MSGTYPE_PATSAPI_LOGON		����		��
// ��¼�����Ϣ			MSGTYPE_PATSAPI_LOGON_RESP	����		��
// �۸�����Ϣ			MSGTYPE_PATSAPI_MD			����		��
// �۸�仯֪ͨ			MSGTYPE_PATSAPI_MD_SRESP	����		��
// �۸���ʧ��			MSGTYPE_PATSAPI_MD_REJECT	����		��
// �µ�������Ϣ			MSGTYPE_PATSAPI_AO			����		��
// ����״̬֪ͨ			MSGTYPE_PATSAPI_OR			����		��
// �����ɽ�֪ͨ			MSGTYPE_PATSAPI_FR			����		��
// ����������Ϣ			MSGTYPE_PATSAPI_CO			����		��
// �˵�������Ϣ			MSGTYPE_PATSAPI_LOGOUT		����		��
// �˵ǽ����Ϣ			MSGTYPE_PATSAPI_LOGOUT_RESP	����		��
// ����״̬֪ͨ			MSGTYPE_PATSAPI_ENGINE_STATUS	����	��
// ������״̬֪ͨ		TPatsApiHostAllmsgMsg		����		��
//**************************************************************

#pragma pack(1)

// ***************����Ϊengine���û������Ϣ����****************
#define MSGTYPE_PATSAPI_BASE	(0x00006000)
#define MSGSTRING_MAX_LEN	2400

// �ýӿ��д���Ϣ�ɺ��ԣ�engine�������ӽ������Զ���¼
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

// engineͨ������Ϣ���û��෵�ص�¼���
#define MSGTYPE_PATSAPI_LOGON_RESP	(MSGTYPE_PATSAPI_BASE+2)
struct TPatsApiLogonRespMsg : public TRawMsg
{
	U8 FailCode; // 0-OK, 
	I8 Reason[61]; //
};

// �û���ͨ������Ϣ���ľ����Լ�ļ۸���Ϣ
#define MSGTYPE_PATSAPI_MD	(MSGTYPE_PATSAPI_BASE+3)
struct TPatsApiMDReqMsg : public TRawMsg
{
	U8  requestType;	//0 - subscribe, other - unsubscribe
	U8  method;						//0 - simple(default, only bid,offer,LastTrade), or 1 - full depth
	I8	exchangeName[11]; // CME
	I8  contractName[11]; // Crude Oil			
	I8  MaturityMonthYear[51]; // SEP07		
};

// engineͨ������Ϣ���û���֪ͨ�۸�仯(simple��)
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

// engineͨ������Ϣ���û���֪ͨ�۸���ʧ����
#define MSGTYPE_PATSAPI_MD_REJECT	(MSGTYPE_PATSAPI_BASE+5)
struct TPatsApiMDRejectMsg : public TRawMsg
{
	I8	exchangeName[11];
	I8  contractName[11];			
	I8  MaturityMonthYear[51];	

	U8 FailCode;
	I8 Reason[64]; 
};

// �û���ͨ������Ϣ�µ�
#define MSGTYPE_PATSAPI_AO	(MSGTYPE_PATSAPI_BASE+6)
struct TPatsApiAddOrderMsg : public TRawMsg
{
	I8	traderAccount[21]; //�ÿմ�����engineʹ�õ����ʺ�Ĭ�ϰ󶨵ĵ�һ�������ʺ�
	I8	userOrderID[64]; // �û���ı�����ţ�����ʹ������ʱ����ϱ����������ַ���
	I8	exchangeName[11]; 
	I8  contractName[11];			
	I8  MaturityMonthYear[51];
	I8	OpenClose;				// 'O' for Open and 'C' for Close
	I8	BuySell;				//  'B' for Buy and 'S' for Sell
	U16	OrderQty;					
	double Price;
};

// engineͨ������Ϣ���û���㱨������״̬�仯
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

// engineͨ������Ϣ���û���㱨�����ɽ���Ϣ
#define MSGTYPE_PATSAPI_FR	(MSGTYPE_PATSAPI_BASE+8)
struct TPatsApiFillRptMsg : public TRawMsg
{
	I8	userOrderID[64];
	I8	exchOrderID[31];  // �������������
	U8	fillType;	// 0 - normal fill, 1 - external fill, 2 - netted fill
	U32	filledLots;
	double	filledPrice;
	I8 dateFilled[9]; //CCYYMMDD
	I8 timeFilled[7]; //HHMMSS
};

// �û���ͨ������Ϣ�´ﳷ������
#define MSGTYPE_PATSAPI_CO	(MSGTYPE_PATSAPI_BASE+9)
struct TPatsApiCancelOrderMsg : public TRawMsg
{
	I8	userOrderID[64];
};

// �û���ͨ������Ϣ�´��˵�����
#define MSGTYPE_PATSAPI_LOGOUT	(MSGTYPE_PATSAPI_BASE+10)
struct TPatsApiLogoutMsg : public TRawMsg
{
	I8	logoutType; // 0 - partial logout, 1 - total logout
};

// engineͨ������Ϣ���û��㱨�˵ǽ��
#define MSGTYPE_PATSAPI_LOGOUT_RESP	(MSGTYPE_PATSAPI_BASE+11)
struct TPatsApiLogoutRespMsg : public TRawMsg
{
	I8	code; // 0 - partial logout OK, 1 - total logout OK, 2 - Force logout, 3 - already logout.
	I8	reason[64]; //reserved.  
};

// engineͨ������Ϣ���û��㱨����״̬
#define MSGTYPE_PATSAPI_ENGINE_STATUS	(MSGTYPE_PATSAPI_BASE+12)
struct TPatsApiEngineStatusMsg : public TRawMsg
{
	I32	status;
};

// engineͨ������Ϣ���û��㱨������֪ͨ
#define MSGTYPE_PATSAPI_ALLMSG	(MSGTYPE_PATSAPI_BASE+13)
struct TPatsApiHostAllmsgMsg : public TRawMsg
{
	I8	content[512];
};

// �û���ͨ������Ϣ��������Լ�ĵ�ǰ�۸���Ϣ
#define MSGTYPE_PATSAPI_CP	(MSGTYPE_PATSAPI_BASE+14)
struct TPatsApiCPReqMsg : public TRawMsg
{
	I8	exchangeName[11]; // CME
	I8  contractName[11]; // Crude Oil			
	I8  MaturityMonthYear[51]; // SEP07		
};

#pragma pack()

#endif 