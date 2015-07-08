#include "FixThreadMsgPool.h"
#include <string.h>

//////////////////////////////////////////////////////////////////////
//	TCycleBuffer & functions


void CBInitialize(TCycleBuffer *pCB, void *buff, long size)
{
	memset(pCB, 0, sizeof(TCycleBuffer));
	pCB->buff = (U8*)buff;
	pCB->size = size;
}

long CBGetUsedLen(const TCycleBuffer *pCB)
{
	long l = pCB->end - pCB->begin;
	return (l >=0) ? l : (pCB->size + l);
}

long CBGetFreeLen(const TCycleBuffer *pCB)
{
	long l = pCB->begin - pCB->end;
	return (l > 0) ? (l - 1) : (pCB->size + l - 1);
}

BOOL CBWriteData(TCycleBuffer *pCB, const void *data, long len)
{
	long b = pCB->begin;
	long e = pCB->end;
	long s = pCB->size;
	long l1, l2;

	if (b > e)
	{
		l1 = b - e - 1;
		l2 = 0;
	}
	else
	{ // 0 <= b <= e <= s, 
		l1 = s - e;
		l2 = b - 1;
	}
	// »º³å¿ÕÓà³¤¶È²»×ã£¬·µ»ØÊ§°Ü
	if ((l1 + l2) < len)
		return FALSE;

	if (l1 > len)
		l1 = len;
	l2 = len - l1;

	if (l1 > 0)
		memcpy(pCB->buff + e, data, l1);
	if (l2 > 0)
		memcpy(pCB->buff, ((U8*)data) + l1, l2);

	pCB->end = (e + len) % s;

	return TRUE;
}

BOOL CBReadData(TCycleBuffer *pCB, void *data, long len)
{
	long b = pCB->begin;
	long e = pCB->end;
	long s = pCB->size;
	long l1, l2;

	if (e >= b)
	{
		l1 = e - b;
		l2 = 0;
	}
	else
	{
		l1 = s - b;
		l2 = e;
	}
	// »º³å¿É¶Á³¤¶È²»×ã£¬·µ»ØÊ§°Ü
	if ((l1 + l2) < len)
		return FALSE;

	if (l1 > len)
		l1 = len;
	l2 = len - l1;

	if ((l1 > 0) && data)
		memcpy(data, pCB->buff + b, l1);
	if ((l2 > 0) && data)
		memcpy(((U8*)data) + l1, pCB->buff, l2);

	pCB->begin= (b + len) % s;

	return TRUE;
}





FixThreadLock::FixThreadLock()
{
	InitializeCriticalSection(&cs);
}

FixThreadLock::~FixThreadLock()
{
	DeleteCriticalSection(&cs);
}

BOOL FixThreadLock::Lock()
{
	EnterCriticalSection(&cs);
	return TRUE;
}

BOOL FixThreadLock::Unlock()
{
	LeaveCriticalSection(&cs);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////
//	class CCycleBuffer

FixCycleBuffer::FixCycleBuffer()
{
	memset(&m_CB, 0, sizeof(m_CB));
	m_bOwnMemory = FALSE;
	m_pWriteLock = NULL;
	m_pReadLock = NULL;
}

FixCycleBuffer::FixCycleBuffer(long nSize, void *pBuff/*=NULL*/, 
	BOOL bParellelWrite/*=FALSE*/, BOOL bParellelRead/*=FALSE*/)
{
	Init(nSize, pBuff, bParellelWrite, bParellelRead);
}

FixCycleBuffer::~FixCycleBuffer()
{
	Uninit();
}

BOOL FixCycleBuffer::Init(long nSize, void *pBuff/*=NULL*/, 
	BOOL bParellelWrite/*=FALSE*/, BOOL bParellelRead/*=FALSE*/)
{
	memset(&m_CB, 0, sizeof(m_CB));
	if (pBuff)
	{
		m_CB.buff = (U8*)pBuff;
		m_bOwnMemory = FALSE;
	}
	else
	{
		m_CB.buff = new U8 [nSize];
		m_bOwnMemory = TRUE;
	}
	m_CB.size = nSize;

	if (bParellelWrite)
		m_pWriteLock = new FixThreadLock();
	if (bParellelRead)
		m_pReadLock = new FixThreadLock();
	return TRUE;
}

BOOL FixCycleBuffer::Uninit()
{
	if (m_bOwnMemory && m_CB.buff)
		delete [] m_CB.buff;
	memset(&m_CB, 0, sizeof(m_CB));
	m_bOwnMemory = FALSE;

	if (m_pWriteLock)
		delete m_pWriteLock;
	m_pWriteLock = NULL;

	if (m_pReadLock)
		delete m_pReadLock;
	m_pReadLock = NULL;

	return TRUE;
}

BOOL FixCycleBuffer::IsInited()
{
	return (m_CB.buff != NULL);
}

long FixCycleBuffer::GetUsedLen()
{
	return CBGetUsedLen(&m_CB);
}

long FixCycleBuffer::GetFreeLen()
{
	return CBGetFreeLen(&m_CB);
}

BOOL FixCycleBuffer::WriteData(const void *data, long len)
{
	if (m_pWriteLock)
		m_pWriteLock->Lock();

	BOOL bResult = CBWriteData(&m_CB, data, len);

	if (m_pWriteLock)
		m_pWriteLock->Unlock();
	return bResult;
}

BOOL FixCycleBuffer::ReadData(void *data, long len)
{
	if (m_pReadLock)
		m_pReadLock->Lock();

	BOOL bResult = CBReadData(&m_CB, data, len);

	if (m_pReadLock)
		m_pReadLock->Unlock();
	return bResult;
}

TRawMsg* AssembleMsg(void* buffer, int buffer_size, U32 type, I32 len, const void *param)
{
	if ((len>0?len:0) + sizeof(TRawMsg) > buffer_size)
		return NULL;

	TRawMsg *msg = (TRawMsg*)buffer;
	msg->__spare = 1;
	msg->type = type;
	msg->len = len;
	if ((len > 0) && param)
		memcpy(MSG_PARAM(msg), param, len);
	return msg;
}


FixThreadMsgPool::FixThreadMsgPool(void)
{
}


FixThreadMsgPool::~FixThreadMsgPool(void)
{
}

BOOL FixThreadMsgPool::Init(long nSize, void *pBuff/*=NULL*/, 
		BOOL bParellelWrite/*=FALSE*/, BOOL bParellelRead/*=FALSE*/)
{
	return FixCycleBuffer::Init(nSize, pBuff, bParellelWrite, bParellelRead);
}

BOOL FixThreadMsgPool::WriteMsg(TRawMsg *msg)
{
	return WriteData(MSG_DATA(msg), MSG_DATA_LEN(msg));
}

BOOL FixThreadMsgPool::ReadMsg(TRawMsg *msg, I32 buff_size)
{
	if (m_pReadLock)
		m_pReadLock->Lock();

	I32 len = buff_size - sizeof(TRawMsg);

	//read header
	BOOL bResult = ReadData(MSG_HEADER(msg), MSG_HEADER_LEN);

	if (bResult)
	{
		if (msg->len > 0)
		{
			// read parameter
			if (msg->len <= len)
				bResult = ReadData(MSG_PARAM(msg), msg->len);
			else
			{
				ReadData(NULL, msg->len);
				bResult = FALSE;
			}
		}
	}

	if (m_pReadLock)
		m_pReadLock->Unlock();
	return bResult;
}
