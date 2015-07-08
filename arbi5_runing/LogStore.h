// LogStore.h: interface for log facility.
//
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
///@module	��־���� 
///@platform win32,c++ 
///@file LogStore.h,LogStore.cpp
///@brief �ּ���־����־�ļ��洢, �̰߳�ȫ
///@history create_20100705
///@contactor ���	sun_dsk1@hotmail.com
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
///@usage	�������ļ����빤�̣���Ҫ��ӡ��־�Ĵ���ģ����Ҫincludeͷ�ļ���
///			ʹ�������������ӡ��־��
///			TRACE_ALARM("...",...); ��ӡ����������־
///			TRACE_LOG("...",...); ��ӡ��Ϣ������־
///			TRACE_DEBUG("...",...); ��ӡ���Լ�����־
///			�����꺯�����÷���printf���ƣ�����β���ü�\n��־��
///@example 
///			TRACE_DEBUG("This is debug. a=%d.",a);
///			TRACE_LOG("This is log. a=%d.",a);
///			TRACE_ALARM("This is alarm. a=%d.",a);
///
///@config  ����־ģ����Զ���־�ļ��Ĵ洢·����ǰ׺��׺����־�����������
///			������λ��cpp�ļ����ײ������£�
///			#define DEFAULT_LOG_LEVEL	telLog //�޶����𣬴�ӡlog������
///											   //������debug��־��
///											   //�˴���������telDebug
///											   //����telAlarm
///			#define DEFAULT_STORE_PATH	"."	   //��־�ļ�·������ǰĿ¼
///			#define DEFAULT_FILE_PREFIX	 "log_" //��־�ļ���ǰ׺
///			#define DEFAULT_FILE_POSTFIX ".log" //��־�ļ�����׺
///			#define DEFAULT_SHOW_SCREEN		TRUE //�Ƿ�ͬʱ�����console
///
///@output	��ָ��Ŀ¼������־�ļ�����Ϊ ǰ׺xxxxxxxx_xxxxxx��׺,
///			���У�xxxΪ��־ģ������ʱ�ı���ʱ�䣨������������ʱ��
///			ÿ����־һ�У���ʽΪ��
///			[����] [ʱ���] ��־���� <Դ���ļ���:�к�>
///
///@notes	������־�ܳ�������Ϊ512�ֽڣ�ʵ����־�����Կ�����400�ֽ���Ϊ��
///			�꺯���в��ü�\n���з�
///
///@bug		N/A
/////////////////////////////////////////////////////////////////////////
#if !defined(AFX_LOGSTORE_H__68EC03EF_5DA1_4904_B1CC_5A88FDC515B8__INCLUDED_)
#define AFX_LOGSTORE_H__68EC03EF_5DA1_4904_B1CC_5A88FDC515B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TRACE_ALARM	\
	_CTracer(telAlarm, TRACE_LOCATION_FILE, TRACE_LOCATION_LINE).Trace

#define TRACE_LOG	\
	_CTracer(telLog, TRACE_LOCATION_FILE, TRACE_LOCATION_LINE).Trace

#define TRACE_DEBUG	\
	_CTracer(telDebug, TRACE_LOCATION_FILE, TRACE_LOCATION_LINE).Trace

#pragma   warning   (disable:   4786) 
#include <string>
using namespace std;

//#ifdef _DEBUG
//#define ENABLE_TRACE_DEBUG	TRUE
#define TRACE_LOCATION_FILE	__FILE__
#define TRACE_LOCATION_LINE	__LINE__
//#else
//#define ENABLE_TRACE_DEBUG	FALSE
//#define TRACE_LOCATION_FILE	NULL
//#define TRACE_LOCATION_LINE	0
//#endif

//#define ENABLE_TRACE_LOG	TRUE
//#define ENABLE_TRACE_ALARM	TRUE

#define DEBUG_LOG_MAX_TRACE_LEN 512


extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
typedef int					BOOL;
#ifndef FALSE
#define FALSE               0
#endif
#ifndef TRUE
#define TRUE                1
#endif
typedef FILE* TOSFileHandle;
#define INVALID_FILE_HANDLE NULL
TOSFileHandle OSOpenFile(char *filepath, char *type);
int OSWriteFile(TOSFileHandle file, const void *buff, int len);
BOOL OSCloseFile(TOSFileHandle file);
}

#include <windows.h>

////////////////////////////////////////////////////////////////////////////////
//	interface IOutputStream

struct IOutputStream
{
	virtual int Write(const void *pBuff, int nLen = -1) = 0;

	int Printf(const char *szFormat, ...);

	virtual ~IOutputStream();

protected:
	IOutputStream();
};


////////////////////////////////////////////////////////////////////////////////
//	class CFileIOStream

class CFileIOStream : public IOutputStream
{
protected:
	TOSFileHandle m_File;

public:
	CFileIOStream(TOSFileHandle file = INVALID_FILE_HANDLE);
	virtual ~CFileIOStream();

	BOOL Init(TOSFileHandle file);
	BOOL Uninit();
	BOOL IsInited();

	virtual int Write(const void *pBuff, int nLen = -1);
};



////////////////////////////////////////////////////////////////////////////////
//	interface defination

enum ETraceEventLevel
{
	telDebug = 0,
	telLog,
	telAlarm
};

struct IDebugTrace
{
	virtual void TraceEvent(ETraceEventLevel level, const char *event) = 0;
	virtual ~IDebugTrace();
};


class CLogStore  : public IDebugTrace
{
protected:
	BOOL m_show_screen;
	CFileIOStream m_stream;
	string m_file_name;
	ETraceEventLevel m_log_level;
	string m_store_path;
	string m_file_prefix;
	string m_file_postfix;

	IOutputStream* GetStore();
	BOOL StoreEvent(const char *session, ETraceEventLevel level, const char *event);

public:

	void SetShowScreen(BOOL show_screen);
	virtual void TraceEvent(ETraceEventLevel level, const char *event);
	BOOL InitStore(ETraceEventLevel log_level, 
			const char *store_path, const char *file_prefix, const char *file_postfix);
	CLogStore();
	virtual ~CLogStore();

private:
	CRITICAL_SECTION cs;

};

////////////////////////////////////////////////////////////////////////////////
//	trace helper class

class _CTracer
{
	ETraceEventLevel m_level;
	const char *m_file;
	int m_line;
public:
	_CTracer(ETraceEventLevel level, const char *file = NULL, int line = 0);
	~_CTracer();

	void Trace(const char *format, ...);
};


#endif // !defined(AFX_LOGSTORE_H__68EC03EF_5DA1_4904_B1CC_5A88FDC515B8__INCLUDED_)
