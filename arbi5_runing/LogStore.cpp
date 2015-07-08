// LogStore.cpp: implementation of the CLogStore class.
//
//////////////////////////////////////////////////////////////////////

#include "LogStore.h"


#define DEFAULT_LOG_LEVEL		telDebug  //also can be telLog or telAlarm
#define DEFAULT_STORE_PATH		"."
#define DEFAULT_FILE_PREFIX		"log_"
#define DEFAULT_FILE_POSTFIX	".log"
#define DEFAULT_SHOW_SCREEN		FALSE  //if true, writing file along with console output
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CLogStore g_LogStore;


IDebugTrace::~IDebugTrace()
{
}

CLogStore::CLogStore()
{
		m_show_screen = DEFAULT_SHOW_SCREEN;
		m_log_level = DEFAULT_LOG_LEVEL;
		m_store_path = DEFAULT_STORE_PATH;
		m_file_prefix = DEFAULT_FILE_PREFIX;
		m_file_postfix = DEFAULT_FILE_POSTFIX;
		InitializeCriticalSection(&cs);

}

CLogStore::~CLogStore()
{
	DeleteCriticalSection(&cs);
	if (m_stream.IsInited())
		m_stream.Uninit();
}

IOutputStream* CLogStore::GetStore()
{
	char buff[20];
	if (!m_stream.IsInited())
	{
		SYSTEMTIME t;
		GetLocalTime( &t );
		sprintf_s(buff, 20, "%04d%02d%02d_%02d%02d%02d", 
				t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
		m_file_name = m_store_path + "\\" + m_file_prefix + buff + m_file_postfix;
		TOSFileHandle hFile;
		//fopen_s(&hFile, m_file_name.c_str(), "w");
		hFile = _fsopen( m_file_name.c_str(), "w",_SH_DENYWR);
		m_stream.Init(hFile);
	}
	return &m_stream;
}

void CLogStore::TraceEvent(ETraceEventLevel level, const char *event)
{
	EnterCriticalSection(&cs);
	if (level >= m_log_level)
	{
		if (level == telLog)
			StoreEvent("LOG", level, event);
		else if (level == telAlarm)
			StoreEvent("ALM", level, event);
		else //debug
			StoreEvent("DBG", level, event);
	}
	LeaveCriticalSection(&cs);
	
}

BOOL CLogStore::StoreEvent(const char *session, ETraceEventLevel level, const char *event)
{
	IOutputStream *pStream = GetStore();
	if (pStream)
	{
		SYSTEMTIME t;
		GetLocalTime( &t );
		if (m_show_screen)
		{
			printf("[%s] [%d-%02d-%02d %02d:%02d:%02d.%03d] %s\n", session, 
				t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds,
				event);
		}
		int len = pStream->Printf("[%s] [%d-%02d-%02d %02d:%02d:%02d.%03d] %s\n", session, 
			t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds,
			event);
	}
	return TRUE;
}

void CLogStore::SetShowScreen(BOOL show_screen)
{
	m_show_screen = show_screen;
}
BOOL CLogStore::InitStore(ETraceEventLevel log_level, 
			   const char *store_path, const char *file_prefix, const char *file_postfix)
{
	m_log_level = log_level;
	
	m_store_path = store_path;
	if (m_store_path.size() == 0)
		m_store_path = ".";
	
	m_file_prefix = file_prefix;
	
	m_file_postfix = file_postfix;
	
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
//	trace helper class

_CTracer::_CTracer(ETraceEventLevel level, const char *file/*=NULL*/, int line/*=0*/)
{
	m_level = level;
	m_file = file;
	m_line = line;
	if (m_file)
	{
		const char *p = m_file;
		while (*p != '\0')
		{
			if ((*p == '/') || (*p == '\\'))
				m_file = p + 1;
			p ++;
		}
	}
}

_CTracer::~_CTracer()
{
}

void _CTracer::Trace(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);

	char buff[DEBUG_LOG_MAX_TRACE_LEN + 1];

	int len = _vsnprintf_s(buff, (DEBUG_LOG_MAX_TRACE_LEN + 1), DEBUG_LOG_MAX_TRACE_LEN, format, ap);
	if (len >= 0)
	{
		if (m_file && (DEBUG_LOG_MAX_TRACE_LEN > len))
			len = len + _snprintf_s(buff + len, (DEBUG_LOG_MAX_TRACE_LEN + 1)-len, DEBUG_LOG_MAX_TRACE_LEN - len, " <%s:%d>", m_file, m_line);

		buff[len] = '\0';

		g_LogStore.TraceEvent(m_level, buff);
	}

	va_end(ap);
}


////////////////////////////////////////////////////////////////////////////////
//	platform independent functions
int OSWriteFile(TOSFileHandle file, const void *buff, int len)
{
	int n, l = len;
	unsigned char *p = (unsigned char*)buff;

	while (l > 0)
	{
		n = fwrite(p,1,l,file);
		if (n <= 0)
		{
			return -1;
		}

		p += n;
		l -= n;
	}
	fflush(file);
	return len;
}

TOSFileHandle OSOpenFile(char *filepath, char *type)
{
	TOSFileHandle ret;
	//fopen_s(&ret, filepath, type);
	ret = _fsopen( filepath, type,_SH_DENYWR);
	return ret;
}

BOOL OSCloseFile(TOSFileHandle file)
{
	fclose(file);
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//	interface IOutputStream


int IOutputStream::Printf(const char *szFormat, ...)
{
	va_list ap;
	va_start(ap, szFormat);

	char buff[DEBUG_LOG_MAX_TRACE_LEN];
	int len = _vsnprintf_s(buff, DEBUG_LOG_MAX_TRACE_LEN, sizeof(buff), szFormat, ap);
	if (len > 0)
		Write(buff, len);

	va_end(ap);
	return len;
}

IOutputStream::IOutputStream()
{
}

IOutputStream::~IOutputStream()
{
}
////////////////////////////////////////////////////////////////////////////////
//	class CFileIOStream

CFileIOStream::CFileIOStream(TOSFileHandle file/*=INVALID_FILE_HANDLE*/)
{
	m_File = INVALID_FILE_HANDLE;
	if (file != INVALID_FILE_HANDLE)
		Init(file);
}

CFileIOStream::~CFileIOStream()
{
	Uninit();
}

BOOL CFileIOStream::Init(TOSFileHandle file)
{
	if (m_File == INVALID_FILE_HANDLE)
	{
		m_File = file;
		return TRUE;
	}
	return FALSE;
}

BOOL CFileIOStream::Uninit()
{
	if (m_File != INVALID_FILE_HANDLE)
	{
		OSCloseFile(m_File);
		m_File = INVALID_FILE_HANDLE;
		return TRUE;
	}
	return FALSE;
}

BOOL CFileIOStream::IsInited()
{
	return (m_File != INVALID_FILE_HANDLE);
}

int CFileIOStream::Write(const void *pBuff, int nLen/*=-1*/)
{
	if (nLen < 0)
		nLen = strlen((const char*)pBuff);
	return OSWriteFile(m_File, pBuff, nLen);
}

