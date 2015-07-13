// LogStore.h: interface for log facility.
//
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
///@module	日志代理 
///@platform win32,c++ 
///@file LogStore.h,LogStore.cpp
///@brief 分级日志，日志文件存储, 线程安全
///@history create_20100705
///@contactor 孙浩	sun_dsk1@hotmail.com
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
///@usage	将两个文件加入工程，需要打印日志的代码模块中要include头文件；
///			使用以下三个宏打印日志：
///			TRACE_ALARM("...",...); 打印警报级别日志
///			TRACE_LOG("...",...); 打印消息级别日志
///			TRACE_DEBUG("...",...); 打印调试级别日志
///			三个宏函数的用法与printf相似，但结尾不用加\n标志。
///@example 
///			TRACE_DEBUG("This is debug. a=%d.",a);
///			TRACE_LOG("This is log. a=%d.",a);
///			TRACE_ALARM("This is alarm. a=%d.",a);
///
///@config  该日志模块可以对日志文件的存储路径，前缀后缀，日志级别进行配置
///			配置项位于cpp文件的首部，如下：
///			#define DEFAULT_LOG_LEVEL	telLog //限定级别，打印log及以上
///											   //即屏蔽debug日志，
///											   //此处还可以是telDebug
///											   //或者telAlarm
///			#define DEFAULT_STORE_PATH	"."	   //日志文件路径，当前目录
///			#define DEFAULT_FILE_PREFIX	 "log_" //日志文件名前缀
///			#define DEFAULT_FILE_POSTFIX ".log" //日志文件名后缀
///			#define DEFAULT_SHOW_SCREEN		TRUE //是否同时输出到console
///
///@output	在指定目录生成日志文件，名为 前缀xxxxxxxx_xxxxxx后缀,
///			其中，xxx为日志模块启动时的本机时间（整个程序启动时）
///			每条日志一行，格式为：
///			[级别] [时间戳] 日志内容 <源码文件名:行号>
///
///@notes	单行日志总长度限制为512字节，实际日志内容以控制在400字节内为宜
///			宏函数中不用加\n换行符
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
