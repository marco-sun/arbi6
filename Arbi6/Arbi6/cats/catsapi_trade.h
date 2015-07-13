//=============================================================================
/**
 *  @library	catsapi.lib
 *
 *  @file	catsapi_trade.h
 *  
 *  catsapi_trade.h, v1.00 2012/10/20 17:23:56 zhaolei
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

#ifndef _CATSAPI_TRADE_H
#define _CATSAPI_TRADE_H

////////////////////
// ����ί��
////////////////////

/// <summary>׼������ί������.</summary>
int WINAPI CATSAPI_Prepare_OrderSingle(CATSHANDLE hHandle);

/// <summary>����ί��.</summary>
int WINAPI CATSAPI_Execute_OrderSingle(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ����ί�н��.</summary>
int WINAPI CATSAPI_Fetch_OrderSingleResult(CATSHANDLE hHandle);

////////////////////
// ����ί��
////////////////////

/// <summary>׼������ί������.</summary>
int WINAPI CATSAPI_Prepare_SmartOrder(CATSHANDLE hHandle);

/// <summary>����ί��.</summary>
int WINAPI CATSAPI_Execute_SmartOrder(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ����ί�н��.</summary>
int WINAPI CATSAPI_Fetch_SmartOrderResult(CATSHANDLE hHandle);

////////////////////
// ����ί�г���
////////////////////

/// <summary>׼������ί�г�������.</summary>
int WINAPI CATSAPI_Prepare_WithDrawSingle(CATSHANDLE hHandle);

/// <summary>����ί�г���.</summary>
int WINAPI CATSAPI_Execute_WithDrawSingle(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ����ί�г������.</summary>
int WINAPI CATSAPI_Fetch_WithDrawSingleResult(CATSHANDLE hHandle);

////////////////////
// ���ί��
////////////////////

/// <summary>׼�����ί������.</summary>
int WINAPI CATSAPI_Prepare_OrderBatch(CATSHANDLE hHandle);

/// <summary>���ί��.</summary>
int WINAPI CATSAPI_Execute_OrderBatch(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ���ί�н��.</summary>
int WINAPI CATSAPI_Fetch_OrderBatchResult(CATSHANDLE hHandle);

/// <summary>��ȡ���ί��ί����ϸ���.</summary>
int WINAPI CATSAPI_Fetch_OrderBatchResult_OrderNos(CATSHANDLE hHandle);


/// <summary>׼�����ί������.</summary>
int WINAPI CATSAPI_Prepare_OrderBatchByOrderSingle(CATSHANDLE hHandle);

/// <summary>���ί��.</summary>
int WINAPI CATSAPI_Execute_OrderBatchByOrderSingle(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ���ί�н��.</summary>
int WINAPI CATSAPI_Fetch_OrderBatchByOrderSingleResult(CATSHANDLE hHandle);

/// <summary>��ȡ���ί��ί����ϸ���.</summary>
int WINAPI CATSAPI_Fetch_OrderBatchByOrderSingleResult_OrderNos(CATSHANDLE hHandle);

////////////////////
// ���ί�г���
////////////////////

/// <summary>׼�����ί�г�������.</summary>
int WINAPI CATSAPI_Prepare_WithDrawBatch(CATSHANDLE hHandle);

/// <summary>���ί�г���.</summary>
int WINAPI CATSAPI_Execute_WithDrawBatch(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ���ί�г������.</summary>
int WINAPI CATSAPI_Fetch_WithDrawBatchResult(CATSHANDLE hHandle);


////////////////////
// ��������ί�кų���
////////////////////

/// <summary>׼������ί�г�������.</summary>
int WINAPI CATSAPI_Prepare_WithDrawBatchByOrderNos(CATSHANDLE hHandle);

/// <summary>����ί�г���.</summary>
int WINAPI CATSAPI_Execute_WithDrawBatchByOrderNos(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ����ί�г������.</summary>
int WINAPI CATSAPI_Fetch_WithDrawBatchByOrderNosResult(CATSHANDLE hHandle);


////////////////////
// �ֽ𻹿�
////////////////////

/// <summary>׼���ֽ𻹿�����.</summary>
int WINAPI CATSAPI_Prepare_DirectReturnLoan(CATSHANDLE hHandle);

/// <summary>�ֽ𻹿�.</summary>
int WINAPI CATSAPI_Execute_DirectReturnLoan(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ�ֽ𻹿���.</summary>
int WINAPI CATSAPI_Fetch_DirectReturnLoanResult(CATSHANDLE hHandle);

////////////////////
// ��ȯ��ȯ
////////////////////

/// <summary>׼����ȯ��ȯ����.</summary>
int WINAPI CATSAPI_Prepare_DirectReturnStock(CATSHANDLE hHandle);

/// <summary>��ȯ��ȯ.</summary>
int WINAPI CATSAPI_Execute_DirectReturnStock(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ȯ��ȯ���.</summary>
int WINAPI CATSAPI_Fetch_DirectReturnStockResult(CATSHANDLE hHandle);

////////////////////
// �����ʽ𼰳ֱֲ䶯
////////////////////

/// <summary>׼���ʽ𼰳ֱֲ䶯��������.</summary>
int WINAPI CATSAPI_PreSub_AssetUpdate(CATSHANDLE hHandle);

/// <summary>�����ʽ𼰳ֱֲ䶯.</summary>
int WINAPI CATSAPI_Subscribe_AssetUpdate(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>��ȡ�ʽ𼰳ֱֲ䶯���.</summary>
int WINAPI CATSAPI_Fetch_AssetUpdateResult(CATSHANDLE hHandle);

/// <summary>��ȡ�ʽ���.</summary>
int WINAPI CATSAPI_Fetch_AssetUpdateResult_Funds(CATSHANDLE hHandle);

/// <summary>��ȡ�ֲֽ��.</summary>
int WINAPI CATSAPI_Fetch_AssetUpdateResult_Positions(CATSHANDLE hHandle);



/// <summary>׼�������ڻ��ֲ���ϸ.</summary>
int WINAPI CATSAPI_PreSub_FuturePositionDetail(CATSHANDLE hHandle);

/// <summary>�����ڻ��ֲ���ϸ.</summary>
int WINAPI CATSAPI_Subscribe_FuturePositionDetail(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>��ȡ�ڻ��ֲ���ϸ����.</summary>
int WINAPI CATSAPI_Fetch_FuturePositionDetailResult(CATSHANDLE hHandle);

/// <summary>��ȡ�ڻ��ֲ���ϸ֮�ֲ�����.</summary>
int WINAPI CATSAPI_Fetch_FuturePositionDetailResult_Position(CATSHANDLE hHandle);


////////////////////
// ���������ʲ����
////////////////////

/// <summary>׼�������ʲ������������.</summary>
int WINAPI CATSAPI_PreSub_CreditAssetUpdate(CATSHANDLE hHandle);

/// <summary>���������ʲ����.</summary>
int WINAPI CATSAPI_Subscribe_CreditAssetUpdate(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>��ȡ�����ʲ����½��.</summary>
int WINAPI CATSAPI_Fetch_CreditAssetUpdateResult(CATSHANDLE hHandle);

/// <summary>��ȡ�����ʲ��ĺ�ͬ���½��.</summary>
int WINAPI CATSAPI_Fetch_CreditAssetUpdate_CompactResult(CATSHANDLE hHandle);

////////////////////
// ����ί�гɽ�
////////////////////

/// <summary>׼��ί�гɽ���������.</summary>
int WINAPI CATSAPI_PreSub_OrderUpdate(CATSHANDLE hHandle);

/// <summary>����ί�гɽ�.</summary>
int WINAPI CATSAPI_Subscribe_OrderUpdate(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>��ȡί�гɽ����.</summary>
int WINAPI CATSAPI_Fetch_OrderUpdateResult(CATSHANDLE hHandle);

/// <summary>��ȡ�������½��.</summary>
int WINAPI CATSAPI_Fetch_OrderUpdateResult_Orders(CATSHANDLE hHandle);


/// <summary>׼����ѯ�ʲ���Ϣ����.</summary>
int WINAPI CATSAPI_Prepare_QueryAssetInfo(CATSHANDLE hHandle);

/// <summary>��ѯ�ʲ���Ϣ.</summary>
int WINAPI CATSAPI_Execute_QueryAssetInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ�ʲ���Ϣ������.</summary>
int WINAPI CATSAPI_Fetch_QueryAssetInfoResult(CATSHANDLE hHandle);

/// <summary>��ȡ�ʽ���Ϣ���.</summary>
int WINAPI CATSAPI_Fetch_QueryAssetInfoResult_FundAsset(CATSHANDLE hHandle);

/// <summary>��ȡ�ֲ���Ϣ���.</summary>
int WINAPI CATSAPI_Fetch_QueryAssetInfoResult_PositionAsset(CATSHANDLE hHandle);

/// <summary>׼����ѯί����Ϣ����.</summary>
int WINAPI CATSAPI_Prepare_QueryOrderInfo(CATSHANDLE hHandle);

/// <summary>��ѯί����Ϣ.</summary>
int WINAPI CATSAPI_Execute_QueryOrderInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯί����Ϣ������.</summary>
int WINAPI CATSAPI_Fetch_QueryOrderInfoResult(CATSHANDLE hHandle);

/// <summary>��ȡί����Ϣ���.</summary>
int WINAPI CATSAPI_Fetch_QueryOrderInfoResult_Orders(CATSHANDLE hHandle);


/// <summary>׼����ѯ�ͻ�������ȯ�����Ϣ.</summary>
int WINAPI CATSAPI_Prepare_QueryMarginTarget(CATSHANDLE hHandle);

/// <summary>��ѯ�ͻ�������ȯ�����Ϣ.</summary>
int WINAPI CATSAPI_Execute_QueryMarginTarget(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ�ͻ�������ȯ�����Ϣ���.</summary>
int WINAPI CATSAPI_Fetch_QueryMarginTargetResult(CATSHANDLE hHandle);

/// <summary>��ȡ�ͻ�������ȯ�����Ϣ���.</summary>
int WINAPI CATSAPI_Fetch_QueryMarginTargetResult_MarginTarget(CATSHANDLE hHandle);


/// <summary>׼����ѯ�ͻ�����Ʒ��Ϣ.</summary>
int WINAPI CATSAPI_Prepare_QueryAssureTarget(CATSHANDLE hHandle);

/// <summary>��ѯ�ͻ�����Ʒ��Ϣ.</summary>
int WINAPI CATSAPI_Execute_QueryAssureTarget(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ�ͻ�����Ʒ��Ϣ��ѯ���.</summary>
int WINAPI CATSAPI_Fetch_QueryAssureTargetResult(CATSHANDLE hHandle);


/// <summary>׼������ȯ��ת.</summary>
int WINAPI CATSAPI_Prepare_AssureSecurityTransfer(CATSHANDLE hHandle);

/// <summary>����ȯ��ת.</summary>
int WINAPI CATSAPI_Execute_AssureSecurityTransfer(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ����ȯ��ת���.</summary>
int WINAPI CATSAPI_Fetch_AssureSecurityTransferResult(CATSHANDLE hHandle);



/// <summary>׼����ѯ�ͻ������ʲ���Ϣ.</summary>
int WINAPI CATSAPI_Prepare_QueryCreditAsset(CATSHANDLE hHandle);

/// <summary>��ѯ�ͻ������ʲ���Ϣ.</summary>
int WINAPI CATSAPI_Execute_QueryCreditAsset(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ�ͻ������ʲ���Ϣ.</summary>
int WINAPI CATSAPI_Fetch_QueryCreditAssetResult(CATSHANDLE hHandle);



/// <summary>׼����ѯ�ͻ���ǰ��Լ��Ϣ.</summary>
int WINAPI CATSAPI_Prepare_QueryCreditCurrentCompact(CATSHANDLE hHandle);

/// <summary>��ѯ�ͻ���ǰ��Լ��Ϣ.</summary>
int WINAPI CATSAPI_Execute_QueryCreditCurrentCompact(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ�ͻ���ǰ��Լ��Ϣ���.</summary>
int WINAPI CATSAPI_Fetch_QueryCreditCurrentCompactResult(CATSHANDLE hHandle);

/// <summary>��ȡ��ѯ�ͻ���ǰ��Լ��Ϣ.</summary>
int WINAPI CATSAPI_Fetch_QueryCreditCurrentCompactResult_CreditCurrentCompact(CATSHANDLE hHandle);



/// <summary>׼����ѯ�ͻ�������ȯ��ʾ��Ϣ.</summary>
int WINAPI CATSAPI_Prepare_QueryCreditTipInfo(CATSHANDLE hHandle);

/// <summary>��ѯ�ͻ�������ȯ��ʾ��Ϣ.</summary>
int WINAPI CATSAPI_Execute_QueryCreditTipInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ�ͻ�������ȯ��ʾ��Ϣ.</summary>
int WINAPI CATSAPI_Fetch_QueryCreditTipInfoResult(CATSHANDLE hHandle);


/// <summary>׼����ѯ�ͻ�����ί������.</summary>
int WINAPI CATSAPI_Prepare_QueryMaxOrderVolume(CATSHANDLE hHandle);

/// <summary>��ѯ�ͻ�����ί������.</summary>
int WINAPI CATSAPI_Execute_QueryMaxOrderVolume(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ����ί��������Ϣ.</summary>
int WINAPI CATSAPI_Fetch_QueryMaxOrderVolumeResult(CATSHANDLE hHandle);


/// <summary>׼����ѯ����������ʱ��.</summary>
int WINAPI CATSAPI_Prepare_QueryTradeTime(CATSHANDLE hHandle);

/// <summary>��ѯ����������ʱ��.</summary>
int WINAPI CATSAPI_Execute_QueryTradeTime(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ����������ʱ����Ϣ.</summary>
int WINAPI CATSAPI_Fetch_QueryTradeTimeResult(CATSHANDLE hHandle);

///��Ϲ�����Ͻ���

/// <summary>׼����ѯ���.</summary>
int WINAPI CATSAPI_Prepare_QueryPortfolio(CATSHANDLE hHandle);

/// <summary>��ѯ���.</summary>
int WINAPI CATSAPI_Execute_QueryPortfolio(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ�����Ϣ.</summary>
int WINAPI CATSAPI_Fetch_QueryPortfolioResult(CATSHANDLE hHandle);


/// <summary>׼���������.</summary>
int WINAPI CATSAPI_Prepare_AddPortfolio(CATSHANDLE hHandle);

/// <summary>�������.</summary>
int WINAPI CATSAPI_Execute_AddPortfolio(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ������Ͻ��.</summary>
int WINAPI CATSAPI_Fetch_AddPortfolioResult(CATSHANDLE hHandle);


/// <summary>׼���������.</summary>
int WINAPI CATSAPI_Prepare_UpdatePortfolio(CATSHANDLE hHandle);

/// <summary>�������.</summary>
int WINAPI CATSAPI_Execute_UpdatePortfolio(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ������Ͻ��.</summary>
int WINAPI CATSAPI_Fetch_UpdatePortfolioResult(CATSHANDLE hHandle);



/// <summary>׼��ɾ�����.</summary>
int WINAPI CATSAPI_Prepare_DelPortfolio(CATSHANDLE hHandle);

/// <summary>ɾ�����.</summary>
int WINAPI CATSAPI_Execute_DelPortfolio(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡɾ����Ͻ��.</summary>
int WINAPI CATSAPI_Fetch_DelPortfolioResult(CATSHANDLE hHandle);



/// <summary>׼����ѯ��ϳ�Ա.</summary>
int WINAPI CATSAPI_Prepare_QueryPortfolioComponent(CATSHANDLE hHandle);

/// <summary>��ѯ��ϳ�Ա.</summary>
int WINAPI CATSAPI_Execute_QueryPortfolioComponent(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ��ϳ�Ա���.</summary>
int WINAPI CATSAPI_Fetch_QueryPortfolioComponentResult(CATSHANDLE hHandle);


/// <summary>׼�������ϳ�Ա.</summary>
int WINAPI CATSAPI_Prepare_AddPortfolioComponent(CATSHANDLE hHandle);

/// <summary>�����ϳ�Ա.</summary>
int WINAPI CATSAPI_Execute_AddPortfolioComponent(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ�����ϳ�Ա���.</summary>
int WINAPI CATSAPI_Fetch_AddPortfolioComponentResult(CATSHANDLE hHandle);



/// <summary>׼��������ϳ�Ա.</summary>
int WINAPI CATSAPI_Prepare_UpdatePortfolioComponent(CATSHANDLE hHandle);

/// <summary>������ϳ�Ա.</summary>
int WINAPI CATSAPI_Execute_UpdatePortfolioComponent(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ������ϳ�Ա���.</summary>
int WINAPI CATSAPI_Fetch_UpdatePortfolioComponentResult(CATSHANDLE hHandle);


/// <summary>׼��ɾ����ϳ�Ա.</summary>
int WINAPI CATSAPI_Prepare_DelPortfolioComponent(CATSHANDLE hHandle);

/// <summary>ɾ����ϳ�Ա.</summary>
int WINAPI CATSAPI_Execute_DelPortfolioComponent(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡɾ����ϳ�Ա���.</summary>
int WINAPI CATSAPI_Fetch_DelPortfolioComponentResult(CATSHANDLE hHandle);


/// <summary>׼�����ٽ����ʽ����.</summary>
int WINAPI CATSAPI_Prepare_QuickTrdFundTransfer(CATSHANDLE hHandle);

/// <summary>���ٽ����ʽ����.</summary>
int WINAPI CATSAPI_Execute_QuickTrdFundTransfer(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ���ٽ����ʽ�������.</summary>
int WINAPI CATSAPI_Fetch_QuickTrdFundTransferResult(CATSHANDLE hHandle);


/// <summary>׼��ϵͳ��Ϣ��������.</summary>
int WINAPI CATSAPI_PreSub_SystemNotifyEvent(CATSHANDLE hHandle);

/// <summary>����ϵͳ��Ϣ.</summary>
int WINAPI CATSAPI_Subscribe_SystemNotifyEvent(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>��ȡϵͳ��Ϣ���.</summary>
int WINAPI CATSAPI_Fetch_SystemNotifyEventResult(CATSHANDLE hHandle);


/// <summary>׼����ѯ��תծ��Ϣ.</summary>
int WINAPI CATSAPI_Prepare_QueryConvertibleBond(CATSHANDLE hHandle);

/// <summary>��ѯ��תծ��Ϣ.</summary>
int WINAPI CATSAPI_Execute_QueryConvertibleBond(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ��תծ���.</summary>
int WINAPI CATSAPI_Fetch_QueryConvertibleBondResult(CATSHANDLE hHandle);




/// <summary>׼������֤�����.</summary>
int WINAPI CATSAPI_Prepare_SetZJPortfolio(CATSHANDLE hHandle);

/// <summary>����֤�����.</summary>
int WINAPI CATSAPI_Execute_SetZJPortfolio(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ����֤����Ͻ��.</summary>
int WINAPI CATSAPI_Fetch_SetZJPortfolioResult(CATSHANDLE hHandle);


/// <summary>׼����ѯ֤�����.</summary>
int WINAPI CATSAPI_Prepare_QueryZJPortfolio(CATSHANDLE hHandle);

/// <summary>��ѯ֤�����.</summary>
int WINAPI CATSAPI_Execute_QueryZJPortfolio(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ֤����Ͻ��.</summary>
int WINAPI CATSAPI_Fetch_QueryZJPortfolioResult(CATSHANDLE hHandle);


/// <summary>׼����ѯ֤����ϳɷֹ�.</summary>
int WINAPI CATSAPI_Prepare_QueryZJComponentStock(CATSHANDLE hHandle);

/// <summary>��ѯ֤����ϳɷֹ�.</summary>
int WINAPI CATSAPI_Execute_QueryZJComponentStock(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ֤����ϳɷֹɽ��.</summary>
int WINAPI CATSAPI_Fetch_QueryZJComponentStockResult(CATSHANDLE hHandle);

/// <summary>׼�����֤����ϳɷֹ�.</summary>
int WINAPI CATSAPI_Prepare_AddZJComponentStock(CATSHANDLE hHandle);

/// <summary>���֤����ϳɷֹ�.</summary>
int WINAPI CATSAPI_Execute_AddZJComponentStock(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ���֤����ϳɷֹɽ��.</summary>
int WINAPI CATSAPI_Fetch_AddZJComponentStockResult(CATSHANDLE hHandle);


/// <summary>׼��ɾ��֤����ϳɷֹ�.</summary>
int WINAPI CATSAPI_Prepare_DeleteZJComponentStock(CATSHANDLE hHandle);

/// <summary>ɾ��֤����ϳɷֹ�.</summary>
int WINAPI CATSAPI_Execute_DeleteZJComponentStock(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡɾ��֤����ϳɷֹɽ��.</summary>
int WINAPI CATSAPI_Fetch_DeleteZJComponentStockResult(CATSHANDLE hHandle);

/// <summary>׼���޸�֤����ϳɷֹ�.</summary>
int WINAPI CATSAPI_Prepare_UpdateZJComponentStock(CATSHANDLE hHandle);

/// <summary>�޸�֤����ϳɷֹ�.</summary>
int WINAPI CATSAPI_Execute_UpdateZJComponentStock(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ�޸�֤����ϳɷֹɽ��.</summary>
int WINAPI CATSAPI_Fetch_UpdateZJComponentStockResult(CATSHANDLE hHandle);


/// <summary>׼����ѯ֤��������ϸ.</summary>
int WINAPI CATSAPI_Prepare_QueryZJArbitrageDetail(CATSHANDLE hHandle);

/// <summary>��ѯ֤��������ϸ.</summary>
int WINAPI CATSAPI_Execute_QueryZJArbitrageDetail(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ������ϸ���.</summary>
int WINAPI CATSAPI_Fetch_QueryZJArbitrageDetailResult(CATSHANDLE hHandle);


/// <summary>׼����ѯ֤������˻����ձ�.</summary>
int WINAPI CATSAPI_Prepare_QueryZJCatsAcctProductInfo(CATSHANDLE hHandle);

/// <summary>��ѯ֤������˻����ձ�.</summary>
int WINAPI CATSAPI_Execute_QueryZJCatsAcctProductInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ֤������˻����ձ���.</summary>
int WINAPI CATSAPI_Fetch_QueryZJCatsAcctProductResultInfo(CATSHANDLE hHandle);

/// <summary>׼������֤������˻����ձ�.</summary>
int WINAPI CATSAPI_Prepare_SetZJCatsAcctProductInfo(CATSHANDLE hHandle);

/// <summary>����֤��`�����˻����ձ�.</summary>
int WINAPI CATSAPI_Execute_SetZJCatsAcctProductInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ����֤������˻����ձ���.</summary>
int WINAPI CATSAPI_Fetch_SetZJCatsAcctProductInfoResult(CATSHANDLE hHandle);

/// <summary>׼����ѯ�����ʽ��˻����ձ�.</summary>
int WINAPI CATSAPI_Prepare_QueryZJProductFundAcctInfo(CATSHANDLE hHandle);

/// <summary>��ѯ�����ʽ��˻����ձ�.</summary>
int WINAPI CATSAPI_Execute_QueryZJProductFundAcctInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ�����ʽ��˻����ձ���.</summary>
int WINAPI CATSAPI_Fetch_QueryZJProductFundAcctInfoResult(CATSHANDLE hHandle);

/// <summary>׼�����ò����ʽ��˻����ձ�.</summary>
int WINAPI CATSAPI_Prepare_SetZJProductFundAcctInfo(CATSHANDLE hHandle);

/// <summary>���ò����ʽ��˻����ձ�.</summary>
int WINAPI CATSAPI_Execute_SetZJProductFundAcctInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ���ò����ʽ��˻����ձ���.</summary>
int WINAPI CATSAPI_Fetch_SetZJProductFundAcctInfoResult(CATSHANDLE hHandle);

/// <summary>׼������֤�����ָ��.</summary>
int WINAPI CATSAPI_PreSub_ZJRisk(CATSHANDLE hHandle);

/// <summary>����֤�����ָ��.</summary>
int WINAPI CATSAPI_Subscribe_ZJRisk(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>��ȡ֤�����ָ����.</summary>
int WINAPI CATSAPI_Fetch_PubZJRiskResult(CATSHANDLE hHandle);

/// <summary>��ȡ֤�����ָ��������.</summary>
int WINAPI CATSAPI_Fetch_PubZJRiskResult_Risks(CATSHANDLE hHandle);

/// <summary>׼��ȡ������֤�����ָ��.</summary>
int WINAPI CATSAPI_PreUnSub_ZJRisk(CATSHANDLE hHandle);

/// <summary>ȡ������֤�����ָ��.</summary>
int WINAPI CATSAPI_UnSubscribe_ZJRisk(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>׼����ѯ֤����տ��Ʊ�.</summary>
int WINAPI CATSAPI_Prepare_QueryZJRiskControl(CATSHANDLE hHandle);

/// <summary>��ѯ��ѯ֤����տ��Ʊ�.</summary>
int WINAPI CATSAPI_Execute_QueryZJRiskControl(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ֤����տ��Ʊ���.</summary>
int WINAPI CATSAPI_Fetch_QueryZJRiskControlResult(CATSHANDLE hHandle);

/// <summary>׼������֤����տ��Ʊ�.</summary>
int WINAPI CATSAPI_Prepare_SaveZJRiskControl(CATSHANDLE hHandle);

/// <summary>����֤����տ��Ʊ�.</summary>
int WINAPI CATSAPI_Execute_SaveZJRiskControl(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ����֤����տ��Ʊ���.</summary>
int WINAPI CATSAPI_Fetch_SaveZJRiskControlResult(CATSHANDLE hHandle);

/// <summary>׼��ɾ��֤����տ��Ʊ�.</summary>
int WINAPI CATSAPI_Prepare_DeleteZJRiskControl(CATSHANDLE hHandle);

/// <summary>ɾ��֤����տ��Ʊ�.</summary>
int WINAPI CATSAPI_Execute_DeleteZJRiskControl(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡɾ��֤����տ��Ʊ���.</summary>
int WINAPI CATSAPI_Fetch_DeleteZJRiskControlResult(CATSHANDLE hHandle);



/// <summary>׼�����ø�ҹί��.</summary>
int WINAPI CATSAPI_Prepare_SetOvernightOrder(CATSHANDLE hHandle);

/// <summary>���ø�ҹί��.</summary>
int WINAPI CATSAPI_Execute_SetOvernightOrder(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ���ø�ҹί�н��.</summary>
int WINAPI CATSAPI_Fetch_SetOvernightOrderResult(CATSHANDLE hHandle);


/// <summary>׼����ѯ��ҹί��.</summary>
int WINAPI CATSAPI_Prepare_QueryOvernightOrder(CATSHANDLE hHandle);

/// <summary>��ѯ��ҹί��.</summary>
int WINAPI CATSAPI_Execute_QueryOvernightOrder(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ��ҹί�н��.</summary>
int WINAPI CATSAPI_Fetch_QueryOvernightOrderResult(CATSHANDLE hHandle);

/// <summary>׼����ѯETF��Ϣ.</summary>
int WINAPI CATSAPI_Prepare_QueryETFInfo(CATSHANDLE hHandle);

/// <summary>��ѯETF��Ϣ.</summary>
int WINAPI CATSAPI_Execute_QueryETFInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯETF��Ϣ���.</summary>
int WINAPI CATSAPI_Fetch_QueryETFInfoResult(CATSHANDLE hHandle);

/// <summary>��ȡ��ѯETF�ɷֹ���Ϣ���.</summary>
int WINAPI CATSAPI_Fetch_QueryETFInfoResult_ComponentsInfo(CATSHANDLE hHandle);


/// <summary>׼����ѯETF�б���Ϣ.</summary>
int WINAPI CATSAPI_Prepare_QueryETFList(CATSHANDLE hHandle);

/// <summary>��ѯETF�б���Ϣ.</summary>
int WINAPI CATSAPI_Execute_QueryETFList(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯETF�б���Ϣ���.</summary>
int WINAPI CATSAPI_Fetch_QueryETFListResult(CATSHANDLE hHandle);

/// <summary>׼������ת������.</summary>
int WINAPI CATSAPI_Prepare_BankTransfer(CATSHANDLE hHandle);

/// <summary>����ת������.</summary>
int WINAPI CATSAPI_Execute_BankTransfer(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ����ת��������.</summary>
int WINAPI CATSAPI_Fetch_BankTransferResult(CATSHANDLE hHandle);


/// <summary>׼����ѯ�ּ�������Ϣ.</summary>
int WINAPI CATSAPI_Prepare_QueryGradeFundInfo(CATSHANDLE hHandle);

/// <summary>��ѯ�ּ�������Ϣ.</summary>
int WINAPI CATSAPI_Execute_QueryGradeFundInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ�ּ�������Ϣ���.</summary>
int WINAPI CATSAPI_Fetch_QueryGradeFundInfoResult(CATSHANDLE hHandle);



/// <summary>׼����ѯͨ���������.</summary>
int WINAPI CATSAPI_Prepare_QueryCommonPortfolio(CATSHANDLE hHandle);

/// <summary>��ѯͨ���������.</summary>
int WINAPI CATSAPI_Execute_QueryCommonPortfolio(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯͨ�����������.</summary>
int WINAPI CATSAPI_Fetch_QueryCommonPortfolioResult(CATSHANDLE hHandle);


/// <summary>׼����ѯͨ����ϱ������.</summary>
int WINAPI CATSAPI_Prepare_QueryCommonComponent(CATSHANDLE hHandle);

/// <summary>��ѯͨ����ϱ������.</summary>
int WINAPI CATSAPI_Execute_QueryCommonComponent(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯͨ����ϱ��������.</summary>
int WINAPI CATSAPI_Fetch_QueryCommonComponentResult(CATSHANDLE hHandle);


/// <summary>׼������ͨ���������.</summary>
int WINAPI CATSAPI_Prepare_SaveCommonPortfolio(CATSHANDLE hHandle);

/// <summary>����ͨ���������.</summary>
int WINAPI CATSAPI_Execute_SaveCommonPortfolio(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ����ͨ�����������.</summary>
int WINAPI CATSAPI_Fetch_SaveCommonPortfolioResult(CATSHANDLE hHandle);


/// <summary>׼��ɾ��ͨ���������.</summary>
int WINAPI CATSAPI_Prepare_DeleteCommonPortfolio(CATSHANDLE hHandle);

/// <summary>ɾ��ͨ���������.</summary>
int WINAPI CATSAPI_Execute_DeleteCommonPortfolio(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡɾ��ͨ�����������.</summary>
int WINAPI CATSAPI_Fetch_DeleteCommonPortfolioResult(CATSHANDLE hHandle);


/// <summary>׼���ɽ���ϸ����.</summary>
int WINAPI CATSAPI_PreSub_Trade(CATSHANDLE hHandle);

/// <summary>�ɽ���ϸ����.</summary>
int WINAPI CATSAPI_Subscribe_Trade(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>��ȡ�ɽ���ϸ������.</summary>
int WINAPI CATSAPI_Fetch_TradeResult(CATSHANDLE hHandle);


/// <summary>׼���ɽ���ϸ����.</summary>
int WINAPI CATSAPI_PreUnSub_Trade(CATSHANDLE hHandle);

/// <summary>�ɽ���ϸ����.</summary>
int WINAPI CATSAPI_UnSubscribe_Trade(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ�ɽ���ϸ������.</summary>
int WINAPI CATSAPI_Fetch_UnSubTradeResult(CATSHANDLE hHandle);

# endif

#ifdef   __cplusplus
}
#endif
