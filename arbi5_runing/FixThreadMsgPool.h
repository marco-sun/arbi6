#pragma once
#include <windows.h> // for critical section

//*********basic type definition********
typedef int					BOOL;
typedef unsigned char		U8;
typedef char				I8;
typedef unsigned short		U16;
typedef short				I16;
typedef unsigned int		U32;
typedef int					I32;

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

// ѭ���������Ļ���ʵ��
extern "C" 
{
typedef struct _T_CYCLE_BUFFER_
{
	U8		*buff;
	long	size; // buff������ܳ��ȣ�ʵ������Ϊsize��1
	long	begin; // ָ���´ζ�ȡ�ĵ�һ��λ��
	long	end; // ָ���´�д��ĵ�һ��λ��
} TCycleBuffer;

// ��buff���������䳤��size��ʼ��ѭ���������ṹ
void CBInitialize(TCycleBuffer *pCB, void *buff, long size);
long CBGetUsedLen(const TCycleBuffer *pCB);
long CBGetFreeLen(const TCycleBuffer *pCB);
// ������������ʱ����ʧ��
BOOL CBWriteData(TCycleBuffer *pCB, const void *data, long len);
// ������������ʱ����ʧ��
BOOL CBReadData(TCycleBuffer *pCB, void *data, long len);

// ������Ϣ����
#pragma pack(1)
struct TRawMsg
{
	long __spare;
	U32	type;		//message type
	I32	len;			//parameter length, <= 0 for no parameter
	U8	_param[1];	//buffer of parameter
} ;
#pragma pack()

#define INIT_MSG_HEADER(msg) if (1) {(msg)->__spare=1;(msg)->type=0;(msg)->len=sizeof(*(msg))-sizeof(TRawMsg);}
#define MSG_SPARE_LEN (sizeof(long))
#define MSG_HEADER(msg) (((U8*)(msg))+MSG_SPARE_LEN)
#define MSG_HEADER_LEN (sizeof(TRawMsg)-MSG_SPARE_LEN)
#define MSG_PARAM(msg) ((U8*)((msg)->_param+1))
#define MSG_PARAM_LEN(msg) (((msg)->len>0)?(msg)->len:0)
#define MSG_DATA(msg) MSG_HEADER(msg)
#define MSG_DATA_LEN(msg) (MSG_PARAM_LEN(msg)+MSG_HEADER_LEN)
#define MSG_TOTAL_LEN(msg) (MSG_PARAM_LEN(msg)+sizeof(TRawMsg))

TRawMsg* AssembleMsg(void* buffer, int buffer_size, U32 type, I32 len, const void *param);

} // end of extern "C"

// ����Fix�߳�������Ϣ���еĶ�д����
class FixThreadLock
{
	CRITICAL_SECTION cs;
public:
	FixThreadLock();
	virtual ~FixThreadLock();

	BOOL Lock();
	BOOL Unlock();
};
// Fix�첽��д������,֧�ֶ���ߺͶ�д��
class FixCycleBuffer
{
protected:
	TCycleBuffer m_CB;
	BOOL m_bOwnMemory;

	FixThreadLock *m_pWriteLock;
	FixThreadLock *m_pReadLock;

public:
	FixCycleBuffer();
	FixCycleBuffer(long nSize, void *pBuff = NULL, 
		BOOL bParellelWrite = FALSE, BOOL bParellelRead = FALSE);
	virtual ~FixCycleBuffer();

	BOOL Init(long nSize, void *pBuff = NULL, 
		BOOL bParellelWrite = FALSE, BOOL bParellelRead = FALSE);
	BOOL Uninit();

	BOOL IsInited();

	long GetUsedLen();
	long GetFreeLen();
	BOOL WriteData(const void *data, long len);
	BOOL ReadData(void *data, long len);
};


class FixThreadMsgPool : public FixCycleBuffer
{
public:
	FixThreadMsgPool(void);
	virtual ~FixThreadMsgPool(void);
	BOOL Init(long nSize, void *pBuff = NULL, 
		BOOL bParellelWrite = FALSE, BOOL bParellelRead = FALSE);

	BOOL ReadMsg(TRawMsg *msg, I32 buff_size);
	BOOL WriteMsg(TRawMsg *msg);
};

