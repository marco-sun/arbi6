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

// 循环缓冲区的基础实现
extern "C" 
{
typedef struct _T_CYCLE_BUFFER_
{
	U8		*buff;
	long	size; // buff缓冲的总长度，实际容量为size减1
	long	begin; // 指向下次读取的第一个位置
	long	end; // 指向下次写入的第一个位置
} TCycleBuffer;

// 用buff缓冲区和其长度size初始化循环缓冲区结构
void CBInitialize(TCycleBuffer *pCB, void *buff, long size);
long CBGetUsedLen(const TCycleBuffer *pCB);
long CBGetFreeLen(const TCycleBuffer *pCB);
// 缓冲容量不足时返回失败
BOOL CBWriteData(TCycleBuffer *pCB, const void *data, long len);
// 缓冲容量不足时返回失败
BOOL CBReadData(TCycleBuffer *pCB, void *data, long len);

// 基础消息类型
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

// 用于Fix线程所属消息队列的读写加锁
class FixThreadLock
{
	CRITICAL_SECTION cs;
public:
	FixThreadLock();
	virtual ~FixThreadLock();

	BOOL Lock();
	BOOL Unlock();
};
// Fix异步读写缓冲类,支持多读者和多写者
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

