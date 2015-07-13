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
// 单笔委托
////////////////////

/// <summary>准备单笔委托请求.</summary>
int WINAPI CATSAPI_Prepare_OrderSingle(CATSHANDLE hHandle);

/// <summary>单笔委托.</summary>
int WINAPI CATSAPI_Execute_OrderSingle(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取单笔委托结果.</summary>
int WINAPI CATSAPI_Fetch_OrderSingleResult(CATSHANDLE hHandle);

////////////////////
// 智能委托
////////////////////

/// <summary>准备智能委托请求.</summary>
int WINAPI CATSAPI_Prepare_SmartOrder(CATSHANDLE hHandle);

/// <summary>智能委托.</summary>
int WINAPI CATSAPI_Execute_SmartOrder(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取智能委托结果.</summary>
int WINAPI CATSAPI_Fetch_SmartOrderResult(CATSHANDLE hHandle);

////////////////////
// 单笔委托撤单
////////////////////

/// <summary>准备单笔委托撤单请求.</summary>
int WINAPI CATSAPI_Prepare_WithDrawSingle(CATSHANDLE hHandle);

/// <summary>单笔委托撤单.</summary>
int WINAPI CATSAPI_Execute_WithDrawSingle(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取单笔委托撤单结果.</summary>
int WINAPI CATSAPI_Fetch_WithDrawSingleResult(CATSHANDLE hHandle);

////////////////////
// 组合委托
////////////////////

/// <summary>准备组合委托请求.</summary>
int WINAPI CATSAPI_Prepare_OrderBatch(CATSHANDLE hHandle);

/// <summary>组合委托.</summary>
int WINAPI CATSAPI_Execute_OrderBatch(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取组合委托结果.</summary>
int WINAPI CATSAPI_Fetch_OrderBatchResult(CATSHANDLE hHandle);

/// <summary>提取组合委托委托明细结果.</summary>
int WINAPI CATSAPI_Fetch_OrderBatchResult_OrderNos(CATSHANDLE hHandle);


/// <summary>准备组合委托请求.</summary>
int WINAPI CATSAPI_Prepare_OrderBatchByOrderSingle(CATSHANDLE hHandle);

/// <summary>组合委托.</summary>
int WINAPI CATSAPI_Execute_OrderBatchByOrderSingle(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取组合委托结果.</summary>
int WINAPI CATSAPI_Fetch_OrderBatchByOrderSingleResult(CATSHANDLE hHandle);

/// <summary>提取组合委托委托明细结果.</summary>
int WINAPI CATSAPI_Fetch_OrderBatchByOrderSingleResult_OrderNos(CATSHANDLE hHandle);

////////////////////
// 组合委托撤单
////////////////////

/// <summary>准备组合委托撤单请求.</summary>
int WINAPI CATSAPI_Prepare_WithDrawBatch(CATSHANDLE hHandle);

/// <summary>组合委托撤单.</summary>
int WINAPI CATSAPI_Execute_WithDrawBatch(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取组合委托撤单结果.</summary>
int WINAPI CATSAPI_Fetch_WithDrawBatchResult(CATSHANDLE hHandle);


////////////////////
// 批量根据委托号撤单
////////////////////

/// <summary>准备批量委托撤单请求.</summary>
int WINAPI CATSAPI_Prepare_WithDrawBatchByOrderNos(CATSHANDLE hHandle);

/// <summary>批量委托撤单.</summary>
int WINAPI CATSAPI_Execute_WithDrawBatchByOrderNos(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取批量委托撤单结果.</summary>
int WINAPI CATSAPI_Fetch_WithDrawBatchByOrderNosResult(CATSHANDLE hHandle);


////////////////////
// 现金还款
////////////////////

/// <summary>准备现金还款请求.</summary>
int WINAPI CATSAPI_Prepare_DirectReturnLoan(CATSHANDLE hHandle);

/// <summary>现金还款.</summary>
int WINAPI CATSAPI_Execute_DirectReturnLoan(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取现金还款结果.</summary>
int WINAPI CATSAPI_Fetch_DirectReturnLoanResult(CATSHANDLE hHandle);

////////////////////
// 现券还券
////////////////////

/// <summary>准备现券还券请求.</summary>
int WINAPI CATSAPI_Prepare_DirectReturnStock(CATSHANDLE hHandle);

/// <summary>现券还券.</summary>
int WINAPI CATSAPI_Execute_DirectReturnStock(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取现券还券结果.</summary>
int WINAPI CATSAPI_Fetch_DirectReturnStockResult(CATSHANDLE hHandle);

////////////////////
// 订阅资金及持仓变动
////////////////////

/// <summary>准备资金及持仓变动订阅请求.</summary>
int WINAPI CATSAPI_PreSub_AssetUpdate(CATSHANDLE hHandle);

/// <summary>订阅资金及持仓变动.</summary>
int WINAPI CATSAPI_Subscribe_AssetUpdate(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>提取资金及持仓变动结果.</summary>
int WINAPI CATSAPI_Fetch_AssetUpdateResult(CATSHANDLE hHandle);

/// <summary>提取资金结果.</summary>
int WINAPI CATSAPI_Fetch_AssetUpdateResult_Funds(CATSHANDLE hHandle);

/// <summary>提取持仓结果.</summary>
int WINAPI CATSAPI_Fetch_AssetUpdateResult_Positions(CATSHANDLE hHandle);



/// <summary>准备订阅期货持仓明细.</summary>
int WINAPI CATSAPI_PreSub_FuturePositionDetail(CATSHANDLE hHandle);

/// <summary>订阅期货持仓明细.</summary>
int WINAPI CATSAPI_Subscribe_FuturePositionDetail(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>提取期货持仓明细数据.</summary>
int WINAPI CATSAPI_Fetch_FuturePositionDetailResult(CATSHANDLE hHandle);

/// <summary>提取期货持仓明细之持仓数据.</summary>
int WINAPI CATSAPI_Fetch_FuturePositionDetailResult_Position(CATSHANDLE hHandle);


////////////////////
// 订阅信用资产变更
////////////////////

/// <summary>准备信用资产变更订阅请求.</summary>
int WINAPI CATSAPI_PreSub_CreditAssetUpdate(CATSHANDLE hHandle);

/// <summary>订阅信用资产变更.</summary>
int WINAPI CATSAPI_Subscribe_CreditAssetUpdate(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>提取信用资产更新结果.</summary>
int WINAPI CATSAPI_Fetch_CreditAssetUpdateResult(CATSHANDLE hHandle);

/// <summary>提取信用资产的合同更新结果.</summary>
int WINAPI CATSAPI_Fetch_CreditAssetUpdate_CompactResult(CATSHANDLE hHandle);

////////////////////
// 订阅委托成交
////////////////////

/// <summary>准备委托成交订阅请求.</summary>
int WINAPI CATSAPI_PreSub_OrderUpdate(CATSHANDLE hHandle);

/// <summary>订阅委托成交.</summary>
int WINAPI CATSAPI_Subscribe_OrderUpdate(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>提取委托成交结果.</summary>
int WINAPI CATSAPI_Fetch_OrderUpdateResult(CATSHANDLE hHandle);

/// <summary>提取订单更新结果.</summary>
int WINAPI CATSAPI_Fetch_OrderUpdateResult_Orders(CATSHANDLE hHandle);


/// <summary>准备查询资产信息请求.</summary>
int WINAPI CATSAPI_Prepare_QueryAssetInfo(CATSHANDLE hHandle);

/// <summary>查询资产信息.</summary>
int WINAPI CATSAPI_Execute_QueryAssetInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询资产信息请求结果.</summary>
int WINAPI CATSAPI_Fetch_QueryAssetInfoResult(CATSHANDLE hHandle);

/// <summary>提取资金信息结果.</summary>
int WINAPI CATSAPI_Fetch_QueryAssetInfoResult_FundAsset(CATSHANDLE hHandle);

/// <summary>提取持仓信息结果.</summary>
int WINAPI CATSAPI_Fetch_QueryAssetInfoResult_PositionAsset(CATSHANDLE hHandle);

/// <summary>准备查询委托信息请求.</summary>
int WINAPI CATSAPI_Prepare_QueryOrderInfo(CATSHANDLE hHandle);

/// <summary>查询委托信息.</summary>
int WINAPI CATSAPI_Execute_QueryOrderInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询委托信息请求结果.</summary>
int WINAPI CATSAPI_Fetch_QueryOrderInfoResult(CATSHANDLE hHandle);

/// <summary>提取委托信息结果.</summary>
int WINAPI CATSAPI_Fetch_QueryOrderInfoResult_Orders(CATSHANDLE hHandle);


/// <summary>准备查询客户融资融券标的信息.</summary>
int WINAPI CATSAPI_Prepare_QueryMarginTarget(CATSHANDLE hHandle);

/// <summary>查询客户融资融券标的信息.</summary>
int WINAPI CATSAPI_Execute_QueryMarginTarget(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询客户融资融券标的信息结果.</summary>
int WINAPI CATSAPI_Fetch_QueryMarginTargetResult(CATSHANDLE hHandle);

/// <summary>提取客户融资融券标的信息结果.</summary>
int WINAPI CATSAPI_Fetch_QueryMarginTargetResult_MarginTarget(CATSHANDLE hHandle);


/// <summary>准备查询客户担保品信息.</summary>
int WINAPI CATSAPI_Prepare_QueryAssureTarget(CATSHANDLE hHandle);

/// <summary>查询客户担保品信息.</summary>
int WINAPI CATSAPI_Execute_QueryAssureTarget(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询客户担保品信息查询结果.</summary>
int WINAPI CATSAPI_Fetch_QueryAssureTargetResult(CATSHANDLE hHandle);


/// <summary>准备担保券划转.</summary>
int WINAPI CATSAPI_Prepare_AssureSecurityTransfer(CATSHANDLE hHandle);

/// <summary>担保券划转.</summary>
int WINAPI CATSAPI_Execute_AssureSecurityTransfer(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取担保券划转结果.</summary>
int WINAPI CATSAPI_Fetch_AssureSecurityTransferResult(CATSHANDLE hHandle);



/// <summary>准备查询客户信用资产信息.</summary>
int WINAPI CATSAPI_Prepare_QueryCreditAsset(CATSHANDLE hHandle);

/// <summary>查询客户信用资产信息.</summary>
int WINAPI CATSAPI_Execute_QueryCreditAsset(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询客户信用资产信息.</summary>
int WINAPI CATSAPI_Fetch_QueryCreditAssetResult(CATSHANDLE hHandle);



/// <summary>准备查询客户当前合约信息.</summary>
int WINAPI CATSAPI_Prepare_QueryCreditCurrentCompact(CATSHANDLE hHandle);

/// <summary>查询客户当前合约信息.</summary>
int WINAPI CATSAPI_Execute_QueryCreditCurrentCompact(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询客户当前合约信息结果.</summary>
int WINAPI CATSAPI_Fetch_QueryCreditCurrentCompactResult(CATSHANDLE hHandle);

/// <summary>提取查询客户当前合约信息.</summary>
int WINAPI CATSAPI_Fetch_QueryCreditCurrentCompactResult_CreditCurrentCompact(CATSHANDLE hHandle);



/// <summary>准备查询客户融资融券提示信息.</summary>
int WINAPI CATSAPI_Prepare_QueryCreditTipInfo(CATSHANDLE hHandle);

/// <summary>查询客户融资融券提示信息.</summary>
int WINAPI CATSAPI_Execute_QueryCreditTipInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询客户融资融券提示信息.</summary>
int WINAPI CATSAPI_Fetch_QueryCreditTipInfoResult(CATSHANDLE hHandle);


/// <summary>准备查询客户最大可委托数量.</summary>
int WINAPI CATSAPI_Prepare_QueryMaxOrderVolume(CATSHANDLE hHandle);

/// <summary>查询客户最大可委托数量.</summary>
int WINAPI CATSAPI_Execute_QueryMaxOrderVolume(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询最大可委托数量信息.</summary>
int WINAPI CATSAPI_Fetch_QueryMaxOrderVolumeResult(CATSHANDLE hHandle);


/// <summary>准备查询交易所交易时间.</summary>
int WINAPI CATSAPI_Prepare_QueryTradeTime(CATSHANDLE hHandle);

/// <summary>查询交易所交易时间.</summary>
int WINAPI CATSAPI_Execute_QueryTradeTime(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询交易所交易时间信息.</summary>
int WINAPI CATSAPI_Fetch_QueryTradeTimeResult(CATSHANDLE hHandle);

///组合管理及组合交易

/// <summary>准备查询组合.</summary>
int WINAPI CATSAPI_Prepare_QueryPortfolio(CATSHANDLE hHandle);

/// <summary>查询组合.</summary>
int WINAPI CATSAPI_Execute_QueryPortfolio(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询组合信息.</summary>
int WINAPI CATSAPI_Fetch_QueryPortfolioResult(CATSHANDLE hHandle);


/// <summary>准备新增组合.</summary>
int WINAPI CATSAPI_Prepare_AddPortfolio(CATSHANDLE hHandle);

/// <summary>新增组合.</summary>
int WINAPI CATSAPI_Execute_AddPortfolio(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取新增组合结果.</summary>
int WINAPI CATSAPI_Fetch_AddPortfolioResult(CATSHANDLE hHandle);


/// <summary>准备更新组合.</summary>
int WINAPI CATSAPI_Prepare_UpdatePortfolio(CATSHANDLE hHandle);

/// <summary>更新组合.</summary>
int WINAPI CATSAPI_Execute_UpdatePortfolio(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取更新组合结果.</summary>
int WINAPI CATSAPI_Fetch_UpdatePortfolioResult(CATSHANDLE hHandle);



/// <summary>准备删除组合.</summary>
int WINAPI CATSAPI_Prepare_DelPortfolio(CATSHANDLE hHandle);

/// <summary>删除组合.</summary>
int WINAPI CATSAPI_Execute_DelPortfolio(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取删除组合结果.</summary>
int WINAPI CATSAPI_Fetch_DelPortfolioResult(CATSHANDLE hHandle);



/// <summary>准备查询组合成员.</summary>
int WINAPI CATSAPI_Prepare_QueryPortfolioComponent(CATSHANDLE hHandle);

/// <summary>查询组合成员.</summary>
int WINAPI CATSAPI_Execute_QueryPortfolioComponent(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询组合成员结果.</summary>
int WINAPI CATSAPI_Fetch_QueryPortfolioComponentResult(CATSHANDLE hHandle);


/// <summary>准备添加组合成员.</summary>
int WINAPI CATSAPI_Prepare_AddPortfolioComponent(CATSHANDLE hHandle);

/// <summary>添加组合成员.</summary>
int WINAPI CATSAPI_Execute_AddPortfolioComponent(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取添加组合成员结果.</summary>
int WINAPI CATSAPI_Fetch_AddPortfolioComponentResult(CATSHANDLE hHandle);



/// <summary>准备更新组合成员.</summary>
int WINAPI CATSAPI_Prepare_UpdatePortfolioComponent(CATSHANDLE hHandle);

/// <summary>更新组合成员.</summary>
int WINAPI CATSAPI_Execute_UpdatePortfolioComponent(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取更新组合成员结果.</summary>
int WINAPI CATSAPI_Fetch_UpdatePortfolioComponentResult(CATSHANDLE hHandle);


/// <summary>准备删除组合成员.</summary>
int WINAPI CATSAPI_Prepare_DelPortfolioComponent(CATSHANDLE hHandle);

/// <summary>删除组合成员.</summary>
int WINAPI CATSAPI_Execute_DelPortfolioComponent(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取删除组合成员结果.</summary>
int WINAPI CATSAPI_Fetch_DelPortfolioComponentResult(CATSHANDLE hHandle);


/// <summary>准备快速交易资金调拨.</summary>
int WINAPI CATSAPI_Prepare_QuickTrdFundTransfer(CATSHANDLE hHandle);

/// <summary>快速交易资金调拨.</summary>
int WINAPI CATSAPI_Execute_QuickTrdFundTransfer(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取快速交易资金调拨结果.</summary>
int WINAPI CATSAPI_Fetch_QuickTrdFundTransferResult(CATSHANDLE hHandle);


/// <summary>准备系统消息订阅请求.</summary>
int WINAPI CATSAPI_PreSub_SystemNotifyEvent(CATSHANDLE hHandle);

/// <summary>订阅系统消息.</summary>
int WINAPI CATSAPI_Subscribe_SystemNotifyEvent(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>提取系统消息结果.</summary>
int WINAPI CATSAPI_Fetch_SystemNotifyEventResult(CATSHANDLE hHandle);


/// <summary>准备查询可转债信息.</summary>
int WINAPI CATSAPI_Prepare_QueryConvertibleBond(CATSHANDLE hHandle);

/// <summary>查询可转债信息.</summary>
int WINAPI CATSAPI_Execute_QueryConvertibleBond(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询可转债结果.</summary>
int WINAPI CATSAPI_Fetch_QueryConvertibleBondResult(CATSHANDLE hHandle);




/// <summary>准备设置证金组合.</summary>
int WINAPI CATSAPI_Prepare_SetZJPortfolio(CATSHANDLE hHandle);

/// <summary>设置证金组合.</summary>
int WINAPI CATSAPI_Execute_SetZJPortfolio(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取设置证金组合结果.</summary>
int WINAPI CATSAPI_Fetch_SetZJPortfolioResult(CATSHANDLE hHandle);


/// <summary>准备查询证金组合.</summary>
int WINAPI CATSAPI_Prepare_QueryZJPortfolio(CATSHANDLE hHandle);

/// <summary>查询证金组合.</summary>
int WINAPI CATSAPI_Execute_QueryZJPortfolio(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询证金组合结果.</summary>
int WINAPI CATSAPI_Fetch_QueryZJPortfolioResult(CATSHANDLE hHandle);


/// <summary>准备查询证金组合成分股.</summary>
int WINAPI CATSAPI_Prepare_QueryZJComponentStock(CATSHANDLE hHandle);

/// <summary>查询证金组合成分股.</summary>
int WINAPI CATSAPI_Execute_QueryZJComponentStock(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询证金组合成分股结果.</summary>
int WINAPI CATSAPI_Fetch_QueryZJComponentStockResult(CATSHANDLE hHandle);

/// <summary>准备添加证金组合成分股.</summary>
int WINAPI CATSAPI_Prepare_AddZJComponentStock(CATSHANDLE hHandle);

/// <summary>添加证金组合成分股.</summary>
int WINAPI CATSAPI_Execute_AddZJComponentStock(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取添加证金组合成分股结果.</summary>
int WINAPI CATSAPI_Fetch_AddZJComponentStockResult(CATSHANDLE hHandle);


/// <summary>准备删除证金组合成分股.</summary>
int WINAPI CATSAPI_Prepare_DeleteZJComponentStock(CATSHANDLE hHandle);

/// <summary>删除证金组合成分股.</summary>
int WINAPI CATSAPI_Execute_DeleteZJComponentStock(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取删除证金组合成分股结果.</summary>
int WINAPI CATSAPI_Fetch_DeleteZJComponentStockResult(CATSHANDLE hHandle);

/// <summary>准备修改证金组合成分股.</summary>
int WINAPI CATSAPI_Prepare_UpdateZJComponentStock(CATSHANDLE hHandle);

/// <summary>修改证金组合成分股.</summary>
int WINAPI CATSAPI_Execute_UpdateZJComponentStock(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取修改证金组合成分股结果.</summary>
int WINAPI CATSAPI_Fetch_UpdateZJComponentStockResult(CATSHANDLE hHandle);


/// <summary>准备查询证金套利明细.</summary>
int WINAPI CATSAPI_Prepare_QueryZJArbitrageDetail(CATSHANDLE hHandle);

/// <summary>查询证金套利明细.</summary>
int WINAPI CATSAPI_Execute_QueryZJArbitrageDetail(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询套利明细结果.</summary>
int WINAPI CATSAPI_Fetch_QueryZJArbitrageDetailResult(CATSHANDLE hHandle);


/// <summary>准备查询证金策略账户对照表.</summary>
int WINAPI CATSAPI_Prepare_QueryZJCatsAcctProductInfo(CATSHANDLE hHandle);

/// <summary>查询证金策略账户对照表.</summary>
int WINAPI CATSAPI_Execute_QueryZJCatsAcctProductInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取证金策略账户对照表结果.</summary>
int WINAPI CATSAPI_Fetch_QueryZJCatsAcctProductResultInfo(CATSHANDLE hHandle);

/// <summary>准备设置证金策略账户对照表.</summary>
int WINAPI CATSAPI_Prepare_SetZJCatsAcctProductInfo(CATSHANDLE hHandle);

/// <summary>设置证金`策略账户对照表.</summary>
int WINAPI CATSAPI_Execute_SetZJCatsAcctProductInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取设置证金策略账户对照表结果.</summary>
int WINAPI CATSAPI_Fetch_SetZJCatsAcctProductInfoResult(CATSHANDLE hHandle);

/// <summary>准备查询策略资金账户对照表.</summary>
int WINAPI CATSAPI_Prepare_QueryZJProductFundAcctInfo(CATSHANDLE hHandle);

/// <summary>查询策略资金账户对照表.</summary>
int WINAPI CATSAPI_Execute_QueryZJProductFundAcctInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询策略资金账户对照表结果.</summary>
int WINAPI CATSAPI_Fetch_QueryZJProductFundAcctInfoResult(CATSHANDLE hHandle);

/// <summary>准备设置策略资金账户对照表.</summary>
int WINAPI CATSAPI_Prepare_SetZJProductFundAcctInfo(CATSHANDLE hHandle);

/// <summary>设置策略资金账户对照表.</summary>
int WINAPI CATSAPI_Execute_SetZJProductFundAcctInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取设置策略资金账户对照表结果.</summary>
int WINAPI CATSAPI_Fetch_SetZJProductFundAcctInfoResult(CATSHANDLE hHandle);

/// <summary>准备订阅证金风险指标.</summary>
int WINAPI CATSAPI_PreSub_ZJRisk(CATSHANDLE hHandle);

/// <summary>订阅证金风险指标.</summary>
int WINAPI CATSAPI_Subscribe_ZJRisk(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>提取证金风险指标结果.</summary>
int WINAPI CATSAPI_Fetch_PubZJRiskResult(CATSHANDLE hHandle);

/// <summary>提取证金风险指标结果详情.</summary>
int WINAPI CATSAPI_Fetch_PubZJRiskResult_Risks(CATSHANDLE hHandle);

/// <summary>准备取消订阅证金风险指标.</summary>
int WINAPI CATSAPI_PreUnSub_ZJRisk(CATSHANDLE hHandle);

/// <summary>取消订阅证金风险指标.</summary>
int WINAPI CATSAPI_UnSubscribe_ZJRisk(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>准备查询证金风险控制表.</summary>
int WINAPI CATSAPI_Prepare_QueryZJRiskControl(CATSHANDLE hHandle);

/// <summary>查询查询证金风险控制表.</summary>
int WINAPI CATSAPI_Execute_QueryZJRiskControl(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询证金风险控制表结果.</summary>
int WINAPI CATSAPI_Fetch_QueryZJRiskControlResult(CATSHANDLE hHandle);

/// <summary>准备设置证金风险控制表.</summary>
int WINAPI CATSAPI_Prepare_SaveZJRiskControl(CATSHANDLE hHandle);

/// <summary>设置证金风险控制表.</summary>
int WINAPI CATSAPI_Execute_SaveZJRiskControl(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取设置证金风险控制表结果.</summary>
int WINAPI CATSAPI_Fetch_SaveZJRiskControlResult(CATSHANDLE hHandle);

/// <summary>准备删除证金风险控制表.</summary>
int WINAPI CATSAPI_Prepare_DeleteZJRiskControl(CATSHANDLE hHandle);

/// <summary>删除证金风险控制表.</summary>
int WINAPI CATSAPI_Execute_DeleteZJRiskControl(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取删除证金风险控制表结果.</summary>
int WINAPI CATSAPI_Fetch_DeleteZJRiskControlResult(CATSHANDLE hHandle);



/// <summary>准备设置隔夜委托.</summary>
int WINAPI CATSAPI_Prepare_SetOvernightOrder(CATSHANDLE hHandle);

/// <summary>设置隔夜委托.</summary>
int WINAPI CATSAPI_Execute_SetOvernightOrder(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取设置隔夜委托结果.</summary>
int WINAPI CATSAPI_Fetch_SetOvernightOrderResult(CATSHANDLE hHandle);


/// <summary>准备查询隔夜委托.</summary>
int WINAPI CATSAPI_Prepare_QueryOvernightOrder(CATSHANDLE hHandle);

/// <summary>查询隔夜委托.</summary>
int WINAPI CATSAPI_Execute_QueryOvernightOrder(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询隔夜委托结果.</summary>
int WINAPI CATSAPI_Fetch_QueryOvernightOrderResult(CATSHANDLE hHandle);

/// <summary>准备查询ETF信息.</summary>
int WINAPI CATSAPI_Prepare_QueryETFInfo(CATSHANDLE hHandle);

/// <summary>查询ETF信息.</summary>
int WINAPI CATSAPI_Execute_QueryETFInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询ETF信息结果.</summary>
int WINAPI CATSAPI_Fetch_QueryETFInfoResult(CATSHANDLE hHandle);

/// <summary>提取查询ETF成分股信息结果.</summary>
int WINAPI CATSAPI_Fetch_QueryETFInfoResult_ComponentsInfo(CATSHANDLE hHandle);


/// <summary>准备查询ETF列表信息.</summary>
int WINAPI CATSAPI_Prepare_QueryETFList(CATSHANDLE hHandle);

/// <summary>查询ETF列表信息.</summary>
int WINAPI CATSAPI_Execute_QueryETFList(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询ETF列表信息结果.</summary>
int WINAPI CATSAPI_Fetch_QueryETFListResult(CATSHANDLE hHandle);

/// <summary>准备银行转账请求.</summary>
int WINAPI CATSAPI_Prepare_BankTransfer(CATSHANDLE hHandle);

/// <summary>银行转账请求.</summary>
int WINAPI CATSAPI_Execute_BankTransfer(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取银行转账请求结果.</summary>
int WINAPI CATSAPI_Fetch_BankTransferResult(CATSHANDLE hHandle);


/// <summary>准备查询分级基金信息.</summary>
int WINAPI CATSAPI_Prepare_QueryGradeFundInfo(CATSHANDLE hHandle);

/// <summary>查询分级基金信息.</summary>
int WINAPI CATSAPI_Execute_QueryGradeFundInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询分级基金信息结果.</summary>
int WINAPI CATSAPI_Fetch_QueryGradeFundInfoResult(CATSHANDLE hHandle);



/// <summary>准备查询通用组合请求.</summary>
int WINAPI CATSAPI_Prepare_QueryCommonPortfolio(CATSHANDLE hHandle);

/// <summary>查询通用组合请求.</summary>
int WINAPI CATSAPI_Execute_QueryCommonPortfolio(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询通用组合请求结果.</summary>
int WINAPI CATSAPI_Fetch_QueryCommonPortfolioResult(CATSHANDLE hHandle);


/// <summary>准备查询通用组合标的请求.</summary>
int WINAPI CATSAPI_Prepare_QueryCommonComponent(CATSHANDLE hHandle);

/// <summary>查询通用组合标的请求.</summary>
int WINAPI CATSAPI_Execute_QueryCommonComponent(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询通用组合标的请求结果.</summary>
int WINAPI CATSAPI_Fetch_QueryCommonComponentResult(CATSHANDLE hHandle);


/// <summary>准备保存通用组合请求.</summary>
int WINAPI CATSAPI_Prepare_SaveCommonPortfolio(CATSHANDLE hHandle);

/// <summary>保存通用组合请求.</summary>
int WINAPI CATSAPI_Execute_SaveCommonPortfolio(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取保存通用组合请求结果.</summary>
int WINAPI CATSAPI_Fetch_SaveCommonPortfolioResult(CATSHANDLE hHandle);


/// <summary>准备删除通用组合请求.</summary>
int WINAPI CATSAPI_Prepare_DeleteCommonPortfolio(CATSHANDLE hHandle);

/// <summary>删除通用组合请求.</summary>
int WINAPI CATSAPI_Execute_DeleteCommonPortfolio(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取删除通用组合请求结果.</summary>
int WINAPI CATSAPI_Fetch_DeleteCommonPortfolioResult(CATSHANDLE hHandle);


/// <summary>准备成交明细请求.</summary>
int WINAPI CATSAPI_PreSub_Trade(CATSHANDLE hHandle);

/// <summary>成交明细请求.</summary>
int WINAPI CATSAPI_Subscribe_Trade(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>提取成交明细请求结果.</summary>
int WINAPI CATSAPI_Fetch_TradeResult(CATSHANDLE hHandle);


/// <summary>准备成交明细请求.</summary>
int WINAPI CATSAPI_PreUnSub_Trade(CATSHANDLE hHandle);

/// <summary>成交明细请求.</summary>
int WINAPI CATSAPI_UnSubscribe_Trade(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取成交明细请求结果.</summary>
int WINAPI CATSAPI_Fetch_UnSubTradeResult(CATSHANDLE hHandle);

# endif

#ifdef   __cplusplus
}
#endif
