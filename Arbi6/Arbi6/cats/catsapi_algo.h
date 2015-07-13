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
/// �㷨������
//////////////////////////////////////////////////////////////////////////

////////////////////
// �����㷨
////////////////////

/// <summary>׼�������㷨����.</summary>
int WINAPI CATSAPI_Prepare_StartAlgo(CATSHANDLE hHandle);

/// <summary>�����㷨.</summary>
int WINAPI CATSAPI_Execute_StartAlgo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ�����㷨���.</summary>
int WINAPI CATSAPI_Fetch_StartAlgoResult(CATSHANDLE hHandle);

////////////////////
// ֹͣ�㷨
////////////////////

/// <summary>׼��ֹͣ�㷨����.</summary>
int WINAPI CATSAPI_Prepare_StopAlgo(CATSHANDLE hHandle);

/// <summary>ֹͣ�㷨.</summary>
int WINAPI CATSAPI_Execute_StopAlgo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡֹͣ�㷨���.</summary>
int WINAPI CATSAPI_Fetch_StopAlgoResult(CATSHANDLE hHandle);

////////////////////
// �����㷨ʵ��
////////////////////

/// <summary>׼�������㷨ʵ������.</summary>
int WINAPI CATSAPI_Prepare_UpdateAlgoInstance(CATSHANDLE hHandle);

/// <summary>�����㷨ʵ��.</summary>
int WINAPI CATSAPI_Execute_UpdateAlgoInstance(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ�����㷨ʵ�����.</summary>
int WINAPI CATSAPI_Fetch_UpdateAlgoInstanceResult(CATSHANDLE hHandle);


////////////////////
// �����㷨ʵ��״̬
////////////////////
//�ɽӿڣ����Ƽ�ʹ��,ʵ���������ϲ���Subscribe_AlgoInstanceEx,ִ����Ϣ�ϲ���SubscribeExecStatisData
/// <summary>׼���㷨ʵ��״̬��������.</summary>
int WINAPI CATSAPI_PreSub_AlgoStatus(CATSHANDLE hHandle);

/// <summary>�����㷨ʵ��״̬.</summary>
int WINAPI CATSAPI_Subscribe_AlgoStatus(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>׼���㷨ʵ��״̬�˶�����.</summary>
int WINAPI CATSAPI_PreUnSub_AlgoStatus(CATSHANDLE hHandle);

/// <summary>�˶��㷨ʵ��״̬.</summary>
int WINAPI CATSAPI_UnSubscribe_AlgoStatus(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ�㷨ʵ��״̬���.</summary>
int WINAPI CATSAPI_Fetch_AlgoStatusResult(CATSHANDLE hHandle);

/// <summary>��ȡ�㷨ʵ��ִ�н��.</summary>
int WINAPI CATSAPI_Fetch_AlgoStatusResult_ExecDatas(CATSHANDLE hHandle);

/// <summary>��ȡ�㷨ʵ���������.</summary>
int WINAPI CATSAPI_Fetch_AlgoStatusResult_Params(CATSHANDLE hHandle);



//�ɽӿڣ����Ƽ�ʹ��,�ϲ���Subscribe_AlgoInstanceEx
/// <summary>׼���㷨ʵ���б�������.</summary>
int WINAPI CATSAPI_PreSub_AlgoInstance(CATSHANDLE hHandle);

/// <summary>�����㷨ʵ���б�.</summary>
int WINAPI CATSAPI_Subscribe_AlgoInstance(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>׼���㷨ʵ���б��˶�����.</summary>
int WINAPI CATSAPI_PreUnSub_AlgoInstance(CATSHANDLE hHandle);

/// <summary>�˶��㷨ʵ���б�.</summary>
int WINAPI CATSAPI_UnSubscribe_AlgoInstance(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ�㷨ʵ�����.</summary>
int WINAPI CATSAPI_Fetch_AlgoInstanceResult(CATSHANDLE hHandle);

/// <summary>��ȡ�㷨ʵ���б���.</summary>
int WINAPI CATSAPI_Fetch_AlgoInstanceListResult(CATSHANDLE hHandle);

/// <summary>��ȡ�㷨ʵ���������.</summary>
int WINAPI CATSAPI_Fetch_AlgoInstanceParamResult(CATSHANDLE hHandle);



/// <summary>׼���㷨ʵ���б�������.</summary>
int WINAPI CATSAPI_PreSub_AlgoInstanceEx(CATSHANDLE hHandle);

/// <summary>�����㷨ʵ���б�.</summary>
int WINAPI CATSAPI_Subscribe_AlgoInstanceEx(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>׼���㷨ʵ���б��˶�����.</summary>
int WINAPI CATSAPI_PreUnSub_AlgoInstanceEx(CATSHANDLE hHandle);

/// <summary>�˶��㷨ʵ���б�.</summary>
int WINAPI CATSAPI_UnSubscribe_AlgoInstanceEx(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ�㷨ʵ�����.</summary>
int WINAPI CATSAPI_Fetch_AlgoInstanceResultEx(CATSHANDLE hHandle);

/// <summary>��ȡ�㷨ʵ���������.</summary>
int WINAPI CATSAPI_Fetch_AlgoInstanceParamResultEx(CATSHANDLE hHandle);


/// <summary>׼���㷨ִ����Ϣ��������.</summary>
int WINAPI CATSAPI_PreSub_AlgoExecStatisData(CATSHANDLE hHandle);

/// <summary>�����㷨ʵ��ִ����Ϣ.</summary>
int WINAPI CATSAPI_Subscribe_AlgoExecStatisData(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>׼���㷨ʵ��ִ����Ϣ�˶�����.</summary>
int WINAPI CATSAPI_PreUnSub_AlgoExecStatisData(CATSHANDLE hHandle);

/// <summary>�˶��㷨ʵ��ִ����Ϣ.</summary>
int WINAPI CATSAPI_UnSubscribe_AlgoExecStatisData(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ�㷨ʵ�����.</summary>
int WINAPI CATSAPI_Fetch_AlgoExecStatisDataResult(CATSHANDLE hHandle);

/// <summary>��ȡ�㷨ʵ��ִ����Ϣ���.</summary>
int WINAPI CATSAPI_Fetch_AlgoExecStatisDataResult_ExecStatisData(CATSHANDLE hHandle);



////////////////////
// �����㷨ʵ��������־
////////////////////

/// <summary>׼���㷨ʵ��������־��������.</summary>
int WINAPI CATSAPI_PreSub_AlgoRunLog(CATSHANDLE hHandle);

/// <summary>�����㷨ʵ��������־.</summary>
int WINAPI CATSAPI_Subscribe_AlgoRunLog(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>׼���㷨ʵ��������־�˶�����.</summary>
int WINAPI CATSAPI_PreUnSub_AlgoRunLog(CATSHANDLE hHandle);

/// <summary>�˶��㷨ʵ��������־.</summary>
int WINAPI CATSAPI_UnSubscribe_AlgoRunLog(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ�㷨ʵ��������־���ͽ��.</summary>
int WINAPI CATSAPI_Fetch_AlgoRunLogResult(CATSHANDLE hHandle);

/// <summary>��ȡ�㷨ʵ��������־���ͽ������־���.</summary>
int WINAPI CATSAPI_Fetch_AlgoRunLogResult_Logs(CATSHANDLE hHandle);



/// <summary>׼���㷨ʵ��ί��ָ�������.</summary>
int WINAPI CATSAPI_PreSub_AlgoInstOrders(CATSHANDLE hHandle);

/// <summary>�����㷨ʵ��ί��ָ��.</summary>
int WINAPI CATSAPI_Subscribe_AlgoInstOrders(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>��ȡ�㷨ʵ��ί��ָ��.</summary>
int WINAPI CATSAPI_Fetch_AlgoInst_Order(CATSHANDLE hHandle);


/// <summary>׼���㷨ʵ������ָ�������.</summary>
int WINAPI CATSAPI_PreSub_AlgoInstCancelOrders(CATSHANDLE hHandle);

/// <summary>�����㷨ʵ������ָ��.</summary>
int WINAPI CATSAPI_Subscribe_AlgoInstCancelOrders(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>��ȡ�㷨ʵ������ָ��.</summary>
int WINAPI CATSAPI_Fetch_AlgoInst_CancelOrder(CATSHANDLE hHandle);


/// <summary>׼�������㷨ʵ��ί��״̬����.</summary>
int WINAPI CATSAPI_Prepare_UpdateAlgoOrderStatus(CATSHANDLE hHandle);

/// <summary>ִ�и����㷨ʵ��ί��״̬����.</summary>
int WINAPI CATSAPI_Execute_UpdateAlgoOrderStatus(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡִ�и����㷨ʵ��ί��״̬������.</summary>
int WINAPI CATSAPI_Fetch_UpdateAlgoOrderResult(CATSHANDLE hHandle);


//��ѯ�ͻ���Ȩ�㷨
/// <summary>׼����ѯ�ͻ���Ȩ�㷨.</summary>
int WINAPI CATSAPI_Prepare_QryCatsAlgo(CATSHANDLE hHandle);

/// <summary>ִ�в�ѯ�ͻ���Ȩ�㷨.</summary>
int WINAPI CATSAPI_Execute_QryCatsAlgo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡִ�в�ѯ�ͻ���Ȩ�㷨������.</summary>
int WINAPI CATSAPI_Fetch_CatsAlgoResult(CATSHANDLE hHandle);

//��ѯ�㷨����
/// <summary>׼����ѯ�㷨����.</summary>
int WINAPI CATSAPI_Prepare_QryAlgoParams(CATSHANDLE hHandle);

/// <summary>ִ�в�ѯ�㷨����.</summary>
int WINAPI CATSAPI_Execute_QryAlgoParams(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ�㷨������ѯ���.</summary>
int WINAPI CATSAPI_Fetch_AlgoParamsResult(CATSHANDLE hHandle);

//��ѯ�㷨ʵ��
/// <summary>׼����ѯ�㷨ʵ��.</summary>
int WINAPI CATSAPI_Prepare_QryAlgoInstance(CATSHANDLE hHandle);

/// <summary>��ѯ�㷨ʵ��.</summary>
int WINAPI CATSAPI_Execute_QryAlgoInstance(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ�㷨ʵ�����.</summary>
int WINAPI CATSAPI_Fetch_QryAlgoInstResult_AlgoInst(CATSHANDLE hHandle);

/// <summary>��ȡ�㷨ʵ���������.</summary>
int WINAPI CATSAPI_Fetch_QryAlgoInstResult_AlgoInstParams(CATSHANDLE hHandle);


//��ѯ�㷨ʵ������
/// <summary>׼����ѯ�㷨ʵ������.</summary>
int WINAPI CATSAPI_Prepare_QryAlgoInstanceParams(CATSHANDLE hHandle);

/// <summary>��ѯ�㷨ʵ������.</summary>
int WINAPI CATSAPI_Execute_QryAlgoInstanceParams(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ�㷨ʵ��������ѯ���.</summary>
int WINAPI CATSAPI_Fetch_AlgoInstanceParamsResult(CATSHANDLE hHandle);

//����㷨ģ��
/// <summary>׼������㷨ģ��.</summary>
int WINAPI CATSAPI_Prepare_AddAlgoTemplates(CATSHANDLE hHandle);

/// <summary>����㷨ģ��.</summary>
int WINAPI CATSAPI_Execute_AddAlgoTemplates(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ����㷨ģ����.</summary>
int WINAPI CATSAPI_Fetch_AddAlgoTemplatesResult(CATSHANDLE hHandle);



////////////////////
// ���Ŀ�תծ����ʵ��ִ����Ϣ
////////////////////

/// <summary>׼����תծ��������ʵ��ִ����Ϣ��������.</summary>
int WINAPI CATSAPI_PreSub_CAProcess(CATSHANDLE hHandle);

/// <summary>���Ŀ�תծ��������ʵ��ִ����Ϣ.</summary>
int WINAPI CATSAPI_Subscribe_CAProcess(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>׼����תծ��������ʵ��ִ����Ϣ�˶�����.</summary>
int WINAPI CATSAPI_PreUnSub_CAProcess(CATSHANDLE hHandle);

/// <summary>�˶���תծ��������ʵ��ִ����Ϣ.</summary>
int WINAPI CATSAPI_UnSubscribe_CAProcess(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��תծ��������ʵ��ִ����Ϣ���.</summary>
int WINAPI CATSAPI_Fetch_CAProcessResult(CATSHANDLE hHandle);

/// <summary>��ȡ��תծ��������ʵ��ִ����Ϣ���.</summary>
int WINAPI CATSAPI_Fetch_CAProcessResult_Processes(CATSHANDLE hHandle);



////////////////////
// ����ͨ������ʵ��ִ����Ϣ
////////////////////

/// <summary>׼��ͨ����������ʵ��ִ����Ϣ��������.</summary>
int WINAPI CATSAPI_PreSub_CACommonProcess(CATSHANDLE hHandle);

/// <summary>����ͨ����������ʵ��ִ����Ϣ.</summary>
int WINAPI CATSAPI_Subscribe_CACommonProcess(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

/// <summary>׼��ͨ����������ʵ��ִ����Ϣ�˶�����.</summary>
int WINAPI CATSAPI_PreUnSub_CACommonProcess(CATSHANDLE hHandle);

/// <summary>�˶�ͨ����������ʵ��ִ����Ϣ.</summary>
int WINAPI CATSAPI_UnSubscribe_CACommonProcess(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡͨ����������ʵ��ִ����Ϣ���.</summary>
int WINAPI CATSAPI_Fetch_CACommonProcessResult(CATSHANDLE hHandle);

/// <summary>��ȡͨ����������ʵ��ִ����Ϣ���.</summary>
int WINAPI CATSAPI_Fetch_CACommonProcessResult_Processes(CATSHANDLE hHandle);


/// <summary>׼����ѯ�㷨���׼���������.</summary>
int WINAPI CATSAPI_Prepare_QueryATRelevantParam(CATSHANDLE hHandle);

/// <summary>��ѯ�㷨���׼���������.</summary>
int WINAPI CATSAPI_Execute_QueryATRelevantParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ�㷨���׼������������е��㷨���.</summary>
int WINAPI CATSAPI_Fetch_QueryATRelevantParamResult_Algos(CATSHANDLE hHandle);

/// <summary>��ȡ��ѯ�㷨���׼������������е��㷨�������.</summary>
int WINAPI CATSAPI_Fetch_QueryATRelevantParamResult_AlgosParams(CATSHANDLE hHandle);

# endif

#ifdef   __cplusplus
}
#endif
