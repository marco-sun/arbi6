//=============================================================================
/**
 *  @library	catsapi.lib
 *
 *  @file	catsapi_algo.h
 *  
 *  catsapi_algo.h, v1.00 2012/10/20 17:23:56 zhaolei
 *  
 *  @description
 *  
 *  @author zhaolei
 */
//=============================================================================

#include "catsapi.h"

#ifdef   __cplusplus
extern "C" {
#endif

#ifndef _CATSAPI_ALGO_H
#define _CATSAPI_ALGO_H

//////////////////////////////////////////////////////////////////////////
/// 算法交易类
//////////////////////////////////////////////////////////////////////////

////////////////////
// 启动算法
////////////////////

/// <summary>准备启动算法请求.</summary>
int WINAPI CATSAPI_Prepare_StartAlgo(CATSHANDLE hHandle);

/// <summary>启动算法.</summary>
int WINAPI CATSAPI_Execute_StartAlgo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取启动算法结果.</summary>
int WINAPI CATSAPI_Fetch_StartAlgoResult(CATSHANDLE hHandle);

////////////////////
// 停止算法
////////////////////

/// <summary>准备停止算法请求.</summary>
int WINAPI CATSAPI_Prepare_StopAlgo(CATSHANDLE hHandle);

/// <summary>停止算法.</summary>
int WINAPI CATSAPI_Execute_StopAlgo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取停止算法结果.</summary>
int WINAPI CATSAPI_Fetch_StopAlgoResult(CATSHANDLE hHandle);

////////////////////
// 更新算法实例
////////////////////

/// <summary>准备更新算法实例请求.</summary>
int WINAPI CATSAPI_Prepare_UpdateAlgoInstance(CATSHANDLE hHandle);

/// <summary>更新算法实例.</summary>
int WINAPI CATSAPI_Execute_UpdateAlgoInstance(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取更新算法实例结果.</summary>
int WINAPI CATSAPI_Fetch_UpdateAlgoInstanceResult(CATSHANDLE hHandle);


////////////////////
// 订阅算法实例状态
////////////////////
//旧接口，不推荐使用,实例及参数合并到Subscribe_AlgoInstanceEx,执行信息合并到SubscribeExecStatisData
/// <summary>准备算法实例状态订阅请求.</summary>
int WINAPI CATSAPI_PreSub_AlgoStatus(CATSHANDLE hHandle);

/// <summary>订阅算法实例状态.</summary>
int WINAPI CATSAPI_Subscribe_AlgoStatus(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>准备算法实例状态退订请求.</summary>
int WINAPI CATSAPI_PreUnSub_AlgoStatus(CATSHANDLE hHandle);

/// <summary>退订算法实例状态.</summary>
int WINAPI CATSAPI_UnSubscribe_AlgoStatus(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取算法实例状态结果.</summary>
int WINAPI CATSAPI_Fetch_AlgoStatusResult(CATSHANDLE hHandle);

/// <summary>提取算法实例执行结果.</summary>
int WINAPI CATSAPI_Fetch_AlgoStatusResult_ExecDatas(CATSHANDLE hHandle);

/// <summary>提取算法实例参数结果.</summary>
int WINAPI CATSAPI_Fetch_AlgoStatusResult_Params(CATSHANDLE hHandle);



//旧接口，不推荐使用,合并到Subscribe_AlgoInstanceEx
/// <summary>准备算法实例列表订阅请求.</summary>
int WINAPI CATSAPI_PreSub_AlgoInstance(CATSHANDLE hHandle);

/// <summary>订阅算法实例列表.</summary>
int WINAPI CATSAPI_Subscribe_AlgoInstance(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>准备算法实例列表退订请求.</summary>
int WINAPI CATSAPI_PreUnSub_AlgoInstance(CATSHANDLE hHandle);

/// <summary>退订算法实例列表.</summary>
int WINAPI CATSAPI_UnSubscribe_AlgoInstance(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取算法实例结果.</summary>
int WINAPI CATSAPI_Fetch_AlgoInstanceResult(CATSHANDLE hHandle);

/// <summary>提取算法实例列表结果.</summary>
int WINAPI CATSAPI_Fetch_AlgoInstanceListResult(CATSHANDLE hHandle);

/// <summary>提取算法实例参数结果.</summary>
int WINAPI CATSAPI_Fetch_AlgoInstanceParamResult(CATSHANDLE hHandle);



/// <summary>准备算法实例列表订阅请求.</summary>
int WINAPI CATSAPI_PreSub_AlgoInstanceEx(CATSHANDLE hHandle);

/// <summary>订阅算法实例列表.</summary>
int WINAPI CATSAPI_Subscribe_AlgoInstanceEx(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>准备算法实例列表退订请求.</summary>
int WINAPI CATSAPI_PreUnSub_AlgoInstanceEx(CATSHANDLE hHandle);

/// <summary>退订算法实例列表.</summary>
int WINAPI CATSAPI_UnSubscribe_AlgoInstanceEx(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取算法实例结果.</summary>
int WINAPI CATSAPI_Fetch_AlgoInstanceResultEx(CATSHANDLE hHandle);

/// <summary>提取算法实例参数结果.</summary>
int WINAPI CATSAPI_Fetch_AlgoInstanceParamResultEx(CATSHANDLE hHandle);


/// <summary>准备算法执行信息订阅请求.</summary>
int WINAPI CATSAPI_PreSub_AlgoExecStatisData(CATSHANDLE hHandle);

/// <summary>订阅算法实例执行信息.</summary>
int WINAPI CATSAPI_Subscribe_AlgoExecStatisData(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>准备算法实例执行信息退订请求.</summary>
int WINAPI CATSAPI_PreUnSub_AlgoExecStatisData(CATSHANDLE hHandle);

/// <summary>退订算法实例执行信息.</summary>
int WINAPI CATSAPI_UnSubscribe_AlgoExecStatisData(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取算法实例结果.</summary>
int WINAPI CATSAPI_Fetch_AlgoExecStatisDataResult(CATSHANDLE hHandle);

/// <summary>提取算法实例执行信息结果.</summary>
int WINAPI CATSAPI_Fetch_AlgoExecStatisDataResult_ExecStatisData(CATSHANDLE hHandle);



////////////////////
// 订阅算法实例运行日志
////////////////////

/// <summary>准备算法实例运行日志订阅请求.</summary>
int WINAPI CATSAPI_PreSub_AlgoRunLog(CATSHANDLE hHandle);

/// <summary>订阅算法实例运行日志.</summary>
int WINAPI CATSAPI_Subscribe_AlgoRunLog(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>准备算法实例运行日志退订请求.</summary>
int WINAPI CATSAPI_PreUnSub_AlgoRunLog(CATSHANDLE hHandle);

/// <summary>退订算法实例运行日志.</summary>
int WINAPI CATSAPI_UnSubscribe_AlgoRunLog(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取算法实例运行日志推送结果.</summary>
int WINAPI CATSAPI_Fetch_AlgoRunLogResult(CATSHANDLE hHandle);

/// <summary>提取算法实例运行日志推送结果中日志结果.</summary>
int WINAPI CATSAPI_Fetch_AlgoRunLogResult_Logs(CATSHANDLE hHandle);



/// <summary>准备算法实例委托指令订阅请求.</summary>
int WINAPI CATSAPI_PreSub_AlgoInstOrders(CATSHANDLE hHandle);

/// <summary>订阅算法实例委托指令.</summary>
int WINAPI CATSAPI_Subscribe_AlgoInstOrders(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>提取算法实例委托指令.</summary>
int WINAPI CATSAPI_Fetch_AlgoInst_Order(CATSHANDLE hHandle);


/// <summary>准备算法实例撤单指令订阅请求.</summary>
int WINAPI CATSAPI_PreSub_AlgoInstCancelOrders(CATSHANDLE hHandle);

/// <summary>订阅算法实例撤单指令.</summary>
int WINAPI CATSAPI_Subscribe_AlgoInstCancelOrders(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>提取算法实例撤单指令.</summary>
int WINAPI CATSAPI_Fetch_AlgoInst_CancelOrder(CATSHANDLE hHandle);


/// <summary>准备更新算法实例委托状态请求.</summary>
int WINAPI CATSAPI_Prepare_UpdateAlgoOrderStatus(CATSHANDLE hHandle);

/// <summary>执行更新算法实例委托状态请求.</summary>
int WINAPI CATSAPI_Execute_UpdateAlgoOrderStatus(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取执行更新算法实例委托状态请求结果.</summary>
int WINAPI CATSAPI_Fetch_UpdateAlgoOrderResult(CATSHANDLE hHandle);


//查询客户授权算法
/// <summary>准备查询客户授权算法.</summary>
int WINAPI CATSAPI_Prepare_QryCatsAlgo(CATSHANDLE hHandle);

/// <summary>执行查询客户授权算法.</summary>
int WINAPI CATSAPI_Execute_QryCatsAlgo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取执行查询客户授权算法请求结果.</summary>
int WINAPI CATSAPI_Fetch_CatsAlgoResult(CATSHANDLE hHandle);

//查询算法参数
/// <summary>准备查询算法参数.</summary>
int WINAPI CATSAPI_Prepare_QryAlgoParams(CATSHANDLE hHandle);

/// <summary>执行查询算法参数.</summary>
int WINAPI CATSAPI_Execute_QryAlgoParams(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取算法参数查询结果.</summary>
int WINAPI CATSAPI_Fetch_AlgoParamsResult(CATSHANDLE hHandle);

//查询算法实例
/// <summary>准备查询算法实例.</summary>
int WINAPI CATSAPI_Prepare_QryAlgoInstance(CATSHANDLE hHandle);

/// <summary>查询算法实例.</summary>
int WINAPI CATSAPI_Execute_QryAlgoInstance(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取算法实例结果.</summary>
int WINAPI CATSAPI_Fetch_QryAlgoInstResult_AlgoInst(CATSHANDLE hHandle);

/// <summary>提取算法实例参数结果.</summary>
int WINAPI CATSAPI_Fetch_QryAlgoInstResult_AlgoInstParams(CATSHANDLE hHandle);


//查询算法实例参数
/// <summary>准备查询算法实例参数.</summary>
int WINAPI CATSAPI_Prepare_QryAlgoInstanceParams(CATSHANDLE hHandle);

/// <summary>查询算法实例参数.</summary>
int WINAPI CATSAPI_Execute_QryAlgoInstanceParams(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取算法实例参数查询结果.</summary>
int WINAPI CATSAPI_Fetch_AlgoInstanceParamsResult(CATSHANDLE hHandle);

//添加算法模版
/// <summary>准备添加算法模版.</summary>
int WINAPI CATSAPI_Prepare_AddAlgoTemplates(CATSHANDLE hHandle);

/// <summary>添加算法模版.</summary>
int WINAPI CATSAPI_Execute_AddAlgoTemplates(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取添加算法模版结果.</summary>
int WINAPI CATSAPI_Fetch_AddAlgoTemplatesResult(CATSHANDLE hHandle);



////////////////////
// 订阅可转债套利实例执行信息
////////////////////

/// <summary>准备可转债套利策略实例执行信息订阅请求.</summary>
int WINAPI CATSAPI_PreSub_CAProcess(CATSHANDLE hHandle);

/// <summary>订阅可转债套利策略实例执行信息.</summary>
int WINAPI CATSAPI_Subscribe_CAProcess(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>准备可转债套利策略实例执行信息退订请求.</summary>
int WINAPI CATSAPI_PreUnSub_CAProcess(CATSHANDLE hHandle);

/// <summary>退订可转债套利策略实例执行信息.</summary>
int WINAPI CATSAPI_UnSubscribe_CAProcess(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取可转债套利策略实例执行信息结果.</summary>
int WINAPI CATSAPI_Fetch_CAProcessResult(CATSHANDLE hHandle);

/// <summary>提取可转债套利策略实例执行信息结果.</summary>
int WINAPI CATSAPI_Fetch_CAProcessResult_Processes(CATSHANDLE hHandle);



////////////////////
// 订阅通用套利实例执行信息
////////////////////

/// <summary>准备通用套利策略实例执行信息订阅请求.</summary>
int WINAPI CATSAPI_PreSub_CACommonProcess(CATSHANDLE hHandle);

/// <summary>订阅通用套利策略实例执行信息.</summary>
int WINAPI CATSAPI_Subscribe_CACommonProcess(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>准备通用套利策略实例执行信息退订请求.</summary>
int WINAPI CATSAPI_PreUnSub_CACommonProcess(CATSHANDLE hHandle);

/// <summary>退订通用套利策略实例执行信息.</summary>
int WINAPI CATSAPI_UnSubscribe_CACommonProcess(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取通用套利策略实例执行信息结果.</summary>
int WINAPI CATSAPI_Fetch_CACommonProcessResult(CATSHANDLE hHandle);

/// <summary>提取通用套利策略实例执行信息结果.</summary>
int WINAPI CATSAPI_Fetch_CACommonProcessResult_Processes(CATSHANDLE hHandle);


/// <summary>准备查询算法交易及参数请求.</summary>
int WINAPI CATSAPI_Prepare_QueryATRelevantParam(CATSHANDLE hHandle);

/// <summary>查询算法交易及参数请求.</summary>
int WINAPI CATSAPI_Execute_QueryATRelevantParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询算法交易及参数请求结果中的算法结果.</summary>
int WINAPI CATSAPI_Fetch_QueryATRelevantParamResult_Algos(CATSHANDLE hHandle);

/// <summary>提取查询算法交易及参数请求结果中的算法参数结果.</summary>
int WINAPI CATSAPI_Fetch_QueryATRelevantParamResult_AlgosParams(CATSHANDLE hHandle);

# endif

#ifdef   __cplusplus
}
#endif
