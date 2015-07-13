//=============================================================================
/**
 *  @library	catsapi.lib
 *
 *  @file	catsapi_acc.h
 *  
 *  catsapi_acc.h, v1.00 2012/11/8 wanghz
 *  
 *  @description
 *  
 *  @author wanghz
 */
//=============================================================================

#include "catsapi.h"

#ifdef   __cplusplus
extern "C" {
#endif

#ifndef _CATSAPI_ACC_H
#define _CATSAPI_ACC_H


//////////////////////////////////////////////////////////////////////////
/// Cats账户管理类
//////////////////////////////////////////////////////////////////////////


////////////////////
//Cats账户登录 
////////////////////

/// <summary>准备CATS连接请求.</summary>
int WINAPI CATSAPI_Prepare_CatsConnect(CATSHANDLE hHandle);

/// <summary>CATS连接.</summary>
int WINAPI CATSAPI_Execute_CatsConnect(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取CATS连接信息结果.</summary>
int WINAPI CATSAPI_Fetch_CatsConnectResult(CATSHANDLE hHandle);

/// <summary>准备CATS账户登录请求.</summary>
int WINAPI CATSAPI_Prepare_CatsLogin(CATSHANDLE hHandle);

/// <summary>CATS账户登录.</summary>
int WINAPI CATSAPI_Execute_CatsLogin(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取CATS账户信息结果.</summary>
int WINAPI CATSAPI_Fetch_CatsLoginResult(CATSHANDLE hHandle);

/// <summary>提取CATS子账户信息结果.</summary>
int WINAPI CATSAPI_Fetch_CatsSubAccResult(CATSHANDLE hHandle);


/// <summary>准备增加子账户请求.</summary>
int WINAPI CATSAPI_Prepare_AddSubAcc(CATSHANDLE hHandle);

/// <summary>增加子账户请求.</summary>
int WINAPI CATSAPI_Execute_AddSubAcc(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取增加子账户请求结果.</summary>
int WINAPI CATSAPI_Fetch_AddSubAccResult(CATSHANDLE hHandle);


/// <summary>准备删除子账户请求.</summary>
int WINAPI CATSAPI_Prepare_DelSubAcc(CATSHANDLE hHandle);

/// <summary>删除子账户请求.</summary>
int WINAPI CATSAPI_Execute_DelSubAcc(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取删除子账户请求结果.</summary>
int WINAPI CATSAPI_Fetch_DelSubAccResult(CATSHANDLE hHandle);


/// <summary>准备查询子账户信息请求.</summary>
int WINAPI CATSAPI_Prepare_QuerySubAcc(CATSHANDLE hHandle);

/// <summary>查询子账户信息请求.</summary>
int WINAPI CATSAPI_Execute_QuerySubAcc(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询子账户请求结果.</summary>
int WINAPI CATSAPI_Fetch_QuerySubAccResult(CATSHANDLE hHandle);

/// <summary>提取子账户结果.</summary>
int WINAPI CATSAPI_Fetch_QuerySubAccResult_SubAcc(CATSHANDLE hHandle);

//获取子账户密码
int WINAPI CATSAPI_GetSubAccPwd(CATSHANDLE hHandle,char *szCatsId,char *szSubAccType,char *szSubAccId,char *szValue,size_t valueLen);



/// <summary>准备修改CATS 账户密码.</summary>
int WINAPI CATSAPI_Prepare_UpdateCatsPwd(CATSHANDLE hHandle);

/// <summary>修改CATS账户密码.</summary>
int WINAPI CATSAPI_Execute_UpdateCatsPwd(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取修改CATS账户密码结果.</summary>
int WINAPI CATSAPI_Fetch_UpdateCatsPwdResult(CATSHANDLE hHandle);


////////////////////
// 交易账户(子账户)登录
////////////////////

/// <summary>准备交易账户登录请求.</summary>
int WINAPI CATSAPI_Prepare_TradeAccLogin(CATSHANDLE hHandle);

/// <summary>交易账户登录.</summary>
int WINAPI CATSAPI_Execute_TradeAccLogin(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取交易账户登录结果.</summary>
int WINAPI CATSAPI_Fetch_TradeAccLoginResult(CATSHANDLE hHandle);



/// <summary>准备查询系统参数请求.</summary>
int WINAPI CATSAPI_Prepare_QuerySystemParam(CATSHANDLE hHandle);

/// <summary>查询系统参数请求.</summary>
int WINAPI CATSAPI_Execute_QuerySystemParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取系统参数结果.</summary>
int WINAPI CATSAPI_Fetch_QuerySystemParamResult(CATSHANDLE hHandle);


/// <summary>准备添加系统参数请求.</summary>
int WINAPI CATSAPI_Prepare_AddSystemParam(CATSHANDLE hHandle);

/// <summary>添加系统参数请求.</summary>
int WINAPI CATSAPI_Execute_AddSystemParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取添加系统参数结果.</summary>
int WINAPI CATSAPI_Fetch_AddSystemParamResult(CATSHANDLE hHandle);


/// <summary>准备删除系统参数请求.</summary>
int WINAPI CATSAPI_Prepare_DelSystemParam(CATSHANDLE hHandle);

/// <summary>删除系统参数请求.</summary>
int WINAPI CATSAPI_Execute_DelSystemParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取删除系统参数结果.</summary>
int WINAPI CATSAPI_Fetch_DelSystemParamResult(CATSHANDLE hHandle);


/// <summary>准备修改或新增系统参数请求.</summary>
int WINAPI CATSAPI_Prepare_ModifySystemParam(CATSHANDLE hHandle);

/// <summary>修改或新增系统参数请求.</summary>
int WINAPI CATSAPI_Execute_ModifySystemParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取修改或新增系统参数结果.</summary>
int WINAPI CATSAPI_Fetch_ModifySystemParamResult(CATSHANDLE hHandle);


/// <summary>准备修改或新增系统参数请求.</summary>
int WINAPI CATSAPI_Prepare_SaveSystemParam(CATSHANDLE hHandle);

/// <summary>修改或新增系统参数请求.</summary>
int WINAPI CATSAPI_Execute_SaveSystemParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取修改或新增系统参数结果.</summary>
int WINAPI CATSAPI_Fetch_SaveSystemParamResult(CATSHANDLE hHandle);


/// <summary>准备修改CATS账户安全登录方式请求.</summary>
int WINAPI CATSAPI_Prepare_UpdateCatsSecLoginType(CATSHANDLE hHandle);

/// <summary>修改CATS账户安全登录方式请求.</summary>
int WINAPI CATSAPI_Execute_UpdateCatsSecLoginType(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取修改CATS账户安全登录方式结果.</summary>
int WINAPI CATSAPI_Fetch_UpdateCatsSecLoginTypeResult(CATSHANDLE hHandle);


/// <summary>准备校验CATS账户安全问答.</summary>
int WINAPI CATSAPI_Prepare_CheckSecurityQuestion(CATSHANDLE hHandle);

/// <summary>校验CATS账户安全问答.</summary>
int WINAPI CATSAPI_Execute_CheckSecurityQuestion(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>校验CATS账户安全问答结果.</summary>
int WINAPI CATSAPI_Fetch_CheckSecurityQuestionResult(CATSHANDLE hHandle);


/// <summary>准备生成CATS账户短信随机验证码.</summary>
int WINAPI CATSAPI_Prepare_GetCatsRandomCode(CATSHANDLE hHandle);

/// <summary>生成CATS账户短信随机验证码.</summary>
int WINAPI CATSAPI_Execute_GetCatsRandomCode(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>生成CATS账户短信随机验证码结果.</summary>
int WINAPI CATSAPI_Fetch_GetCatsRandomCodeResult(CATSHANDLE hHandle);


/// <summary>准备校验CATS账户短信随机验证码.</summary>
int WINAPI CATSAPI_Prepare_CheckCatsRandomCode(CATSHANDLE hHandle);

/// <summary>校验CATS账户短信随机验证码.</summary>
int WINAPI CATSAPI_Execute_CheckCatsRandomCode(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>校验CATS账户短信随机验证码结果.</summary>
int WINAPI CATSAPI_Fetch_CheckCatsRandomCodeResult(CATSHANDLE hHandle);


/// <summary>准备获取系统默认安全问题.</summary>
int WINAPI CATSAPI_Prepare_QuerySysDefSecQuestion(CATSHANDLE hHandle);

/// <summary>获取系统默认安全问题.</summary>
int WINAPI CATSAPI_Execute_QuerySysDefSecQuestion(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>获取系统默认安全问题结果.</summary>
int WINAPI CATSAPI_Fetch_QuerySysDefSecQuestionResult(CATSHANDLE hHandle);


/// <summary>准备查询CATS账户安全问答.</summary>
int WINAPI CATSAPI_Prepare_QuerySecurityQuestion(CATSHANDLE hHandle);

/// <summary>查询CATS账户安全问答.</summary>
int WINAPI CATSAPI_Execute_QuerySecurityQuestion(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>查询CATS账户安全问答结果.</summary>
int WINAPI CATSAPI_Fetch_QuerySecurityQuestionResult(CATSHANDLE hHandle);


/// <summary>准备添加CATS账户安全问答.</summary>
int WINAPI CATSAPI_Prepare_AddSecurityQuestion(CATSHANDLE hHandle);

/// <summary>添加CATS账户安全问答.</summary>
int WINAPI CATSAPI_Execute_AddSecurityQuestion(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>添加CATS账户安全问答结果.</summary>
int WINAPI CATSAPI_Fetch_AddSecurityQuestionResult(CATSHANDLE hHandle);


/// <summary>准备删除CATS账户安全问答.</summary>
int WINAPI CATSAPI_Prepare_DeleteSecurityQuestion(CATSHANDLE hHandle);

/// <summary>删除CATS账户安全问答.</summary>
int WINAPI CATSAPI_Execute_DeleteSecurityQuestion(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>删除CATS账户安全问答结果.</summary>
int WINAPI CATSAPI_Fetch_DeleteSecurityQuestionResult(CATSHANDLE hHandle);


/// <summary>准备更新CATS账户安全问答.</summary>
int WINAPI CATSAPI_Prepare_UpdateSecurityQuestion(CATSHANDLE hHandle);

/// <summary>更新CATS账户安全问答.</summary>
int WINAPI CATSAPI_Execute_UpdateSecurityQuestion(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>更新CATS账户安全问答结果.</summary>
int WINAPI CATSAPI_Fetch_UpdateSecurityQuestionResult(CATSHANDLE hHandle);


/// <summary>准备更新CATS账户二次验证.</summary>
int WINAPI CATSAPI_Prepare_SetSecondLoginCheck(CATSHANDLE hHandle);

/// <summary>更新CATS账户二次验证.</summary>
int WINAPI CATSAPI_Execute_SetSecondLoginCheck(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>更新CATS账户二次验证结果.</summary>
int WINAPI CATSAPI_Fetch_SetSecondLoginCheckResult(CATSHANDLE hHandle);



/// <summary>准备查询CATS账户二次验证.</summary>
int WINAPI CATSAPI_Prepare_QuerySecondLoginCheck(CATSHANDLE hHandle);

/// <summary>查询CATS账户二次验证.</summary>
int WINAPI CATSAPI_Execute_QuerySecondLoginCheck(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>查询CATS账户二次验证结果.</summary>
int WINAPI CATSAPI_Fetch_QuerySecondLoginCheckResult(CATSHANDLE hHandle);

/// <summary>查询CATS账户二次验证问答结果.</summary>
int WINAPI CATSAPI_Fetch_QuerySecondLoginCheckResult_Question(CATSHANDLE hHandle);



/// <summary>准备查询数据字典.</summary>
int WINAPI CATSAPI_Prepare_QueryDictionary(CATSHANDLE hHandle);

/// <summary>查询数据字典请求.</summary>
int WINAPI CATSAPI_Execute_QueryDictionary(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取数据字典结果.</summary>
int WINAPI CATSAPI_Fetch_QueryDictionaryResult(CATSHANDLE hHandle);


/// <summary>准备刷新子账户信息.</summary>
int WINAPI CATSAPI_Prepare_RefreshAccountInfo(CATSHANDLE hHandle);

/// <summary>刷新子账户信息请求.</summary>
int WINAPI CATSAPI_Execute_RefreshAccountInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取刷新子账户信息请求结果.</summary>
int WINAPI CATSAPI_Fetch_RefreshAccountInfoResult(CATSHANDLE hHandle);


/// <summary>准备查询CATS账户功能权限.</summary>
int WINAPI CATSAPI_Prepare_QueryCatsFunction(CATSHANDLE hHandle);

/// <summary>查询CATS账户功能权限.</summary>
int WINAPI CATSAPI_Execute_QueryCatsFunction(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询CATS账户功能权限结果.</summary>
int WINAPI CATSAPI_Fetch_QueryCatsFunctionResult(CATSHANDLE hHandle);

# endif
#ifdef   __cplusplus
}
#endif