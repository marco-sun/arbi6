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
// 订阅行情
////////////////////

/// <summary>准备行情订阅请求.</summary>
int WINAPI CATSAPI_PreSub_MarketData(CATSHANDLE hHandle);

/// <summary>订阅行情.</summary>
int WINAPI CATSAPI_Subscribe_MarketData(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>准备行情退订请求.</summary>
int WINAPI CATSAPI_PreUnSub_MarketData(CATSHANDLE hHandle);

/// <summary>退订行情.</summary>
int WINAPI CATSAPI_UnSubscribe_MarketData(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取行情结果.</summary>
int WINAPI CATSAPI_Fetch_MarketDataResult(CATSHANDLE hHandle);


/// <summary>准备行情快照订阅请求.</summary>
int WINAPI CATSAPI_PreSub_MarketDataSnapshot(CATSHANDLE hHandle);

/// <summary>订阅行情快照.</summary>
int WINAPI CATSAPI_Subscribe_MarketDataSnapshot(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>准备行情快照退订请求.</summary>
int WINAPI CATSAPI_PreUnSub_MarketDataSnapshot(CATSHANDLE hHandle);

/// <summary>退订行情快照.</summary>
int WINAPI CATSAPI_UnSubscribe_MarketDataSnapshot(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取行情快照结果.</summary>
int WINAPI CATSAPI_Fetch_MarketDataSnapshotResult(CATSHANDLE hHandle);


/// <summary>准备批量行情订阅请求.</summary>
int WINAPI CATSAPI_PreSub_BatchMarketData(CATSHANDLE hHandle);

/// <summary>批量订阅行情.</summary>
int WINAPI CATSAPI_Subscribe_BatchMarketData(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>准备批量行情退订请求.</summary>
int WINAPI CATSAPI_PreUnSub_BatchMarketData(CATSHANDLE hHandle);

/// <summary>退订批量行情.</summary>
int WINAPI CATSAPI_UnSubscribe_BatchMarketData(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取批量订阅行情推送结果.</summary>
int WINAPI CATSAPI_Fetch_BatchMarketDataResult(CATSHANDLE hHandle);

////////////////////
// 订阅聚集行情
////////////////////

/// <summary>准备聚集行情订阅请求.</summary>
int WINAPI CATSAPI_PreSub_BarMarketData(CATSHANDLE hHandle);

/// <summary>订阅聚集行情.</summary>
int WINAPI CATSAPI_Subscribe_BarMarketData(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>准备聚集行情退订请求.</summary>
int WINAPI CATSAPI_PreUnSub_BarMarketData(CATSHANDLE hHandle);

/// <summary>退订聚集行情.</summary>
int WINAPI CATSAPI_UnSubscribe_BarMarketData(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取聚集行情结果.</summary>
int WINAPI CATSAPI_Fetch_BarMarketDataResult(CATSHANDLE hHandle);



/// <summary>准备分钟行情订阅请求.</summary>
int WINAPI CATSAPI_PreSub_MinuteBar(CATSHANDLE hHandle);

/// <summary>订阅分钟行情.</summary>
int WINAPI CATSAPI_Subscribe_MinuteBar(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>提取分钟行情结果.</summary>
int WINAPI CATSAPI_Fetch_MinuteBarResult(CATSHANDLE hHandle);

/// <summary>准备日线行情订阅请求.</summary>
int WINAPI CATSAPI_PreSub_DayBar(CATSHANDLE hHandle);

/// <summary>订阅日线行情.</summary>
int WINAPI CATSAPI_Subscribe_DayBar(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>提取日线行情结果.</summary>
int WINAPI CATSAPI_Fetch_DayBarResult(CATSHANDLE hHandle);

/// <summary>准备取消分钟线订阅.</summary>
int WINAPI CATSAPI_PreUnSub_MinuteBar(CATSHANDLE hHandle);
/// <summary>取消分钟线订阅.</summary>
int WINAPI CATSAPI_UnSubscribe_MinuteBar(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>准备取消日线订阅.</summary>
int WINAPI CATSAPI_PreUnSub_DayBar(CATSHANDLE hHandle);
/// <summary>取消日线订阅.</summary>
int WINAPI CATSAPI_UnSubscribe_DayBar(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);



/// <summary>准备分价数据订阅请求.</summary>
int WINAPI CATSAPI_PreSub_PriceList(CATSHANDLE hHandle);

/// <summary>订阅分价数据.</summary>
int WINAPI CATSAPI_Subscribe_PriceList(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>准备取消分价数据订阅请求.</summary>
int WINAPI CATSAPI_PreUnSub_PriceList(CATSHANDLE hHandle);

/// <summary>取消订阅分价数据.</summary>
int WINAPI CATSAPI_UnSubscribe_PriceList(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取分价数据订阅结果.</summary>
int WINAPI CATSAPI_Fetch_PriceListResult(CATSHANDLE hHandle);

/// <summary>提取分价数据推送数据.</summary>
int WINAPI CATSAPI_Fetch_PubPriceListResult(CATSHANDLE hHandle);


/// <summary>准备逐笔成交订阅请求.</summary>
int WINAPI CATSAPI_PreSub_StockMatchDetail(CATSHANDLE hHandle);

/// <summary>订阅逐笔成交数据.</summary>
int WINAPI CATSAPI_Subscribe_StockMatchDetail(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>准备取消逐笔成交数据订阅请求.</summary>
int WINAPI CATSAPI_PreUnSub_StockMatchDetail(CATSHANDLE hHandle);

/// <summary>取消订阅逐笔成交数据.</summary>
int WINAPI CATSAPI_UnSubscribe_StockMatchDetail(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取逐笔成交结果.</summary>
int WINAPI CATSAPI_Fetch_StockMatchDetailResult(CATSHANDLE hHandle);

/// <summary>提取逐笔成交推送数据.</summary>
int WINAPI CATSAPI_Fetch_PubStockMatchDetailResult(CATSHANDLE hHandle);

/// <summary>准备Level2逐笔成交订阅请求.</summary>
int WINAPI CATSAPI_PreSub_StockMatchDetailL2(CATSHANDLE hHandle);

/// <summary>订阅L2逐笔成交数据.</summary>
int WINAPI CATSAPI_Subscribe_StockMatchDetailL2(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>准备取消L2逐笔成交数据订阅请求.</summary>
int WINAPI CATSAPI_PreUnSub_StockMatchDetailL2(CATSHANDLE hHandle);

/// <summary>取消订阅L2逐笔成交数据.</summary>
int WINAPI CATSAPI_UnSubscribe_StockMatchDetailL2(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取L2逐笔成交数据.</summary>
int WINAPI CATSAPI_Fetch_StockMatchDetailResultL2(CATSHANDLE hHandle);

/// <summary>提取L2逐笔成交推送数据.</summary>
int WINAPI CATSAPI_Fetch_PubStockMatchDetailResultL2(CATSHANDLE hHandle);



/// <summary>准备委托队列数据订阅请求.</summary>
int WINAPI CATSAPI_PreSub_OrderQueue(CATSHANDLE hHandle);

/// <summary>订阅委托队列数据.</summary>
int WINAPI CATSAPI_Subscribe_OrderQueue(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>准备取消委托队列数据订阅请求.</summary>
int WINAPI CATSAPI_PreUnSub_OrderQueue(CATSHANDLE hHandle);

/// <summary>取消订阅委托队列数据.</summary>
int WINAPI CATSAPI_UnSubscribe_OrderQueue(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取委托队列数据推送数据.</summary>
int WINAPI CATSAPI_Fetch_PubOrderQueueResult(CATSHANDLE hHandle);

/// <summary>提取委托队列数据推送订单列表数据.</summary>
int WINAPI CATSAPI_Fetch_PubOrderQueueResult_Orders(CATSHANDLE hHandle);


////////////////////
// 查询当日分钟线
////////////////////

/// <summary>准备查询当日分钟线请求.</summary>
int WINAPI CATSAPI_Prepare_QueryTodayMinBar(CATSHANDLE hHandle);

/// <summary>查询当日分钟线.</summary>
int WINAPI CATSAPI_Execute_QueryTodayMinBar(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询当日分钟线结果.</summary>
int WINAPI CATSAPI_Fetch_QueryTodayMinBarResult(CATSHANDLE hHandle);

/// <summary>提取当日分钟线结果.</summary>
int WINAPI CATSAPI_Fetch_QueryTodayMinBarResult_MinBars(CATSHANDLE hHandle);

////////////////////
// 查询历史分钟线
////////////////////

/// <summary>准备查询历史分钟线请求.</summary>
int WINAPI CATSAPI_Prepare_QueryMinBar(CATSHANDLE hHandle);

/// <summary>查询历史分钟线.</summary>
int WINAPI CATSAPI_Execute_QueryMinBar(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询历史分钟线结果.</summary>
int WINAPI CATSAPI_Fetch_QueryMinBarResult(CATSHANDLE hHandle);

/// <summary>提取历史分钟线结果.</summary>
int WINAPI CATSAPI_Fetch_QueryMinBarResult_MinBars(CATSHANDLE hHandle);

////////////////////
// 查询历史分钟线文件路径以便通过ftp下载
////////////////////

/// <summary>准备查询历史分钟线文件路径信息请求.</summary>
int WINAPI CATSAPI_Prepare_QueryHisMinFilePath(CATSHANDLE hHandle);

/// <summary>查询历史分钟线文件路径信息.</summary>
int WINAPI CATSAPI_Execute_QueryHisMinFilePath(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询历史分钟线文件路径结果.</summary>
int WINAPI CATSAPI_Fetch_QueryHisMinFilePathResult(CATSHANDLE hHandle);

/// <summary>提取历史分钟线文件路径结果.</summary>
int WINAPI CATSAPI_Fetch_QueryHisMinFilePathResult_FilePaths(CATSHANDLE hHandle);


////////////////////
// 下载历史行情文件
////////////////////

/// <summary>准备下载请求.</summary>
int WINAPI CATSAPI_Prepare_DownloadFile(CATSHANDLE hHandle);

/// <summary>下载历史行情文件.</summary>
int WINAPI CATSAPI_Execute_DownloadFile(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取下载行情文件结果.</summary>
int WINAPI CATSAPI_Fetch_DownloadFileResult(CATSHANDLE hHandle);

////////////////////
// 查询历史日线
////////////////////

/// <summary>准备查询历史日线请求.</summary>
int WINAPI CATSAPI_Prepare_QueryDayBar(CATSHANDLE hHandle);

/// <summary>查询历史日线.</summary>
int WINAPI CATSAPI_Execute_QueryDayBar(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取历史日线查询请求结果.</summary>
int WINAPI CATSAPI_Fetch_QueryDayBarResult(CATSHANDLE hHandle);

/// <summary>提取历史日线结果.</summary>
int WINAPI CATSAPI_Fetch_QueryDayBarResult_DayBars(CATSHANDLE hHandle);



///NOTE:不推荐使用此接口，请使用下面的查询商品信息接口查询全部商品信息
/// <summary>准备查询证券信息请求.</summary>
int WINAPI CATSAPI_Prepare_QueryStockInfo(CATSHANDLE hHandle);

/// <summary>查询证券信息.</summary>
int WINAPI CATSAPI_Execute_QueryStockInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询证券信息请求结果.</summary>
int WINAPI CATSAPI_Fetch_QueryStockInfoResult(CATSHANDLE hHandle);

/// <summary>提取证券信息结果.</summary>
int WINAPI CATSAPI_Fetch_QueryStockInfoResult_StockInfo(CATSHANDLE hHandle);


/// <summary>准备查询商品信息请求.</summary>
int WINAPI CATSAPI_Prepare_QueryCommodityInfo(CATSHANDLE hHandle);

/// <summary>查询商品信息.</summary>
int WINAPI CATSAPI_Execute_QueryCommodityInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询商品信息请求结果.</summary>
int WINAPI CATSAPI_Fetch_QueryCommodityInfoResult(CATSHANDLE hHandle);

/// <summary>提取商品信息结果.</summary>
int WINAPI CATSAPI_Fetch_QueryCommodityInfoResult_CommodityInfo(CATSHANDLE hHandle);



/// <summary>准备查询商品类型信息.</summary>
int WINAPI CATSAPI_Prepare_QueryCommodityTypeInfo(CATSHANDLE hHandle);

/// <summary>查询商品类型信息.</summary>
int WINAPI CATSAPI_Execute_QueryCommodityTypeInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取查询商品类型信息请求结果.</summary>
int WINAPI CATSAPI_Fetch_QueryCommodityTypeInfoResult(CATSHANDLE hHandle);



/// <summary>准备订阅组合价格请求.</summary>
int WINAPI CATSAPI_PreSub_PortfolioPrice(CATSHANDLE hHandle);

/// <summary>订阅组合价格请求.</summary>
int WINAPI CATSAPI_Subscribe_PortfolioPrice(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>提取订阅组合价格请求结果.</summary>
int WINAPI CATSAPI_Fetch_PortfolioPriceResult(CATSHANDLE hHandle);


/// <summary>准备取消订阅组合价格请求.</summary>
int WINAPI CATSAPI_PreUnSub_PortfolioPrice(CATSHANDLE hHandle);

/// <summary>取消订阅组合价格请求.</summary>
int WINAPI CATSAPI_UnSubscribe_PortfolioPrice(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>提取取消订阅组合价格请求结果.</summary>
int WINAPI CATSAPI_Fetch_UnSubPortfolioPriceResult(CATSHANDLE hHandle);

# endif

#ifdef   __cplusplus
}
#endif
