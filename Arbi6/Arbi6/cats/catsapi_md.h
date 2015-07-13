//=============================================================================
/**
 *  @library	catsapi.lib
 *
 *  @file	catsapi_md.h
 *  
 *  catsapi_md.h, v1.00 2012/10/20 17:23:56 zhaolei
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

#ifndef _CATSAPI_MD_H
#define _CATSAPI_MD_H

////////////////////
// ��������
////////////////////

/// <summary>׼�����鶩������.</summary>
int WINAPI CATSAPI_PreSub_MarketData(CATSHANDLE hHandle);

/// <summary>��������.</summary>
int WINAPI CATSAPI_Subscribe_MarketData(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>׼�������˶�����.</summary>
int WINAPI CATSAPI_PreUnSub_MarketData(CATSHANDLE hHandle);

/// <summary>�˶�����.</summary>
int WINAPI CATSAPI_UnSubscribe_MarketData(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ������.</summary>
int WINAPI CATSAPI_Fetch_MarketDataResult(CATSHANDLE hHandle);


/// <summary>׼��������ն�������.</summary>
int WINAPI CATSAPI_PreSub_MarketDataSnapshot(CATSHANDLE hHandle);

/// <summary>�����������.</summary>
int WINAPI CATSAPI_Subscribe_MarketDataSnapshot(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>׼����������˶�����.</summary>
int WINAPI CATSAPI_PreUnSub_MarketDataSnapshot(CATSHANDLE hHandle);

/// <summary>�˶��������.</summary>
int WINAPI CATSAPI_UnSubscribe_MarketDataSnapshot(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ������ս��.</summary>
int WINAPI CATSAPI_Fetch_MarketDataSnapshotResult(CATSHANDLE hHandle);


/// <summary>׼���������鶩������.</summary>
int WINAPI CATSAPI_PreSub_BatchMarketData(CATSHANDLE hHandle);

/// <summary>������������.</summary>
int WINAPI CATSAPI_Subscribe_BatchMarketData(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>׼�����������˶�����.</summary>
int WINAPI CATSAPI_PreUnSub_BatchMarketData(CATSHANDLE hHandle);

/// <summary>�˶���������.</summary>
int WINAPI CATSAPI_UnSubscribe_BatchMarketData(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ���������������ͽ��.</summary>
int WINAPI CATSAPI_Fetch_BatchMarketDataResult(CATSHANDLE hHandle);

////////////////////
// ���ľۼ�����
////////////////////

/// <summary>׼���ۼ����鶩������.</summary>
int WINAPI CATSAPI_PreSub_BarMarketData(CATSHANDLE hHandle);

/// <summary>���ľۼ�����.</summary>
int WINAPI CATSAPI_Subscribe_BarMarketData(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>׼���ۼ������˶�����.</summary>
int WINAPI CATSAPI_PreUnSub_BarMarketData(CATSHANDLE hHandle);

/// <summary>�˶��ۼ�����.</summary>
int WINAPI CATSAPI_UnSubscribe_BarMarketData(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ�ۼ�������.</summary>
int WINAPI CATSAPI_Fetch_BarMarketDataResult(CATSHANDLE hHandle);



/// <summary>׼���������鶩������.</summary>
int WINAPI CATSAPI_PreSub_MinuteBar(CATSHANDLE hHandle);

/// <summary>���ķ�������.</summary>
int WINAPI CATSAPI_Subscribe_MinuteBar(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>��ȡ����������.</summary>
int WINAPI CATSAPI_Fetch_MinuteBarResult(CATSHANDLE hHandle);

/// <summary>׼���������鶩������.</summary>
int WINAPI CATSAPI_PreSub_DayBar(CATSHANDLE hHandle);

/// <summary>������������.</summary>
int WINAPI CATSAPI_Subscribe_DayBar(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>��ȡ����������.</summary>
int WINAPI CATSAPI_Fetch_DayBarResult(CATSHANDLE hHandle);

/// <summary>׼��ȡ�������߶���.</summary>
int WINAPI CATSAPI_PreUnSub_MinuteBar(CATSHANDLE hHandle);
/// <summary>ȡ�������߶���.</summary>
int WINAPI CATSAPI_UnSubscribe_MinuteBar(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>׼��ȡ�����߶���.</summary>
int WINAPI CATSAPI_PreUnSub_DayBar(CATSHANDLE hHandle);
/// <summary>ȡ�����߶���.</summary>
int WINAPI CATSAPI_UnSubscribe_DayBar(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);



/// <summary>׼���ּ����ݶ�������.</summary>
int WINAPI CATSAPI_PreSub_PriceList(CATSHANDLE hHandle);

/// <summary>���ķּ�����.</summary>
int WINAPI CATSAPI_Subscribe_PriceList(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>׼��ȡ���ּ����ݶ�������.</summary>
int WINAPI CATSAPI_PreUnSub_PriceList(CATSHANDLE hHandle);

/// <summary>ȡ�����ķּ�����.</summary>
int WINAPI CATSAPI_UnSubscribe_PriceList(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ�ּ����ݶ��Ľ��.</summary>
int WINAPI CATSAPI_Fetch_PriceListResult(CATSHANDLE hHandle);

/// <summary>��ȡ�ּ�������������.</summary>
int WINAPI CATSAPI_Fetch_PubPriceListResult(CATSHANDLE hHandle);


/// <summary>׼����ʳɽ���������.</summary>
int WINAPI CATSAPI_PreSub_StockMatchDetail(CATSHANDLE hHandle);

/// <summary>������ʳɽ�����.</summary>
int WINAPI CATSAPI_Subscribe_StockMatchDetail(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>׼��ȡ����ʳɽ����ݶ�������.</summary>
int WINAPI CATSAPI_PreUnSub_StockMatchDetail(CATSHANDLE hHandle);

/// <summary>ȡ��������ʳɽ�����.</summary>
int WINAPI CATSAPI_UnSubscribe_StockMatchDetail(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ʳɽ����.</summary>
int WINAPI CATSAPI_Fetch_StockMatchDetailResult(CATSHANDLE hHandle);

/// <summary>��ȡ��ʳɽ���������.</summary>
int WINAPI CATSAPI_Fetch_PubStockMatchDetailResult(CATSHANDLE hHandle);

/// <summary>׼��Level2��ʳɽ���������.</summary>
int WINAPI CATSAPI_PreSub_StockMatchDetailL2(CATSHANDLE hHandle);

/// <summary>����L2��ʳɽ�����.</summary>
int WINAPI CATSAPI_Subscribe_StockMatchDetailL2(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>׼��ȡ��L2��ʳɽ����ݶ�������.</summary>
int WINAPI CATSAPI_PreUnSub_StockMatchDetailL2(CATSHANDLE hHandle);

/// <summary>ȡ������L2��ʳɽ�����.</summary>
int WINAPI CATSAPI_UnSubscribe_StockMatchDetailL2(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡL2��ʳɽ�����.</summary>
int WINAPI CATSAPI_Fetch_StockMatchDetailResultL2(CATSHANDLE hHandle);

/// <summary>��ȡL2��ʳɽ���������.</summary>
int WINAPI CATSAPI_Fetch_PubStockMatchDetailResultL2(CATSHANDLE hHandle);



/// <summary>׼��ί�ж������ݶ�������.</summary>
int WINAPI CATSAPI_PreSub_OrderQueue(CATSHANDLE hHandle);

/// <summary>����ί�ж�������.</summary>
int WINAPI CATSAPI_Subscribe_OrderQueue(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>׼��ȡ��ί�ж������ݶ�������.</summary>
int WINAPI CATSAPI_PreUnSub_OrderQueue(CATSHANDLE hHandle);

/// <summary>ȡ������ί�ж�������.</summary>
int WINAPI CATSAPI_UnSubscribe_OrderQueue(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡί�ж���������������.</summary>
int WINAPI CATSAPI_Fetch_PubOrderQueueResult(CATSHANDLE hHandle);

/// <summary>��ȡί�ж����������Ͷ����б�����.</summary>
int WINAPI CATSAPI_Fetch_PubOrderQueueResult_Orders(CATSHANDLE hHandle);


////////////////////
// ��ѯ���շ�����
////////////////////

/// <summary>׼����ѯ���շ���������.</summary>
int WINAPI CATSAPI_Prepare_QueryTodayMinBar(CATSHANDLE hHandle);

/// <summary>��ѯ���շ�����.</summary>
int WINAPI CATSAPI_Execute_QueryTodayMinBar(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ���շ����߽��.</summary>
int WINAPI CATSAPI_Fetch_QueryTodayMinBarResult(CATSHANDLE hHandle);

/// <summary>��ȡ���շ����߽��.</summary>
int WINAPI CATSAPI_Fetch_QueryTodayMinBarResult_MinBars(CATSHANDLE hHandle);

////////////////////
// ��ѯ��ʷ������
////////////////////

/// <summary>׼����ѯ��ʷ����������.</summary>
int WINAPI CATSAPI_Prepare_QueryMinBar(CATSHANDLE hHandle);

/// <summary>��ѯ��ʷ������.</summary>
int WINAPI CATSAPI_Execute_QueryMinBar(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ��ʷ�����߽��.</summary>
int WINAPI CATSAPI_Fetch_QueryMinBarResult(CATSHANDLE hHandle);

/// <summary>��ȡ��ʷ�����߽��.</summary>
int WINAPI CATSAPI_Fetch_QueryMinBarResult_MinBars(CATSHANDLE hHandle);

////////////////////
// ��ѯ��ʷ�������ļ�·���Ա�ͨ��ftp����
////////////////////

/// <summary>׼����ѯ��ʷ�������ļ�·����Ϣ����.</summary>
int WINAPI CATSAPI_Prepare_QueryHisMinFilePath(CATSHANDLE hHandle);

/// <summary>��ѯ��ʷ�������ļ�·����Ϣ.</summary>
int WINAPI CATSAPI_Execute_QueryHisMinFilePath(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ��ʷ�������ļ�·�����.</summary>
int WINAPI CATSAPI_Fetch_QueryHisMinFilePathResult(CATSHANDLE hHandle);

/// <summary>��ȡ��ʷ�������ļ�·�����.</summary>
int WINAPI CATSAPI_Fetch_QueryHisMinFilePathResult_FilePaths(CATSHANDLE hHandle);


////////////////////
// ������ʷ�����ļ�
////////////////////

/// <summary>׼����������.</summary>
int WINAPI CATSAPI_Prepare_DownloadFile(CATSHANDLE hHandle);

/// <summary>������ʷ�����ļ�.</summary>
int WINAPI CATSAPI_Execute_DownloadFile(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ���������ļ����.</summary>
int WINAPI CATSAPI_Fetch_DownloadFileResult(CATSHANDLE hHandle);

////////////////////
// ��ѯ��ʷ����
////////////////////

/// <summary>׼����ѯ��ʷ��������.</summary>
int WINAPI CATSAPI_Prepare_QueryDayBar(CATSHANDLE hHandle);

/// <summary>��ѯ��ʷ����.</summary>
int WINAPI CATSAPI_Execute_QueryDayBar(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ʷ���߲�ѯ������.</summary>
int WINAPI CATSAPI_Fetch_QueryDayBarResult(CATSHANDLE hHandle);

/// <summary>��ȡ��ʷ���߽��.</summary>
int WINAPI CATSAPI_Fetch_QueryDayBarResult_DayBars(CATSHANDLE hHandle);



///NOTE:���Ƽ�ʹ�ô˽ӿڣ���ʹ������Ĳ�ѯ��Ʒ��Ϣ�ӿڲ�ѯȫ����Ʒ��Ϣ
/// <summary>׼����ѯ֤ȯ��Ϣ����.</summary>
int WINAPI CATSAPI_Prepare_QueryStockInfo(CATSHANDLE hHandle);

/// <summary>��ѯ֤ȯ��Ϣ.</summary>
int WINAPI CATSAPI_Execute_QueryStockInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ֤ȯ��Ϣ������.</summary>
int WINAPI CATSAPI_Fetch_QueryStockInfoResult(CATSHANDLE hHandle);

/// <summary>��ȡ֤ȯ��Ϣ���.</summary>
int WINAPI CATSAPI_Fetch_QueryStockInfoResult_StockInfo(CATSHANDLE hHandle);


/// <summary>׼����ѯ��Ʒ��Ϣ����.</summary>
int WINAPI CATSAPI_Prepare_QueryCommodityInfo(CATSHANDLE hHandle);

/// <summary>��ѯ��Ʒ��Ϣ.</summary>
int WINAPI CATSAPI_Execute_QueryCommodityInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ��Ʒ��Ϣ������.</summary>
int WINAPI CATSAPI_Fetch_QueryCommodityInfoResult(CATSHANDLE hHandle);

/// <summary>��ȡ��Ʒ��Ϣ���.</summary>
int WINAPI CATSAPI_Fetch_QueryCommodityInfoResult_CommodityInfo(CATSHANDLE hHandle);



/// <summary>׼����ѯ��Ʒ������Ϣ.</summary>
int WINAPI CATSAPI_Prepare_QueryCommodityTypeInfo(CATSHANDLE hHandle);

/// <summary>��ѯ��Ʒ������Ϣ.</summary>
int WINAPI CATSAPI_Execute_QueryCommodityTypeInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ��Ʒ������Ϣ������.</summary>
int WINAPI CATSAPI_Fetch_QueryCommodityTypeInfoResult(CATSHANDLE hHandle);



/// <summary>׼��������ϼ۸�����.</summary>
int WINAPI CATSAPI_PreSub_PortfolioPrice(CATSHANDLE hHandle);

/// <summary>������ϼ۸�����.</summary>
int WINAPI CATSAPI_Subscribe_PortfolioPrice(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>��ȡ������ϼ۸�������.</summary>
int WINAPI CATSAPI_Fetch_PortfolioPriceResult(CATSHANDLE hHandle);


/// <summary>׼��ȡ��������ϼ۸�����.</summary>
int WINAPI CATSAPI_PreUnSub_PortfolioPrice(CATSHANDLE hHandle);

/// <summary>ȡ��������ϼ۸�����.</summary>
int WINAPI CATSAPI_UnSubscribe_PortfolioPrice(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡȡ��������ϼ۸�������.</summary>
int WINAPI CATSAPI_Fetch_UnSubPortfolioPriceResult(CATSHANDLE hHandle);

# endif

#ifdef   __cplusplus
}
#endif
