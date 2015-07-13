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
/// Cats�˻�������
//////////////////////////////////////////////////////////////////////////


////////////////////
//Cats�˻���¼ 
////////////////////

/// <summary>׼��CATS��������.</summary>
int WINAPI CATSAPI_Prepare_CatsConnect(CATSHANDLE hHandle);

/// <summary>CATS����.</summary>
int WINAPI CATSAPI_Execute_CatsConnect(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡCATS������Ϣ���.</summary>
int WINAPI CATSAPI_Fetch_CatsConnectResult(CATSHANDLE hHandle);

/// <summary>׼��CATS�˻���¼����.</summary>
int WINAPI CATSAPI_Prepare_CatsLogin(CATSHANDLE hHandle);

/// <summary>CATS�˻���¼.</summary>
int WINAPI CATSAPI_Execute_CatsLogin(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡCATS�˻���Ϣ���.</summary>
int WINAPI CATSAPI_Fetch_CatsLoginResult(CATSHANDLE hHandle);

/// <summary>��ȡCATS���˻���Ϣ���.</summary>
int WINAPI CATSAPI_Fetch_CatsSubAccResult(CATSHANDLE hHandle);


/// <summary>׼���������˻�����.</summary>
int WINAPI CATSAPI_Prepare_AddSubAcc(CATSHANDLE hHandle);

/// <summary>�������˻�����.</summary>
int WINAPI CATSAPI_Execute_AddSubAcc(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ�������˻�������.</summary>
int WINAPI CATSAPI_Fetch_AddSubAccResult(CATSHANDLE hHandle);


/// <summary>׼��ɾ�����˻�����.</summary>
int WINAPI CATSAPI_Prepare_DelSubAcc(CATSHANDLE hHandle);

/// <summary>ɾ�����˻�����.</summary>
int WINAPI CATSAPI_Execute_DelSubAcc(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡɾ�����˻�������.</summary>
int WINAPI CATSAPI_Fetch_DelSubAccResult(CATSHANDLE hHandle);


/// <summary>׼����ѯ���˻���Ϣ����.</summary>
int WINAPI CATSAPI_Prepare_QuerySubAcc(CATSHANDLE hHandle);

/// <summary>��ѯ���˻���Ϣ����.</summary>
int WINAPI CATSAPI_Execute_QuerySubAcc(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯ���˻�������.</summary>
int WINAPI CATSAPI_Fetch_QuerySubAccResult(CATSHANDLE hHandle);

/// <summary>��ȡ���˻����.</summary>
int WINAPI CATSAPI_Fetch_QuerySubAccResult_SubAcc(CATSHANDLE hHandle);

//��ȡ���˻�����
int WINAPI CATSAPI_GetSubAccPwd(CATSHANDLE hHandle,char *szCatsId,char *szSubAccType,char *szSubAccId,char *szValue,size_t valueLen);



/// <summary>׼���޸�CATS �˻�����.</summary>
int WINAPI CATSAPI_Prepare_UpdateCatsPwd(CATSHANDLE hHandle);

/// <summary>�޸�CATS�˻�����.</summary>
int WINAPI CATSAPI_Execute_UpdateCatsPwd(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ�޸�CATS�˻�������.</summary>
int WINAPI CATSAPI_Fetch_UpdateCatsPwdResult(CATSHANDLE hHandle);


////////////////////
// �����˻�(���˻�)��¼
////////////////////

/// <summary>׼�������˻���¼����.</summary>
int WINAPI CATSAPI_Prepare_TradeAccLogin(CATSHANDLE hHandle);

/// <summary>�����˻���¼.</summary>
int WINAPI CATSAPI_Execute_TradeAccLogin(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ�����˻���¼���.</summary>
int WINAPI CATSAPI_Fetch_TradeAccLoginResult(CATSHANDLE hHandle);



/// <summary>׼����ѯϵͳ��������.</summary>
int WINAPI CATSAPI_Prepare_QuerySystemParam(CATSHANDLE hHandle);

/// <summary>��ѯϵͳ��������.</summary>
int WINAPI CATSAPI_Execute_QuerySystemParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡϵͳ�������.</summary>
int WINAPI CATSAPI_Fetch_QuerySystemParamResult(CATSHANDLE hHandle);


/// <summary>׼�����ϵͳ��������.</summary>
int WINAPI CATSAPI_Prepare_AddSystemParam(CATSHANDLE hHandle);

/// <summary>���ϵͳ��������.</summary>
int WINAPI CATSAPI_Execute_AddSystemParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ���ϵͳ�������.</summary>
int WINAPI CATSAPI_Fetch_AddSystemParamResult(CATSHANDLE hHandle);


/// <summary>׼��ɾ��ϵͳ��������.</summary>
int WINAPI CATSAPI_Prepare_DelSystemParam(CATSHANDLE hHandle);

/// <summary>ɾ��ϵͳ��������.</summary>
int WINAPI CATSAPI_Execute_DelSystemParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡɾ��ϵͳ�������.</summary>
int WINAPI CATSAPI_Fetch_DelSystemParamResult(CATSHANDLE hHandle);


/// <summary>׼���޸Ļ�����ϵͳ��������.</summary>
int WINAPI CATSAPI_Prepare_ModifySystemParam(CATSHANDLE hHandle);

/// <summary>�޸Ļ�����ϵͳ��������.</summary>
int WINAPI CATSAPI_Execute_ModifySystemParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ�޸Ļ�����ϵͳ�������.</summary>
int WINAPI CATSAPI_Fetch_ModifySystemParamResult(CATSHANDLE hHandle);


/// <summary>׼���޸Ļ�����ϵͳ��������.</summary>
int WINAPI CATSAPI_Prepare_SaveSystemParam(CATSHANDLE hHandle);

/// <summary>�޸Ļ�����ϵͳ��������.</summary>
int WINAPI CATSAPI_Execute_SaveSystemParam(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ�޸Ļ�����ϵͳ�������.</summary>
int WINAPI CATSAPI_Fetch_SaveSystemParamResult(CATSHANDLE hHandle);


/// <summary>׼���޸�CATS�˻���ȫ��¼��ʽ����.</summary>
int WINAPI CATSAPI_Prepare_UpdateCatsSecLoginType(CATSHANDLE hHandle);

/// <summary>�޸�CATS�˻���ȫ��¼��ʽ����.</summary>
int WINAPI CATSAPI_Execute_UpdateCatsSecLoginType(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ�޸�CATS�˻���ȫ��¼��ʽ���.</summary>
int WINAPI CATSAPI_Fetch_UpdateCatsSecLoginTypeResult(CATSHANDLE hHandle);


/// <summary>׼��У��CATS�˻���ȫ�ʴ�.</summary>
int WINAPI CATSAPI_Prepare_CheckSecurityQuestion(CATSHANDLE hHandle);

/// <summary>У��CATS�˻���ȫ�ʴ�.</summary>
int WINAPI CATSAPI_Execute_CheckSecurityQuestion(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>У��CATS�˻���ȫ�ʴ���.</summary>
int WINAPI CATSAPI_Fetch_CheckSecurityQuestionResult(CATSHANDLE hHandle);


/// <summary>׼������CATS�˻����������֤��.</summary>
int WINAPI CATSAPI_Prepare_GetCatsRandomCode(CATSHANDLE hHandle);

/// <summary>����CATS�˻����������֤��.</summary>
int WINAPI CATSAPI_Execute_GetCatsRandomCode(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>����CATS�˻����������֤����.</summary>
int WINAPI CATSAPI_Fetch_GetCatsRandomCodeResult(CATSHANDLE hHandle);


/// <summary>׼��У��CATS�˻����������֤��.</summary>
int WINAPI CATSAPI_Prepare_CheckCatsRandomCode(CATSHANDLE hHandle);

/// <summary>У��CATS�˻����������֤��.</summary>
int WINAPI CATSAPI_Execute_CheckCatsRandomCode(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>У��CATS�˻����������֤����.</summary>
int WINAPI CATSAPI_Fetch_CheckCatsRandomCodeResult(CATSHANDLE hHandle);


/// <summary>׼����ȡϵͳĬ�ϰ�ȫ����.</summary>
int WINAPI CATSAPI_Prepare_QuerySysDefSecQuestion(CATSHANDLE hHandle);

/// <summary>��ȡϵͳĬ�ϰ�ȫ����.</summary>
int WINAPI CATSAPI_Execute_QuerySysDefSecQuestion(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡϵͳĬ�ϰ�ȫ������.</summary>
int WINAPI CATSAPI_Fetch_QuerySysDefSecQuestionResult(CATSHANDLE hHandle);


/// <summary>׼����ѯCATS�˻���ȫ�ʴ�.</summary>
int WINAPI CATSAPI_Prepare_QuerySecurityQuestion(CATSHANDLE hHandle);

/// <summary>��ѯCATS�˻���ȫ�ʴ�.</summary>
int WINAPI CATSAPI_Execute_QuerySecurityQuestion(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ѯCATS�˻���ȫ�ʴ���.</summary>
int WINAPI CATSAPI_Fetch_QuerySecurityQuestionResult(CATSHANDLE hHandle);


/// <summary>׼�����CATS�˻���ȫ�ʴ�.</summary>
int WINAPI CATSAPI_Prepare_AddSecurityQuestion(CATSHANDLE hHandle);

/// <summary>���CATS�˻���ȫ�ʴ�.</summary>
int WINAPI CATSAPI_Execute_AddSecurityQuestion(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>���CATS�˻���ȫ�ʴ���.</summary>
int WINAPI CATSAPI_Fetch_AddSecurityQuestionResult(CATSHANDLE hHandle);


/// <summary>׼��ɾ��CATS�˻���ȫ�ʴ�.</summary>
int WINAPI CATSAPI_Prepare_DeleteSecurityQuestion(CATSHANDLE hHandle);

/// <summary>ɾ��CATS�˻���ȫ�ʴ�.</summary>
int WINAPI CATSAPI_Execute_DeleteSecurityQuestion(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>ɾ��CATS�˻���ȫ�ʴ���.</summary>
int WINAPI CATSAPI_Fetch_DeleteSecurityQuestionResult(CATSHANDLE hHandle);


/// <summary>׼������CATS�˻���ȫ�ʴ�.</summary>
int WINAPI CATSAPI_Prepare_UpdateSecurityQuestion(CATSHANDLE hHandle);

/// <summary>����CATS�˻���ȫ�ʴ�.</summary>
int WINAPI CATSAPI_Execute_UpdateSecurityQuestion(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>����CATS�˻���ȫ�ʴ���.</summary>
int WINAPI CATSAPI_Fetch_UpdateSecurityQuestionResult(CATSHANDLE hHandle);


/// <summary>׼������CATS�˻�������֤.</summary>
int WINAPI CATSAPI_Prepare_SetSecondLoginCheck(CATSHANDLE hHandle);

/// <summary>����CATS�˻�������֤.</summary>
int WINAPI CATSAPI_Execute_SetSecondLoginCheck(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>����CATS�˻�������֤���.</summary>
int WINAPI CATSAPI_Fetch_SetSecondLoginCheckResult(CATSHANDLE hHandle);



/// <summary>׼����ѯCATS�˻�������֤.</summary>
int WINAPI CATSAPI_Prepare_QuerySecondLoginCheck(CATSHANDLE hHandle);

/// <summary>��ѯCATS�˻�������֤.</summary>
int WINAPI CATSAPI_Execute_QuerySecondLoginCheck(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ѯCATS�˻�������֤���.</summary>
int WINAPI CATSAPI_Fetch_QuerySecondLoginCheckResult(CATSHANDLE hHandle);

/// <summary>��ѯCATS�˻�������֤�ʴ���.</summary>
int WINAPI CATSAPI_Fetch_QuerySecondLoginCheckResult_Question(CATSHANDLE hHandle);



/// <summary>׼����ѯ�����ֵ�.</summary>
int WINAPI CATSAPI_Prepare_QueryDictionary(CATSHANDLE hHandle);

/// <summary>��ѯ�����ֵ�����.</summary>
int WINAPI CATSAPI_Execute_QueryDictionary(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ�����ֵ���.</summary>
int WINAPI CATSAPI_Fetch_QueryDictionaryResult(CATSHANDLE hHandle);


/// <summary>׼��ˢ�����˻���Ϣ.</summary>
int WINAPI CATSAPI_Prepare_RefreshAccountInfo(CATSHANDLE hHandle);

/// <summary>ˢ�����˻���Ϣ����.</summary>
int WINAPI CATSAPI_Execute_RefreshAccountInfo(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡˢ�����˻���Ϣ������.</summary>
int WINAPI CATSAPI_Fetch_RefreshAccountInfoResult(CATSHANDLE hHandle);


/// <summary>׼����ѯCATS�˻�����Ȩ��.</summary>
int WINAPI CATSAPI_Prepare_QueryCatsFunction(CATSHANDLE hHandle);

/// <summary>��ѯCATS�˻�����Ȩ��.</summary>
int WINAPI CATSAPI_Execute_QueryCatsFunction(CATSHANDLE hHandle, TS_Notify_t pUserOnAnswer, void* arg);

/// <summary>��ȡ��ѯCATS�˻�����Ȩ�޽��.</summary>
int WINAPI CATSAPI_Fetch_QueryCatsFunctionResult(CATSHANDLE hHandle);

# endif
#ifdef   __cplusplus
}
#endif