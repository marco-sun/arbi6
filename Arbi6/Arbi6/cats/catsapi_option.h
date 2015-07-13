
//=============================================================================
/**
 *  @library	catsapi.lib
 *
 *  @file	catsapi_option.h
 *  
 *  catsapi_option.h, v1.00 2014/01/02 13:33:56 wanghz
 *  
 *  @description 期权相关接口
 *  
 *  @author wanghz
 */
//=============================================================================

#include "catsapi.h"

#ifdef   __cplusplus
extern "C" {
#endif

#ifndef _CATSAPI_OPTION_H
#define _CATSAPI_OPTION_H

	//////////////////////////////////////////////////////////////////////////
	/// 期权
	//////////////////////////////////////////////////////////////////////////

	////////////////////
	//期权做市模型数据查询
	////////////////////
/// <summary>准备查询期权做市模型数据请求.</summary>
int WINAPI CATSAPI_Prepare_QueryOMMModelData(CATSHANDLE hHandle);

/// <summary>查询期权做市模型数据请求.</summary>
int WINAPI CATSAPI_Execute_QueryOMMModelData(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询期权做市模型数据结果.</summary>
int WINAPI CATSAPI_Fetch_QueryOMMModelDataResult_Underlings(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryOMMModelDataResult_Models(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryOMMModelDataResult_OmmInstruments(CATSHANDLE hHandle);



/// <summary>准备查询期权wing curve请求.</summary>
int WINAPI CATSAPI_Prepare_QueryWingCurve(CATSHANDLE hHandle);

/// <summary>查询期权wing curve数据请求.</summary>
int WINAPI CATSAPI_Execute_QueryWingCurve(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询期权wing curve数据结果.</summary>
int WINAPI CATSAPI_Fetch_QueryWingCurveResult(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveResult_BidVols(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveResult_AskVols(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveResult_CurrentStrikeVols(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveResult_MarketStrikeVols(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveResult_Greeks(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveResult_Params(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveResult_Curves(CATSHANDLE hHandle);


/// <summary>准备查询期权wing curve请求.</summary>
int WINAPI CATSAPI_Prepare_QueryWingCurveWithParam(CATSHANDLE hHandle);

/// <summary>查询期权wing curve数据请求.</summary>
int WINAPI CATSAPI_Execute_QueryWingCurveWithParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询期权wing curve数据结果.</summary>
int WINAPI CATSAPI_Fetch_QueryWingCurveWithParamResult(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveWithParamResult_BidVols(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveWithParamResult_AskVols(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveWithParamResult_StrikeVols(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveWithParamResult_Greeks(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveWithParamResult_Params(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveWithParamResult_Curves(CATSHANDLE hHandle);


/// <summary>准备设置模型参数请求.</summary>
int WINAPI CATSAPI_Prepare_SetModelParam(CATSHANDLE hHandle);

/// <summary>设置模型参数请求.</summary>
int WINAPI CATSAPI_Execute_SetModelParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取设置模型参数结果.</summary>
int WINAPI CATSAPI_Fetch_SetModelParamResult(CATSHANDLE hHandle);



/// <summary>准备保存期权策略参数请求.</summary>
int WINAPI CATSAPI_Prepare_SaveOptionStrategeParam(CATSHANDLE hHandle);

/// <summary>保存期权策略参数请求.</summary>
int WINAPI CATSAPI_Execute_SaveOptionStrategeParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取保存期权策略参数结果.</summary>
int WINAPI CATSAPI_Fetch_SaveOptionStrategeParamResult(CATSHANDLE hHandle);

/// <summary>准备查询期权策略参数请求.</summary>
int WINAPI CATSAPI_Prepare_QueryOptionStrategeParam(CATSHANDLE hHandle);

/// <summary>查询期权策略参数请求.</summary>
int WINAPI CATSAPI_Execute_QueryOptionStrategeParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询期权策略参数结果.</summary>
int WINAPI CATSAPI_Fetch_QueryOptionStrategeParamResult(CATSHANDLE hHandle);



/// <summary>准备删除期权策略参数请求.</summary>
int WINAPI CATSAPI_Prepare_DelOptionStrategeParam(CATSHANDLE hHandle);

/// <summary>删除期权策略参数请求.</summary>
int WINAPI CATSAPI_Execute_DelOptionStrategeParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取删除期权策略参数结果.</summary>
int WINAPI CATSAPI_Fetch_DelOptionStrategeParamResult(CATSHANDLE hHandle);


/// <summary>准备初始化策略参数请求.</summary>
int WINAPI CATSAPI_Prepare_InitStrategyParam(CATSHANDLE hHandle);

/// <summary>初始化策略参数请求.</summary>
int WINAPI CATSAPI_Execute_InitStrategyParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取初始化策略参数结果.</summary>
int WINAPI CATSAPI_Fetch_InitStrategyParamResult(CATSHANDLE hHandle);


/// <summary>准备通用更新订阅请求.</summary>
int WINAPI CATSAPI_PreSub_AccountPublishDate(CATSHANDLE hHandle);

/// <summary>订阅通用更新.</summary>
int WINAPI CATSAPI_Subscribe_AccountPublishDate(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>提取通用更新结果.</summary>
int WINAPI CATSAPI_Fetch_AccountPublishDateResult(CATSHANDLE hHandle);

/// <summary>提取通用更新键值结果.</summary>
int WINAPI CATSAPI_Fetch_AccountPublishDateResult_KeyValue(CATSHANDLE hHandle);


/// <summary>准备期权策略查询请求.</summary>
int WINAPI CATSAPI_Prepare_QueryOptionStrategy(CATSHANDLE hHandle);

/// <summary>期权策略查询请求.</summary>
int WINAPI CATSAPI_Execute_QueryOptionStrategy(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取期权策略查询请求结果.</summary>
int WINAPI CATSAPI_Fetch_QueryOptionStrategyResult(CATSHANDLE hHandle);


/// <summary>准备期权策略删除请求.</summary>
int WINAPI CATSAPI_Prepare_DeleteOptionStrategy(CATSHANDLE hHandle);

/// <summary>期权策略删除请求.</summary>
int WINAPI CATSAPI_Execute_DeleteOptionStrategy(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取期权策略删除请求结果.</summary>
int WINAPI CATSAPI_Fetch_DeleteOptionStrategyResult(CATSHANDLE hHandle);


/// <summary>准备期权对冲策略参数保存请求.</summary>
int WINAPI CATSAPI_Prepare_SaveOptionHedgingParam(CATSHANDLE hHandle);

/// <summary>期权对冲策略参数保存请求.</summary>
int WINAPI CATSAPI_Execute_SaveOptionHedgingParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取期权对冲策略参数保存请求结果.</summary>
int WINAPI CATSAPI_Fetch_SaveOptionHedgingParamResult(CATSHANDLE hHandle);


/// <summary>准备期权对冲策略参数查询请求.</summary>
int WINAPI CATSAPI_Prepare_QueryOptionHedgingParam(CATSHANDLE hHandle);

/// <summary>期权对冲策略参数查询请求.</summary>
int WINAPI CATSAPI_Execute_QueryOptionHedgingParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取期权对冲策略参数查询请求结果.</summary>
int WINAPI CATSAPI_Fetch_QueryOptionHedgingParamResult(CATSHANDLE hHandle);


/// <summary>准备期权对冲策略参数删除请求.</summary>
int WINAPI CATSAPI_Prepare_DeleteOptionHedgingParam(CATSHANDLE hHandle);

/// <summary>期权对冲策略参数删除请求.</summary>
int WINAPI CATSAPI_Execute_DeleteOptionHedgingParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取期权对冲策略参数删除请求结果.</summary>
int WINAPI CATSAPI_Fetch_DeleteOptionHedgingParamResult(CATSHANDLE hHandle);


/// <summary>准备期权对冲策略查询请求.</summary>
int WINAPI CATSAPI_Prepare_QueryOptionHedgingStrategy(CATSHANDLE hHandle);

/// <summary>期权对冲策略查询请求.</summary>
int WINAPI CATSAPI_Execute_QueryOptionHedgingStrategy(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取期权对冲策略查询请求结果.</summary>
int WINAPI CATSAPI_Fetch_QueryOptionHedgingStrategyResult(CATSHANDLE hHandle);


/// <summary>准备期权对冲策略删除请求.</summary>
int WINAPI CATSAPI_Prepare_DeleteOptionHedgingStrategy(CATSHANDLE hHandle);

/// <summary>期权对冲策略删除请求.</summary>
int WINAPI CATSAPI_Execute_DeleteOptionHedgingStrategy(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取期权对冲策略删除请求结果.</summary>
int WINAPI CATSAPI_Fetch_DeleteOptionHedgingStrategyResult(CATSHANDLE hHandle);


/// <summary>准备期权组合保存请求.</summary>
int WINAPI CATSAPI_Prepare_SaveOptionPortfolio(CATSHANDLE hHandle);

/// <summary>期权组合保存请求.</summary>
int WINAPI CATSAPI_Execute_SaveOptionPortfolio(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取期权组合保存请求结果.</summary>
int WINAPI CATSAPI_Fetch_SaveOptionPortfolioResult(CATSHANDLE hHandle);


/// <summary>准备期权组合查询请求.</summary>
int WINAPI CATSAPI_Prepare_QueryOptionPortfolio(CATSHANDLE hHandle);

/// <summary>期权组合查询请求.</summary>
int WINAPI CATSAPI_Execute_QueryOptionPortfolio(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取期权组合查询请求结果.</summary>
int WINAPI CATSAPI_Fetch_QueryOptionPortfolioResult(CATSHANDLE hHandle);



# endif

#ifdef   __cplusplus
}
#endif
