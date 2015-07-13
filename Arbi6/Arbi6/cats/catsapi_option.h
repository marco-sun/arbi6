
//=============================================================================
/**
 *  @library	catsapi.lib
 *
 *  @file	catsapi_option.h
 *  
 *  catsapi_option.h, v1.00 2014/01/02 13:33:56 wanghz
 *  
 *  @description ��Ȩ��ؽӿ�
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
	/// ��Ȩ
	//////////////////////////////////////////////////////////////////////////

	////////////////////
	//��Ȩ����ģ�����ݲ�ѯ
	////////////////////
/// <summary>׼����ѯ��Ȩ����ģ����������.</summary>
int WINAPI CATSAPI_Prepare_QueryOMMModelData(CATSHANDLE hHandle);

/// <summary>��ѯ��Ȩ����ģ����������.</summary>
int WINAPI CATSAPI_Execute_QueryOMMModelData(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ��Ȩ����ģ�����ݽ��.</summary>
int WINAPI CATSAPI_Fetch_QueryOMMModelDataResult_Underlings(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryOMMModelDataResult_Models(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryOMMModelDataResult_OmmInstruments(CATSHANDLE hHandle);



/// <summary>׼����ѯ��Ȩwing curve����.</summary>
int WINAPI CATSAPI_Prepare_QueryWingCurve(CATSHANDLE hHandle);

/// <summary>��ѯ��Ȩwing curve��������.</summary>
int WINAPI CATSAPI_Execute_QueryWingCurve(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ��Ȩwing curve���ݽ��.</summary>
int WINAPI CATSAPI_Fetch_QueryWingCurveResult(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveResult_BidVols(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveResult_AskVols(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveResult_CurrentStrikeVols(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveResult_MarketStrikeVols(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveResult_Greeks(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveResult_Params(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveResult_Curves(CATSHANDLE hHandle);


/// <summary>׼����ѯ��Ȩwing curve����.</summary>
int WINAPI CATSAPI_Prepare_QueryWingCurveWithParam(CATSHANDLE hHandle);

/// <summary>��ѯ��Ȩwing curve��������.</summary>
int WINAPI CATSAPI_Execute_QueryWingCurveWithParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ��Ȩwing curve���ݽ��.</summary>
int WINAPI CATSAPI_Fetch_QueryWingCurveWithParamResult(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveWithParamResult_BidVols(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveWithParamResult_AskVols(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveWithParamResult_StrikeVols(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveWithParamResult_Greeks(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveWithParamResult_Params(CATSHANDLE hHandle);
int WINAPI CATSAPI_Fetch_QueryWingCurveWithParamResult_Curves(CATSHANDLE hHandle);


/// <summary>׼������ģ�Ͳ�������.</summary>
int WINAPI CATSAPI_Prepare_SetModelParam(CATSHANDLE hHandle);

/// <summary>����ģ�Ͳ�������.</summary>
int WINAPI CATSAPI_Execute_SetModelParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ����ģ�Ͳ������.</summary>
int WINAPI CATSAPI_Fetch_SetModelParamResult(CATSHANDLE hHandle);



/// <summary>׼��������Ȩ���Բ�������.</summary>
int WINAPI CATSAPI_Prepare_SaveOptionStrategeParam(CATSHANDLE hHandle);

/// <summary>������Ȩ���Բ�������.</summary>
int WINAPI CATSAPI_Execute_SaveOptionStrategeParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ������Ȩ���Բ������.</summary>
int WINAPI CATSAPI_Fetch_SaveOptionStrategeParamResult(CATSHANDLE hHandle);

/// <summary>׼����ѯ��Ȩ���Բ�������.</summary>
int WINAPI CATSAPI_Prepare_QueryOptionStrategeParam(CATSHANDLE hHandle);

/// <summary>��ѯ��Ȩ���Բ�������.</summary>
int WINAPI CATSAPI_Execute_QueryOptionStrategeParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ��Ȩ���Բ������.</summary>
int WINAPI CATSAPI_Fetch_QueryOptionStrategeParamResult(CATSHANDLE hHandle);



/// <summary>׼��ɾ����Ȩ���Բ�������.</summary>
int WINAPI CATSAPI_Prepare_DelOptionStrategeParam(CATSHANDLE hHandle);

/// <summary>ɾ����Ȩ���Բ�������.</summary>
int WINAPI CATSAPI_Execute_DelOptionStrategeParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡɾ����Ȩ���Բ������.</summary>
int WINAPI CATSAPI_Fetch_DelOptionStrategeParamResult(CATSHANDLE hHandle);


/// <summary>׼����ʼ�����Բ�������.</summary>
int WINAPI CATSAPI_Prepare_InitStrategyParam(CATSHANDLE hHandle);

/// <summary>��ʼ�����Բ�������.</summary>
int WINAPI CATSAPI_Execute_InitStrategyParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ʼ�����Բ������.</summary>
int WINAPI CATSAPI_Fetch_InitStrategyParamResult(CATSHANDLE hHandle);


/// <summary>׼��ͨ�ø��¶�������.</summary>
int WINAPI CATSAPI_PreSub_AccountPublishDate(CATSHANDLE hHandle);

/// <summary>����ͨ�ø���.</summary>
int WINAPI CATSAPI_Subscribe_AccountPublishDate(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>��ȡͨ�ø��½��.</summary>
int WINAPI CATSAPI_Fetch_AccountPublishDateResult(CATSHANDLE hHandle);

/// <summary>��ȡͨ�ø��¼�ֵ���.</summary>
int WINAPI CATSAPI_Fetch_AccountPublishDateResult_KeyValue(CATSHANDLE hHandle);


/// <summary>׼����Ȩ���Բ�ѯ����.</summary>
int WINAPI CATSAPI_Prepare_QueryOptionStrategy(CATSHANDLE hHandle);

/// <summary>��Ȩ���Բ�ѯ����.</summary>
int WINAPI CATSAPI_Execute_QueryOptionStrategy(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��Ȩ���Բ�ѯ������.</summary>
int WINAPI CATSAPI_Fetch_QueryOptionStrategyResult(CATSHANDLE hHandle);


/// <summary>׼����Ȩ����ɾ������.</summary>
int WINAPI CATSAPI_Prepare_DeleteOptionStrategy(CATSHANDLE hHandle);

/// <summary>��Ȩ����ɾ������.</summary>
int WINAPI CATSAPI_Execute_DeleteOptionStrategy(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��Ȩ����ɾ��������.</summary>
int WINAPI CATSAPI_Fetch_DeleteOptionStrategyResult(CATSHANDLE hHandle);


/// <summary>׼����Ȩ�Գ���Բ�����������.</summary>
int WINAPI CATSAPI_Prepare_SaveOptionHedgingParam(CATSHANDLE hHandle);

/// <summary>��Ȩ�Գ���Բ�����������.</summary>
int WINAPI CATSAPI_Execute_SaveOptionHedgingParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��Ȩ�Գ���Բ�������������.</summary>
int WINAPI CATSAPI_Fetch_SaveOptionHedgingParamResult(CATSHANDLE hHandle);


/// <summary>׼����Ȩ�Գ���Բ�����ѯ����.</summary>
int WINAPI CATSAPI_Prepare_QueryOptionHedgingParam(CATSHANDLE hHandle);

/// <summary>��Ȩ�Գ���Բ�����ѯ����.</summary>
int WINAPI CATSAPI_Execute_QueryOptionHedgingParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��Ȩ�Գ���Բ�����ѯ������.</summary>
int WINAPI CATSAPI_Fetch_QueryOptionHedgingParamResult(CATSHANDLE hHandle);


/// <summary>׼����Ȩ�Գ���Բ���ɾ������.</summary>
int WINAPI CATSAPI_Prepare_DeleteOptionHedgingParam(CATSHANDLE hHandle);

/// <summary>��Ȩ�Գ���Բ���ɾ������.</summary>
int WINAPI CATSAPI_Execute_DeleteOptionHedgingParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��Ȩ�Գ���Բ���ɾ��������.</summary>
int WINAPI CATSAPI_Fetch_DeleteOptionHedgingParamResult(CATSHANDLE hHandle);


/// <summary>׼����Ȩ�Գ���Բ�ѯ����.</summary>
int WINAPI CATSAPI_Prepare_QueryOptionHedgingStrategy(CATSHANDLE hHandle);

/// <summary>��Ȩ�Գ���Բ�ѯ����.</summary>
int WINAPI CATSAPI_Execute_QueryOptionHedgingStrategy(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��Ȩ�Գ���Բ�ѯ������.</summary>
int WINAPI CATSAPI_Fetch_QueryOptionHedgingStrategyResult(CATSHANDLE hHandle);


/// <summary>׼����Ȩ�Գ����ɾ������.</summary>
int WINAPI CATSAPI_Prepare_DeleteOptionHedgingStrategy(CATSHANDLE hHandle);

/// <summary>��Ȩ�Գ����ɾ������.</summary>
int WINAPI CATSAPI_Execute_DeleteOptionHedgingStrategy(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��Ȩ�Գ����ɾ��������.</summary>
int WINAPI CATSAPI_Fetch_DeleteOptionHedgingStrategyResult(CATSHANDLE hHandle);


/// <summary>׼����Ȩ��ϱ�������.</summary>
int WINAPI CATSAPI_Prepare_SaveOptionPortfolio(CATSHANDLE hHandle);

/// <summary>��Ȩ��ϱ�������.</summary>
int WINAPI CATSAPI_Execute_SaveOptionPortfolio(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��Ȩ��ϱ���������.</summary>
int WINAPI CATSAPI_Fetch_SaveOptionPortfolioResult(CATSHANDLE hHandle);


/// <summary>׼����Ȩ��ϲ�ѯ����.</summary>
int WINAPI CATSAPI_Prepare_QueryOptionPortfolio(CATSHANDLE hHandle);

/// <summary>��Ȩ��ϲ�ѯ����.</summary>
int WINAPI CATSAPI_Execute_QueryOptionPortfolio(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��Ȩ��ϲ�ѯ������.</summary>
int WINAPI CATSAPI_Fetch_QueryOptionPortfolioResult(CATSHANDLE hHandle);



# endif

#ifdef   __cplusplus
}
#endif
