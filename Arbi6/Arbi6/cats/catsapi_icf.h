//=============================================================================
/**
 *  @library	catsapi.lib
 *
 *  @file	catsapi_icf.h
 *  
 *  catsapi_icf.h, v1.00 2012/10/20 17:23:56 zhaolei
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

#ifndef _CATSAPI_ICF_H
#define _CATSAPI_ICF_H

	//////////////////////////////////////////////////////////////////////////
	/// �ּ�����
	//////////////////////////////////////////////////////////////////////////

	////////////////////
	// ���ķּ�����������
	////////////////////

	/// <summary>׼���ּ����������ݶ�������.</summary>
	int WINAPI CATSAPI_PreSub_ICFStatisData(CATSHANDLE hHandle);

	/// <summary>���ķּ�����������.</summary>
	int WINAPI CATSAPI_Subscribe_ICFStatisData(CATSHANDLE hHandle, TS_Notify_t pUserOnPublish, void* pUserOnPublishArg, TS_Notify_t pUserOnAnswer, void* pUserOnAnswerArg);

	/// <summary>׼���ּ������������˶�����.</summary>
	int WINAPI CATSAPI_PreUnSub_ICFStatisData(CATSHANDLE hHandle);

	/// <summary>�˶��ּ�����������.</summary>
	int WINAPI CATSAPI_UnSubscribe_ICFStatisData(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

	/// <summary>��ȡ�ּ����������ݽ��.</summary>
	int WINAPI CATSAPI_Fetch_ICFStatisDataResult(CATSHANDLE hHandle);

# endif

#ifdef   __cplusplus
}
#endif