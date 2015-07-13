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

	/// <summary>用户钩子函数.</summary>
	typedef void (TS_Notify)(void* arg);

	/// <summary>用户钩子函数.</summary>
	typedef TS_Notify* TS_Notify_t;

	/// <summary>CATSHANDLE.</summary>
	typedef void* CATSHANDLE;

	/************************************************************************/
	/* API Functions                                                        */
	/************************************************************************/

	/// <summary>获取默认配置文件(catsapi.ini)的配置信息(配置信息由用户任意指定).</summary>
	///
	/// <param name="sect">   The sect.</param>
	/// <param name="name">   The name.</param>
	/// <param name="def_val">The def value.</param>
	///
	/// <returns>The int def.</returns>

	int WINAPI get_int_def(const char* sect, const char *name, int def_val);

	/// <summary>获取默认配置文件(catsapi.ini)的配置信息(配置信息由用户任意指定).</summary>
	///
	/// <param name="sect">   The sect.</param>
	/// <param name="name">   The name.</param>
	/// <param name="def_val">true to def value.</param>
	///
	/// <returns>The bool def.</returns>

	bool WINAPI get_bool_def(const char* sect, const char *name, bool def_val);

	/// <summary>获取默认配置文件(catsapi.ini)的配置信息(配置信息由用户任意指定).</summary>
	///
	/// <param name="sect">   The sect.</param>
	/// <param name="name">   The name.</param>
	/// <param name="def_val">The def value.</param>
	///
	/// <summary>获取默认配置文件(catsapi.ini)的配置信息(配置信息由用户任意指定).</summary>

	float WINAPI get_float_def(const char* sect, const char *name, float def_val);

	/// <summary>获取默认配置文件(catsapi.ini)的配置信息(配置信息由用户任意指定).</summary>
	///
	/// <param name="sect">   The sect.</param>
	/// <param name="name">   The name.</param>
	/// <param name="buf">   [out]The buffer.</param>
	/// <param name="bufLen">The buffer length.</param>
	/// <param name="def_val">The def value.</param>
	///
	/// <returns>0 if success else fail.</returns>

	int WINAPI get_string_def(const char* sect, const char *name, const char *def_val,char *buf,int bufLen);

	/// <summary>客户端日志工具，记录debug信息</summary>
	///
	/// <param name="fmt">Describes the format to use.</param>
	///
	/// <returns>.</returns>

	void WINAPI logdebug(const char* fmt, ...);

	/// <summary>客户端日志工具，记录warn信息</summary>
	///
	/// <param name="fmt">Describes the format to use.</param>
	///
	/// <returns>.</returns>

	void WINAPI logwarn(const char* fmt, ...);

	/// <summary>客户端日志工具，记录error信息</summary>
	///
	/// <param name="fmt">Describes the format to use.</param>
	///
	/// <returns>.</returns>

	void WINAPI logerror(const char* fmt, ...);

	/// <summary>客户端日志工具，记录info信息</summary>
	///
	/// <param name="fmt">Describes the format to use.</param>
	///
	/// <returns>.</returns>

	void WINAPI loginfo(const char* fmt, ...);

	/// <summary>获取CATSAPI版本信息.</summary>
	///
	/// <returns>CATSAPI版本信息.</returns>

	const char* WINAPI CATSAPI_GetVersion();

	/// <summary>初始化CATSAPI库.</summary>
	///
	/// <param name="debug_console">The debug console.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_Init(int debug_console);

	/// <summary>退出清除CATSAPI库.</summary>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_Fini();

	/// <summary>初始创建CATS会话.</summary>
	///
	/// <param name="hHandle">					  [in,out] If non-null, the handle.</param>
	/// <param name="use_ssl">					  The use ssl.</param>
	/// <param name="start_conn">				  The start connection.</param>
	/// <param name="OnReConnectedAfterUSerLogin">The on re connected after u ser login.</param>
	/// <param name="arg">						  [in,out] If non-null, the argument.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_InitSession(CATSHANDLE* hHandle, int use_ssl, int start_conn, TS_Notify_t OnTrdReConnectedAfterUSerLogin, void* arg_TrdRecon, TS_Notify_t OnTrdDisConnectedAfterUSerLogin=NULL, void* arg_TrdDiscon=NULL, TS_Notify_t OnHqReConnectedAfterUSerLogin = NULL, void* arg_HqRecon = NULL, TS_Notify_t OnHqDisConnectedAfterUSerLogin=NULL, void* arg_HqDiscon=NULL);

	/// <summary>退出清理CATS会话.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_ExitSession(CATSHANDLE hHandle);

	// 取服务器时间
	// int WINAPI CATSAPI_QueryServerDate(CATSHANDLE hHandle, int* date);

	/// <summary>获取最近一次的系统错误.</summary>
	///
	/// <param name="hHandle"> The handle.</param>
	/// <param name="errorMsg">[in,out] If non-null, message describing the error.</param>
	/// <param name="buffLen"> Length of the buffer.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_GetLastError(CATSHANDLE hHandle, char* errorMsg, size_t buffLen);

	/// <summary>设置业务接口的输入参数.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="name">   [in,out] If non-null, the name.</param>
	/// <param name="szValue">[in,out] If non-null, the value.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_SetParam(CATSHANDLE hHandle, const char* name, const char* szValue);


	/// <summary>设置业务接口的输入参数组.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="szValue">If non-null, the value.</param>
	///
	/// <returns>成功：0, 失败: -1.</returns>
	///
	/// <example>szValue示例: "field1=1001;field2=abc"</example>

	int WINAPI CATSAPI_SetGroupParam(CATSHANDLE hHandle, const char* szValue);

	/// <summary>设置业务接口的输入参数组.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="szValue">If non-null, the value.</param>
	/// <param name="len">If non-null, char* array size.</param>
	///
	/// <returns>成功：0, 失败: -1.</returns>
	///

	int WINAPI CATSAPI_SetGroupParamByIdx(CATSHANDLE hHandle, const char* szValue[],const size_t len);

	/// <summary>获取下一条数据.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_GetNext(CATSHANDLE hHandle);


	/// <summary>获取二进制数据.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="nBufLen">Buffer length.</param>
	///
	/// <returns>.</returns>

	char* WINAPI CATSAPI_GetBuffer(CATSHANDLE hHandle, int *pBufLen);

	/// <summary>获取业务接口输出参数.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="name">   [in,out] If non-null, the name.</param>
	/// <param name="szValue">[in,out] If non-null, the value.</param>
	/// <param name="buffLen">Buffer长度(最长2048字节).</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_GetCStrField(CATSHANDLE hHandle, const char* name, char* szValue, size_t buffLen);

	/// <summary>获取业务接口输出参数.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="name">   If non-null, the name.</param>
	/// <param name="nValue"> [in,out] If non-null, the value.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_GetIntField(CATSHANDLE hHandle, const char* name, int& nValue);

	/// <summary>获取业务接口输出参数.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="name">   If non-null, the name.</param>
	/// <param name="nValue"> [in,out] If non-null, the value.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_GetInt64Field(CATSHANDLE hHandle, const char* name, Int64& nValue);

	/// <summary>获取业务接口输出参数.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="name">   If non-null, the name.</param>
	/// <param name="lValue"> [in,out] If non-null, the value.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_GetLongField(CATSHANDLE hHandle, const char* name, long& lValue);

	/// <summary>获取业务接口输出参数.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="name">   If non-null, the name.</param>
	/// <param name="dValue"> [in,out] If non-null, the value.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_GetFloatField(CATSHANDLE hHandle, const char* name, double& dValue);

	//////////////////////////////////////////////////////////////////////////
	//	客户端内存数据库专用函数
	//  开发者可任意创建自己的内存表并使用SQL访问
	//////////////////////////////////////////////////////////////////////////

	/// <summary>执行批量的SQL.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="szSQL">  The SQL.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_MDB_ExecBatchSQL(CATSHANDLE hHandle, const char *szSQL);

	// 执行无结果集的SQL
	int WINAPI CATSAPI_MDB_ExecDML(CATSHANDLE hHandle, const char * FormatString, ...);

	/// <summary>执行统计计算SQL并返回第一列整数值.</summary>
	///
	/// <param name="hHandle">	   The handle.</param>
	/// <param name="FormatString">The format string.</param>
	///
	/// <returns>.</returns>
	///
	/// <example>SQL示例: select count(*) from mytable 返回mytable的记录条数</example>

	int WINAPI CATSAPI_MDB_ExecScalar(CATSHANDLE hHandle, const char * FormatString, ...);

	/// <summary>执行一条INSERT语句,返回受影响行数.</summary>
	///
	/// <param name="hHandle">	   The handle.</param>
	/// <param name="FormatString">The format string.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_MDB_Insert(CATSHANDLE hHandle, const char * FormatString, ...);

	/// <summary>执行一条UPDATE语句,返回受影响行数.</summary>
	///
	/// <param name="hHandle">	   The handle.</param>
	/// <param name="FormatString">The format string.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_MDB_Update(CATSHANDLE hHandle, const char * FormatString, ...);

	/// <summary>执行一条DELETE语句,返回受影响行数.</summary>
	///
	/// <param name="hHandle">	   The handle.</param>
	/// <param name="FormatString">The format string.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_MDB_Delete(CATSHANDLE hHandle, const char * FormatString, ...);

	/// <summary>执行一条Select语句.</summary>
	///
	/// <param name="hHandle">	   The handle.</param>
	/// <param name="FormatString">The format string.</param>
	///
	/// <returns>.</returns>

	bool WINAPI CATSAPI_MDB_Select(CATSHANDLE hHandle, const char *FormatString, ...);

	/// <summary>移动光标取下一条,执行完查询需移动光标到第一条记录.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	///
	/// <returns>true:获取行成功,false:记录已取完.</returns>

	bool WINAPI CATSAPI_MDB_MoveNext(CATSHANDLE hHandle);

	/// <summary>获取列数.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_MDB_GetColCount(CATSHANDLE hHandle);

	/// <summary>获取列名.</summary>
	///
	/// <param name="hHandle"> The handle.</param>
	/// <param name="col">	   The col.</param>
	/// <param name="pColName">[in,out] If non-null, name of the col.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_MDB_GetColName(CATSHANDLE hHandle, int col, char* pColName);

	/// <summary>获取列类型.</summary>
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

	/// <summary>获取列类型名.</summary>
	///
	/// <param name="hHandle">	   The handle.</param>
	/// <param name="col">		   The col.</param>
	/// <param name="pColTypeName">[in,out] If non-null, name of the col type.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_MDB_GetColTypeName(CATSHANDLE hHandle, int col, char* pColTypeName);

	/// <summary>获取列大小.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="nCol">   The col.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_MDB_GetColSize(CATSHANDLE hHandle, int nCol);

	/// <summary>根据列名，获取列数据.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="name">   The name.</param>
	/// <param name="val">	  [in,out] If non-null, the value.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_MDB_GetColInt(CATSHANDLE hHandle, const char *name, int* val);

	/// <summary>根据列名，获取列数据.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="name">   The name.</param>
	/// <param name="val">	  [in,out] If non-null, the value.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_MDB_GetColDbl(CATSHANDLE hHandle, const char *name, double* val);

	/// <summary>根据列名，获取列数据.</summary>
	///
	/// <param name="hHandle">The handle.</param>
	/// <param name="name">   The name.</param>
	/// <param name="val">	  [in,out] If non-null, the value.</param>
	/// <param name="len">	  The length.</param>
	///
	/// <returns>.</returns>

	int WINAPI CATSAPI_MDB_GetColCStr(CATSHANDLE hHandle, const char *name, char *val, size_t len);
	// int WINAPI CATSAPI_MDB_GetColInt64(CATSHANDLE hHandle, const char *name, int64* val);

	/// <summary>根据列名，获取列数据.</summary>
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
