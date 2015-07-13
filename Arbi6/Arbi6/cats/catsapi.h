//=============================================================================
/**
*  @library	catsapi.lib
*
*  @file	catsapi.h
*  
*  catsapi.h,v 1.00 2011/09/08 18:46:56 jackie_zhang Exp
*  
*  @description
*  
*  @author jackie_zhang <jackie_zhang@21cn.com>
*  @author 
*/
//=============================================================================

#ifdef   __cplusplus
extern "C" {
#endif

#ifndef _CATSAPI_H
#define _CATSAPI_H

#ifdef WIN32
#define Int64 __int64
#include <Windows.h>
#ifdef CATSAPI_EXPORTS
#define CATSAPI_API __declspec(dllexport)
#else
#define CATSAPI_API __declspec(dllimport)
#endif
#define CATSAPISTDCALL	__stdcall	/* ensure stcall calling convention on NT */
#else
#define CATSAPI_API
#define CATSAPISTDCALL				/* leave blank for other systems */
#define WINAPI 
#define Int64 long long
#endif

	/************************************************************************/
	/* new type definitions                                                 */
	/************************************************************************/

	/// <summary>�û����Ӻ���.</summary>
	typedef void (TS_Notify)(void* arg);

	/// <summary>�û����Ӻ���.</summary>
	typedef TS_Notify* TS_Notify_t;

	/// <summary>CATSHANDLE.</summary>
	typedef void* CATSHANDLE;

	/************************************************************************/
	/* API Functions                                                        */
	/************************************************************************/

	/// <summary>��ȡĬ�������ļ�(catsapi.ini)��������Ϣ(������Ϣ���û�����ָ��).</summary>
	///
	/// <param name="sect">   The sect.</param>
	/// <param name="name">   The name.</param>
	/// <param name="def_val">The def value.</param>
	///
	/// <returns>The int def.</returns>

	int WINAPI get_int_def(const char* sect, const char *name, int def_val);

	/// <summary>��ȡĬ�������ļ�(catsapi.ini)��������Ϣ(������Ϣ���û�����ָ��).</summary>
	///
	/// <param name="sect">   The sect.</param>
	/// <param name="name">   The name.</param>
	/// <param name="def_val">true to def value.</param>
	///
	/// <returns>The bool def.</returns>

	bool WINAPI get_bool_def(const char* sect, const char *name, bool def_val);

	/// <summary>��ȡĬ�������ļ�(catsapi.ini)��������Ϣ(������Ϣ���û�����ָ��).</summary>
	///
	/// <param name="sect">   The sect.</param>
	/// <param name="name">   The name.</param>
	/// <param name="def_val">The def value.</param>
	///
	/// <summary>��ȡĬ�������ļ�(catsapi.ini)��������Ϣ(������Ϣ���û�����ָ��).</summary>

	float WINAPI get_float_def(const char* sect, const char *name, float def_val);

	/// <summary>��ȡĬ�������ļ�(catsapi.ini)��������Ϣ(������Ϣ���û�����ָ��).</summary>
	///
	/// <param name="sect">   The sect.</param>
	/// <param name="name">   The name.</param>
	/// <param name="buf">   [out]The buffer.</param>
	/// <param name="bufLen">The buffer length.</param>
	/// <param name="def_val">The def value.</param>
	///
	/// <returns>0 if success else fail.</returns>

	int WINAPI get_string_def(const char* sect, const char *name, const char *def_val,char *buf,int bufLen);

	/// <summary>�ͻ�����־���ߣ���¼debug��Ϣ</summary>
	///
	/// <param name="fmt">Describes the format to use.</param>
	///
	/// <returns>.</returns>

	void WINAPI logdebug(const char* fmt, ...);

	/// <summary>�ͻ�����־���ߣ���¼warn��Ϣ</summary>
	///
	/// <param name="fmt">Describes the format to use.</param>
	///
	/// <returns>.</returns>

	void WINAPI logwarn(const char* fmt, ...);

	/// <summary>�ͻ�����־���ߣ���¼error��Ϣ</summary>
	///
	/// <param name="fmt">Describes the format to use.</param>
	///
	/// <returns>.</returns>

	void WINAPI logerror(const char* fmt, ...);

	/// <summary>�ͻ�����־���ߣ���¼info��Ϣ</summary>
	///
	/// <param name="fmt">Describes the format to use.</param>
	///
	/// <returns>.</returns>

	void WINAPI loginfo(const char* fmt, ...);

	/// <summary>��ȡCATSAPI�汾��Ϣ.</summary>
	///
	/// <returns>CATSAPI�汾��Ϣ.</returns>

	const char* WINAPI CATSAPI_GetVersion();

	/// <summary>��ʼ��CATSAPI��.</summary>
	///
	/// <param name="debug_console">The debug console.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_Init(int debug_console);

	/// <summary>�˳����CATSAPI��.</summary>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_Fini();

	/// <summary>��ʼ����CATS�Ự.</summary>
	///
	/// <param name="hHandle">					  [in,out] If non-null, the handle.</param>
	/// <param name="use_ssl">					  The use ssl.</param>
	/// <param name="start_conn">				  The start connection.</param>
	/// <param name="OnReConnectedAfterUSerLogin">The on re connected after u ser login.</param>
	/// <param name="arg">						  [in,out] If non-null, the argument.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_InitSession(CATSHANDLE* hHandle, int use_ssl, int start_conn, TS_Notify_t OnTrdReConnectedAfterUSerLogin, void* arg_TrdRecon, TS_Notify_t OnTrdDisConnectedAfterUSerLogin=NULL, void* arg_TrdDiscon=NULL, TS_Notify_t OnHqReConnectedAfterUSerLogin = NULL, void* arg_HqRecon = NULL, TS_Notify_t OnHqDisConnectedAfterUSerLogin=NULL, void* arg_HqDiscon=NULL);

	/// <summary>�˳�����CATS�Ự.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_ExitSession(CATSHANDLE hHandle);

	// ȡ������ʱ��
	// int WINAPI CATSAPI_QueryServerDate(CATSHANDLE hHandle, int* date);

	/// <summary>��ȡ���һ�ε�ϵͳ����.</summary>
	///
	/// <param name="hHandle"> The handle.</param>
	/// <param name="errorMsg">[in,out] If non-null, message describing the error.</param>
	/// <param name="buffLen"> Length of the buffer.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_GetLastError(CATSHANDLE hHandle, char* errorMsg, size_t buffLen);

	/// <summary>����ҵ��ӿڵ��������.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="name">   [in,out] If non-null, the name.</param>
	/// <param name="szValue">[in,out] If non-null, the value.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_SetParam(CATSHANDLE hHandle, const char* name, const char* szValue);


	/// <summary>����ҵ��ӿڵ����������.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="szValue">If non-null, the value.</param>
	///
	/// <returns>�ɹ���0, ʧ��: -1.</returns>
	///
	/// <example>szValueʾ��: "field1=1001;field2=abc"</example>

	int WINAPI CATSAPI_SetGroupParam(CATSHANDLE hHandle, const char* szValue);

	/// <summary>����ҵ��ӿڵ����������.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="szValue">If non-null, the value.</param>
	/// <param name="len">If non-null, char* array size.</param>
	///
	/// <returns>�ɹ���0, ʧ��: -1.</returns>
	///

	int WINAPI CATSAPI_SetGroupParamByIdx(CATSHANDLE hHandle, const char* szValue[],const size_t len);

	/// <summary>��ȡ��һ������.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_GetNext(CATSHANDLE hHandle);


	/// <summary>��ȡ����������.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="nBufLen">Buffer length.</param>
	///
	/// <returns>.</returns>

	char* WINAPI CATSAPI_GetBuffer(CATSHANDLE hHandle, int *pBufLen);

	/// <summary>��ȡҵ��ӿ��������.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="name">   [in,out] If non-null, the name.</param>
	/// <param name="szValue">[in,out] If non-null, the value.</param>
	/// <param name="buffLen">Buffer����(�2048�ֽ�).</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_GetCStrField(CATSHANDLE hHandle, const char* name, char* szValue, size_t buffLen);

	/// <summary>��ȡҵ��ӿ��������.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="name">   If non-null, the name.</param>
	/// <param name="nValue"> [in,out] If non-null, the value.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_GetIntField(CATSHANDLE hHandle, const char* name, int& nValue);

	/// <summary>��ȡҵ��ӿ��������.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="name">   If non-null, the name.</param>
	/// <param name="nValue"> [in,out] If non-null, the value.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_GetInt64Field(CATSHANDLE hHandle, const char* name, Int64& nValue);

	/// <summary>��ȡҵ��ӿ��������.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="name">   If non-null, the name.</param>
	/// <param name="lValue"> [in,out] If non-null, the value.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_GetLongField(CATSHANDLE hHandle, const char* name, long& lValue);

	/// <summary>��ȡҵ��ӿ��������.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="name">   If non-null, the name.</param>
	/// <param name="dValue"> [in,out] If non-null, the value.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_GetFloatField(CATSHANDLE hHandle, const char* name, double& dValue);

	//////////////////////////////////////////////////////////////////////////
	//	�ͻ����ڴ����ݿ�ר�ú���
	//  �����߿����ⴴ���Լ����ڴ��ʹ��SQL����
	//////////////////////////////////////////////////////////////////////////

	/// <summary>ִ��������SQL.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="szSQL">  The SQL.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_MDB_ExecBatchSQL(CATSHANDLE hHandle, const char *szSQL);

	// ִ���޽������SQL
	int WINAPI CATSAPI_MDB_ExecDML(CATSHANDLE hHandle, const char * FormatString, ...);

	/// <summary>ִ��ͳ�Ƽ���SQL�����ص�һ������ֵ.</summary>
	///
	/// <param name="hHandle">	   The handle.</param>
	/// <param name="FormatString">The format string.</param>
	///
	/// <returns>.</returns>
	///
	/// <example>SQLʾ��: select count(*) from mytable ����mytable�ļ�¼����</example>

	int WINAPI CATSAPI_MDB_ExecScalar(CATSHANDLE hHandle, const char * FormatString, ...);

	/// <summary>ִ��һ��INSERT���,������Ӱ������.</summary>
	///
	/// <param name="hHandle">	   The handle.</param>
	/// <param name="FormatString">The format string.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_MDB_Insert(CATSHANDLE hHandle, const char * FormatString, ...);

	/// <summary>ִ��һ��UPDATE���,������Ӱ������.</summary>
	///
	/// <param name="hHandle">	   The handle.</param>
	/// <param name="FormatString">The format string.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_MDB_Update(CATSHANDLE hHandle, const char * FormatString, ...);

	/// <summary>ִ��һ��DELETE���,������Ӱ������.</summary>
	///
	/// <param name="hHandle">	   The handle.</param>
	/// <param name="FormatString">The format string.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_MDB_Delete(CATSHANDLE hHandle, const char * FormatString, ...);

	/// <summary>ִ��һ��Select���.</summary>
	///
	/// <param name="hHandle">	   The handle.</param>
	/// <param name="FormatString">The format string.</param>
	///
	/// <returns>.</returns>

	bool WINAPI CATSAPI_MDB_Select(CATSHANDLE hHandle, const char *FormatString, ...);

	/// <summary>�ƶ����ȡ��һ��,ִ�����ѯ���ƶ���굽��һ����¼.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	///
	/// <returns>true:��ȡ�гɹ�,false:��¼��ȡ��.</returns>

	bool WINAPI CATSAPI_MDB_MoveNext(CATSHANDLE hHandle);

	/// <summary>��ȡ����.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_MDB_GetColCount(CATSHANDLE hHandle);

	/// <summary>��ȡ����.</summary>
	///
	/// <param name="hHandle"> The handle.</param>
	/// <param name="col">	   The col.</param>
	/// <param name="pColName">[in,out] If non-null, name of the col.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_MDB_GetColName(CATSHANDLE hHandle, int col, char* pColName);

	/// <summary>��ȡ������.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="col">	  The col.</param>
	///
	/// <returns>
	/// 	SQLITE_INTEGER	1
	/// 	SQLITE_FLOAT	2
	/// 	SQLITE_TEXT		3
	/// 	SQLITE_BLOB		4
	/// 	SQLITE_NULL		5
	/// </returns>

	int WINAPI CATSAPI_MDB_GetColType(CATSHANDLE hHandle, int col);

	/// <summary>��ȡ��������.</summary>
	///
	/// <param name="hHandle">	   The handle.</param>
	/// <param name="col">		   The col.</param>
	/// <param name="pColTypeName">[in,out] If non-null, name of the col type.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_MDB_GetColTypeName(CATSHANDLE hHandle, int col, char* pColTypeName);

	/// <summary>��ȡ�д�С.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="nCol">   The col.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_MDB_GetColSize(CATSHANDLE hHandle, int nCol);

	/// <summary>������������ȡ������.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="name">   The name.</param>
	/// <param name="val">	  [in,out] If non-null, the value.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_MDB_GetColInt(CATSHANDLE hHandle, const char *name, int* val);

	/// <summary>������������ȡ������.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="name">   The name.</param>
	/// <param name="val">	  [in,out] If non-null, the value.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_MDB_GetColDbl(CATSHANDLE hHandle, const char *name, double* val);

	/// <summary>������������ȡ������.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="name">   The name.</param>
	/// <param name="val">	  [in,out] If non-null, the value.</param>
	/// <param name="len">	  The length.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_MDB_GetColCStr(CATSHANDLE hHandle, const char *name, char *val, size_t len);
	// int WINAPI CATSAPI_MDB_GetColInt64(CATSHANDLE hHandle, const char *name, int64* val);

	/// <summary>������������ȡ������.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="name">   The name.</param>
	/// <param name="val">	  [in,out] If non-null, the value.</param>
	/// <param name="len">	  [in,out] If non-null, the length.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_MDB_GetColData(CATSHANDLE hHandle, const char *name, unsigned char *val, size_t* len);




# endif

#ifdef   __cplusplus
}
#endif
