/////////////////////////////////////////////////////////////////////////
///@system ��һ��������ϵͳ
///@company �Ϻ��ڻ���Ϣ�������޹�˾
///@file ThostFtdcUserApi.h
///@brief �����˿ͻ��˽ӿ�
///@history 
///20060106	�Ժ��		�������ļ�
/////////////////////////////////////////////////////////////////////////

#if !defined(THOST_FTDCUSERAPI_H)
#define THOST_FTDCUSERAPI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThostFtdcUserApiStruct.h"

#if defined(USERAPI_IS_LIB) && defined(WIN32)
#ifdef LIB_USER_API_EXPORT
#define USER_API_EXPORT __declspec(dllexport)
#else
#define USER_API_EXPORT __declspec(dllimport)
#endif
#else
#define USER_API_EXPORT 
#endif


class CThostFtdcUserSpi
{
public:
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected(){};
	
	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason){};
		
	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse){};

  ///�ͻ�����֤��Ӧ
	virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
	

	///�����������
	virtual void OnRspDataDump(CThostFtdcSettlementRefField *pSettlementRef, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///װ�ؽ�����Ϣ������Ӧ
	virtual void OnRspLoadSettlementInfo(CThostFtdcLoadSettlementInfoField *pLoadSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�ǳ�������Ӧ
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ȫ���ǳ�������Ӧ
	virtual void OnRspLogoutAll(CThostFtdcLogoutAllField *pLogoutAll, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ǿ���û��ǳ�������Ӧ
	virtual void OnRspForceUserLogout(CThostFtdcForceUserLogoutField *pForceUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�û��������������Ӧ
	virtual void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ǿ���û��˳�������Ӧ
	virtual void OnRspForceUserExit(CThostFtdcForceUserLogoutField *pForceUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�ʽ��˻��������������Ӧ
	virtual void OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����¼��������Ӧ
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///Ԥ��¼��������Ӧ
	virtual void OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///Ԥ�񳷵�¼��������Ӧ
	virtual void OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��������������Ӧ
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��ѯ��󱨵�������Ӧ
	virtual void OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///Ͷ���߽�����ȷ����Ӧ
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���������ʽ�ת�ڻ���Ӧ
	virtual void OnRspTransferBankToFuture(CThostFtdcTransferBankToFutureRspField *pTransferBankToFutureRsp, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ڻ��ʽ�ת������Ӧ
	virtual void OnRspTransferFutureToBank(CThostFtdcTransferFutureToBankRspField *pTransferFutureToBankRsp, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ�����ʽ���Ӧ
	virtual void OnRspTransferQryBank(CThostFtdcTransferQryBankRspField *pTransferQryBankRsp, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ���н�����ϸ��Ӧ
	virtual void OnRspTransferQryDetail(CThostFtdcTransferQryDetailRspField *pTransferQryDetailRsp, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ɾ��Ԥ����Ӧ
	virtual void OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ɾ��Ԥ�񳷵���Ӧ
	virtual void OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///У��Ͷ����������Ӧ
	virtual void OnRspVerifyInvestorPassword(CThostFtdcVerifyInvestorPasswordField *pVerifyInvestorPassword, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���ݼ۸��ѯ��󱨵�������Ӧ
	virtual void OnRspQueryMaxOrderVolumeWithPrice(CThostFtdcQueryMaxOrderVolumeWithPriceField *pQueryMaxOrderVolumeWithPrice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������Ӳ�Ʒ��Ӧ
	virtual void OnRspInsProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸Ĳ�Ʒ��Ӧ
	virtual void OnRspUpdProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������Ӻ�Լ��Ӧ
	virtual void OnRspInsInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸ĺ�Լ��Ӧ
	virtual void OnRspUpdInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������Ӿ��͹�˾��Ӧ
	virtual void OnRspInsBroker(CThostFtdcBrokerField *pBroker, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸ľ��͹�˾��Ӧ
	virtual void OnRspUpdBroker(CThostFtdcBrokerField *pBroker, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������ӽ���Ա��Ӧ
	virtual void OnRspInsTrader(CThostFtdcTraderField *pTrader, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸Ľ���Ա��Ӧ
	virtual void OnRspUpdTrader(CThostFtdcTraderField *pTrader, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������ӻ�Ա���͹�˾��ϵ��Ӧ
	virtual void OnRspInsPartBroker(CThostFtdcPartBrokerField *pPartBroker, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸Ļ�Ա���͹�˾��ϵ��Ӧ
	virtual void OnRspUpdPartBroker(CThostFtdcPartBrokerField *pPartBroker, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������ӹ����û���Ӧ
	virtual void OnRspInsSuperUser(CThostFtdcSuperUserField *pSuperUser, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸Ĺ����û���Ӧ
	virtual void OnRspUpdSuperUser(CThostFtdcSuperUserField *pSuperUser, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������ӹ����û�Ȩ����Ӧ
	virtual void OnRspInsSuperUserFunction(CThostFtdcSuperUserFunctionField *pSuperUserFunction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ�������û�Ȩ����Ӧ
	virtual void OnRspDelSuperUserFunction(CThostFtdcSuperUserFunctionField *pSuperUserFunction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��������Ͷ������Ӧ
	virtual void OnRspInsInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸�Ͷ������Ӧ
	virtual void OnRspUpdInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������ӽ��ױ�����Ӧ
	virtual void OnRspInsTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸Ľ��ױ�����Ӧ
	virtual void OnRspUpdTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ�����ױ�����Ӧ
	virtual void OnRspDelTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��������Ͷ���߷�����Ӧ
	virtual void OnRspInsInvestorGroup(CThostFtdcInvestorGroupField *pInvestorGroup, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸�Ͷ���߷�����Ӧ
	virtual void OnRspUpdInvestorGroup(CThostFtdcInvestorGroupField *pInvestorGroup, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸Ľ����˺���Ӧ
	virtual void OnRspUpdTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸�Ͷ���ֲ߳���Ӧ
	virtual void OnRspUpdInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������Ӻ�Լ��֤������Ӧ
	virtual void OnRspInsInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸ĺ�Լ��֤������Ӧ
	virtual void OnRspUpdInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ����Լ��֤������Ӧ
	virtual void OnRspDelInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������Ӻ�Լ����������Ӧ
	virtual void OnRspInsInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸ĺ�Լ����������Ӧ
	virtual void OnRspUpdInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ����Լ����������Ӧ
	virtual void OnRspDelInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������Ӻ�Լ����Ȩ����Ӧ
	virtual void OnRspInsInstrumentTradingRight(CThostFtdcInstrumentTradingRightField *pInstrumentTradingRight, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸ĺ�Լ����Ȩ����Ӧ
	virtual void OnRspUpdInstrumentTradingRight(CThostFtdcInstrumentTradingRightField *pInstrumentTradingRight, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ����Լ����Ȩ����Ӧ
	virtual void OnRspDelInstrumentTradingRight(CThostFtdcInstrumentTradingRightField *pInstrumentTradingRight, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������Ӿ��͹�˾�û���Ӧ
	virtual void OnRspInsBrokerUser(CThostFtdcBrokerUserField *pBrokerUser, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸ľ��͹�˾�û���Ӧ
	virtual void OnRspUpdBrokerUser(CThostFtdcBrokerUserField *pBrokerUser, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ�����͹�˾�û���Ӧ
	virtual void OnRspDelBrokerUser(CThostFtdcBrokerUserField *pBrokerUser, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������Ӿ��͹�˾�û�������Ӧ
	virtual void OnRspInsBrokerUserPassword(CThostFtdcBrokerUserPasswordField *pBrokerUserPassword, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸ľ��͹�˾�û�������Ӧ
	virtual void OnRspUpdBrokerUserPassword(CThostFtdcBrokerUserPasswordField *pBrokerUserPassword, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ�����͹�˾�û�������Ӧ
	virtual void OnRspDelBrokerUserPassword(CThostFtdcBrokerUserPasswordField *pBrokerUserPassword, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������Ӿ��͹�˾�û�Ȩ����Ӧ
	virtual void OnRspInsBrokerUserFunction(CThostFtdcBrokerUserFunctionField *pBrokerUserFunction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ�����͹�˾�û�Ȩ����Ӧ
	virtual void OnRspDelBrokerUserFunction(CThostFtdcBrokerUserFunctionField *pBrokerUserFunction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ��Ͷ������Ӧ
	virtual void OnRspDelInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ͬ���������Ӧ
	virtual void OnRspSyncDeposit(CThostFtdcSyncDepositField *pSyncDeposit, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������Ӻ�Լ��֤���ʵ�����Ӧ
	virtual void OnRspInsInstrumentMarginRateAdjust(CThostFtdcInstrumentMarginRateAdjustField *pInstrumentMarginRateAdjust, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸ĺ�Լ��֤���ʵ�����Ӧ
	virtual void OnRspUpdInstrumentMarginRateAdjust(CThostFtdcInstrumentMarginRateAdjustField *pInstrumentMarginRateAdjust, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ����Լ��֤���ʵ�����Ӧ
	virtual void OnRspDelInstrumentMarginRateAdjust(CThostFtdcInstrumentMarginRateAdjustField *pInstrumentMarginRateAdjust, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������ӽ�������֤������Ӧ
	virtual void OnRspInsExchangeMarginRate(CThostFtdcExchangeMarginRateField *pExchangeMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸Ľ�������֤������Ӧ
	virtual void OnRspUpdExchangeMarginRate(CThostFtdcExchangeMarginRateField *pExchangeMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ����������֤������Ӧ
	virtual void OnRspDelExchangeMarginRate(CThostFtdcExchangeMarginRateField *pExchangeMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������ӽ�������֤���ʵ�����Ӧ
	virtual void OnRspInsExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸Ľ�������֤���ʵ�����Ӧ
	virtual void OnRspUpdExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ����������֤���ʵ�����Ӧ
	virtual void OnRspDelExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��������Ͷ�����˻���Ӧ
	virtual void OnRspInsInvestorAccount(CThostFtdcInvestorAccountField *pInvestorAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸�Ͷ�����˻���Ӧ
	virtual void OnRspUpdInvestorAccount(CThostFtdcInvestorAccountField *pInvestorAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ��Ͷ�����˻���Ӧ
	virtual void OnRspDelInvestorAccount(CThostFtdcInvestorAccountField *pInvestorAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������Ӹ���ӯ���㷨��Ӧ
	virtual void OnRspInsPositionProfitAlgorithm(CThostFtdcPositionProfitAlgorithmField *pPositionProfitAlgorithm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸ĸ���ӯ���㷨��Ӧ
	virtual void OnRspUpdPositionProfitAlgorithm(CThostFtdcPositionProfitAlgorithmField *pPositionProfitAlgorithm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ������ӯ���㷨��Ӧ
	virtual void OnRspDelPositionProfitAlgorithm(CThostFtdcPositionProfitAlgorithmField *pPositionProfitAlgorithm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������ӻ�Ա�ʽ��ۿ���Ӧ
	virtual void OnRspInsDiscount(CThostFtdcDiscountField *pDiscount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸Ļ�Ա�ʽ��ۿ���Ӧ
	virtual void OnRspUpdDiscount(CThostFtdcDiscountField *pDiscount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ����Ա�ʽ��ۿ���Ӧ
	virtual void OnRspDelDiscount(CThostFtdcDiscountField *pDiscount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ͬ�����ö����Ӧ
	virtual void OnRspSyncCredit(CThostFtdcSyncDepositField *pSyncDeposit, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ͬ����Ѻ�����Ӧ
	virtual void OnRspSyncMortgage(CThostFtdcSyncDepositField *pSyncDeposit, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���������ʽ��˻�������Ӧ
	virtual void OnRspInsTradingAccountPassword(CThostFtdcTradingAccountPasswordField *pTradingAccountPassword, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸��ʽ��˻�������Ӧ
	virtual void OnRspUpdTradingAccountPassword(CThostFtdcTradingAccountPasswordField *pTradingAccountPassword, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ���ʽ��˻�������Ӧ
	virtual void OnRspDelTradingAccountPassword(CThostFtdcTradingAccountPasswordField *pTradingAccountPassword, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������ӿͻ�֪ͨ��Ӧ
	virtual void OnRspInsNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸Ŀͻ�֪ͨ��Ӧ
	virtual void OnRspUpdNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ���ͻ�֪ͨ��Ӧ
	virtual void OnRspDelNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���������û�Ȩ����Ӧ
	virtual void OnRspInsUserRight(CThostFtdcUserRightField *pUserRight, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸��û�Ȩ����Ӧ
	virtual void OnRspUpdUserRight(CThostFtdcUserRightField *pUserRight, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ���û�Ȩ����Ӧ
	virtual void OnRspDelUserRight(CThostFtdcUserRightField *pUserRight, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ͬ������
	virtual void OnRspSyncCharge(CThostFtdcSyncDepositField *pSyncDeposit, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������Ӿ��͹�˾�����ʽ��㷨����Ӧ
	virtual void OnRspInsBrokerWithdrawAlgorithm(CThostFtdcBrokerWithdrawAlgorithmField *pBrokerWithdrawAlgorithm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸ľ��͹�˾�����ʽ��㷨����Ӧ
	virtual void OnRspUpdBrokerWithdrawAlgorithm(CThostFtdcBrokerWithdrawAlgorithmField *pBrokerWithdrawAlgorithm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ�����͹�˾�����ʽ��㷨����Ӧ
	virtual void OnRspDelBrokerWithdrawAlgorithm(CThostFtdcBrokerWithdrawAlgorithmField *pBrokerWithdrawAlgorithm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��������Ͷ���߿����ʽ��㷨����Ӧ
	virtual void OnRspInsInvestorWithdrawAlgorithm(CThostFtdcInvestorWithdrawAlgorithmField *pInvestorWithdrawAlgorithm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸�Ͷ���߿����ʽ��㷨����Ӧ
	virtual void OnRspUpdInvestorWithdrawAlgorithm(CThostFtdcInvestorWithdrawAlgorithmField *pInvestorWithdrawAlgorithm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ��Ͷ���߿����ʽ��㷨����Ӧ
	virtual void OnRspDelInvestorWithdrawAlgorithm(CThostFtdcInvestorWithdrawAlgorithmField *pInvestorWithdrawAlgorithm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���������û�IP��Ӧ
	virtual void OnRspInsUserIP(CThostFtdcUserIPField *pUserIP, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸��û�IP��Ӧ
	virtual void OnRspUpdUserIP(CThostFtdcUserIPField *pUserIP, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ���û�IP��Ӧ
	virtual void OnRspDelUserIP(CThostFtdcUserIPField *pUserIP, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������ӿͻ�֪ͨ��Ӧ
	virtual void OnRspInsTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸Ŀͻ�֪ͨ��Ӧ
	virtual void OnRspUpdTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ���ͻ�֪ͨ��Ӧ
	virtual void OnRspDelTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���������û���̬���Ʋ�����Ӧ
	virtual void OnRspInsBrokerUserOTPParam(CThostFtdcBrokerUserOTPParamField *pBrokerUserOTPParam, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸��û���̬���Ʋ�����Ӧ
	virtual void OnRspUpdBrokerUserOTPParam(CThostFtdcBrokerUserOTPParamField *pBrokerUserOTPParam, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ���û���̬���Ʋ�����Ӧ
	virtual void OnRspDelBrokerUserOTPParam(CThostFtdcBrokerUserOTPParamField *pBrokerUserOTPParam, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ֹ�ͬ���û���̬������Ӧ
	virtual void OnRspManualSyncBrokerUserOTP(CThostFtdcManualSyncBrokerUserOTPField *pManualSyncBrokerUserOTP, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��������Ͷ������������ģ����Ӧ
	virtual void OnRspInsCommRateModel(CThostFtdcCommRateModelField *pCommRateModel, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸�Ͷ������������ģ����Ӧ
	virtual void OnRspUpdCommRateModel(CThostFtdcCommRateModelField *pCommRateModel, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ��Ͷ������������ģ����Ӧ
	virtual void OnRspDelCommRateModel(CThostFtdcCommRateModelField *pCommRateModel, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���������ֱ�����ת��������Ӧ
	virtual void OnRspInsDRTransfer(CThostFtdcDRTransferField *pDRTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ������Ӧ
	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ�ɽ���Ӧ
	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯͶ���ֲ߳���Ӧ
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ�ʽ��˻���Ӧ
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯͶ������Ӧ
	virtual void OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ���ױ�����Ӧ
	virtual void OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯͶ���߷�����Ӧ
	virtual void OnRspQryInvestorGroup(CThostFtdcInvestorGroupField *pInvestorGroup, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ��Լ��֤������Ӧ
	virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ��Լ����������Ӧ
	virtual void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ��Լ����Ȩ����Ӧ
	virtual void OnRspQryInstrumentTradingRight(CThostFtdcInstrumentTradingRightField *pInstrumentTradingRight, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ���͹�˾��Ӧ
	virtual void OnRspQryBroker(CThostFtdcBrokerField *pBroker, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ����Ա��Ӧ
	virtual void OnRspQryTrader(CThostFtdcTraderField *pTrader, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ���͹�˾��Ա������Ӧ
	virtual void OnRspQryPartBroker(CThostFtdcPartBrokerField *pPartBroker, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ�����û�����Ȩ����Ӧ
	virtual void OnRspQrySuperUserFunction(CThostFtdcSuperUserFunctionField *pSuperUserFunction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ�û��Ự��Ӧ
	virtual void OnRspQryUserSession(CThostFtdcUserSessionField *pUserSession, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯǰ��״̬��Ӧ
	virtual void OnRspQryFrontStatus(CThostFtdcFrontStatusField *pFrontStatus, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ������������Ӧ
	virtual void OnRspQryExchangeOrder(CThostFtdcExchangeOrderField *pExchangeOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����鱨��������Ӧ
	virtual void OnRspQryOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ����������������Ӧ
	virtual void OnRspQryExchangeOrderAction(CThostFtdcExchangeOrderActionField *pExchangeOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ�����û���Ӧ
	virtual void OnRspQrySuperUser(CThostFtdcSuperUserField *pSuperUser, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ��������Ӧ
	virtual void OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ��Ʒ��Ӧ
	virtual void OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ��Լ��Ӧ
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ������Ӧ
	virtual void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ���͹�˾�û���Ӧ
	virtual void OnRspQryBrokerUser(CThostFtdcBrokerUserField *pBrokerUser, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ���͹�˾�û�Ȩ����Ӧ
	virtual void OnRspQryBrokerUserFunction(CThostFtdcBrokerUserFunctionField *pBrokerUserFunction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ����Ա���̻���Ӧ
	virtual void OnRspQryTraderOffer(CThostFtdcTraderOfferField *pTraderOffer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ�������ˮ��Ӧ
	virtual void OnRspQrySyncDeposit(CThostFtdcSyncDepositField *pSyncDeposit, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯͶ���߽�������Ӧ
	virtual void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ��Լ״̬��Ӧ
	virtual void OnRspQryInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯǿƽ�����Ͷ���ֲ߳���Ӧ
	virtual void OnRspQryForceClosePosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯת��������Ӧ
	virtual void OnRspQryTransferBank(CThostFtdcTransferBankField *pTransferBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯͶ���ֲ߳���ϸ��Ӧ
	virtual void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ���鱨�̻���Ӧ
	virtual void OnRspQryMDTraderOffer(CThostFtdcMDTraderOfferField *pMDTraderOffer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ�ͻ�֪ͨ��Ӧ
	virtual void OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ������Ϣȷ����Ӧ
	virtual void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ��Ϻ�Լ������Ӧ
	virtual void OnRspQryCombinationLeg(CThostFtdcCombinationLegField *pCombinationLeg, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ����ͬ��״̬��Ӧ
	virtual void OnRspQrySyncStatus(CThostFtdcSyncStatusField *pSyncStatus, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ��ϵ����Ӧ
	virtual void OnRspQryLinkMan(CThostFtdcLinkManField *pLinkMan, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ��ϵ����Ӧ
	virtual void OnRspQryBrokerUserEvent(CThostFtdcBrokerUserEventField *pBrokerUserEvent, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯͶ���ֲ߳���ϸ��Ӧ
	virtual void OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ���󱨵���Ӧ
	virtual void OnRspQryErrOrder(CThostFtdcErrOrderField *pErrOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ���󱨵�������Ӧ
	virtual void OnRspQryErrOrderAction(CThostFtdcErrOrderActionField *pErrOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ������״̬��Ӧ
	virtual void OnRspQryExchangeSequence(CThostFtdcExchangeSequenceField *pExchangeSequence, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ��֤����ϵͳ���͹�˾��Կ��Ӧ
	virtual void OnRspQryCFMMCBrokerKey(CThostFtdcCFMMCBrokerKeyField *pCFMMCBrokerKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ��Ӧ
	virtual void OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯͶ������������ģ����Ӧ
	virtual void OnRspQryCommRateModel(CThostFtdcCommRateModelField *pCommRateModel, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ�ֵ��۵���Ϣ��Ӧ
	virtual void OnRspQryEWarrantOffset(CThostFtdcEWarrantOffsetField *pEWarrantOffset, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ��ʷ������Ӧ
	virtual void OnRspHisQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯת����ˮ��Ӧ
	virtual void OnRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ����ǩԼ��ϵ��Ӧ
	virtual void OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����Ӧ��
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��������Ӧ��
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���ݽ��������붩������Ӧ��
	virtual void OnRspSubMarketDataByExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���ݽ���������ȡ����������Ӧ��
	virtual void OnRspUnSubMarketDataByExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����֪ͨ
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder) {};

	///�ɽ�֪ͨ
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade) {};

	///����¼�����ر�
	virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo) {};

	///������������ر�
	virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo) {};

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {};

	///��Լ����״̬֪ͨ
	virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus) {};

	///Ԥ��¼�����ر�
	virtual void OnErrRtnParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo) {};

	///Ԥ�񳷵�¼�����ر�
	virtual void OnErrRtnParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo) {};

	///ɾ��Ԥ��¼�����ر�
	virtual void OnErrRtnRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo) {};

	///ɾ��Ԥ�񳷵�¼�����ر�
	virtual void OnErrRtnRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo) {};

	///����֪ͨ
	virtual void OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo) {};

	///��ʾ������У�����
	virtual void OnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField *pErrorConditionalOrder) {};

	///Ͷ�����ʽ�
	virtual void OnRtnBrokerDeposit(CThostFtdcBrokerDepositField *pBrokerDeposit) {};

	///�����ѯǩԼ������Ӧ
	virtual void OnRspQryContractBank(CThostFtdcContractBankField *pContractBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯԤ����Ӧ
	virtual void OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯԤ�񳷵���Ӧ
	virtual void OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ����֪ͨ��Ӧ
	virtual void OnRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ���͹�˾���ײ�����Ӧ
	virtual void OnRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField *pBrokerTradingParams, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ���͹�˾�����㷨��Ӧ
	virtual void OnRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField *pBrokerTradingAlgos, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ���͹�˾�ʽ���Ӧ
	virtual void OnRspQueryBrokerDeposit(CThostFtdcQueryBrokerDepositField *pQueryBrokerDeposit, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���������û��µ�Ȩ�޷�����Ӧ
	virtual void OnRspInsUserRightsAssign(CThostFtdcUserRightsAssignField *pUserRightsAssign, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸��û��µ�Ȩ�޷�����Ӧ
	virtual void OnRspUpdUserRightsAssign(CThostFtdcUserRightsAssignField *pUserRightsAssign, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ���û��µ�Ȩ�޷�����Ӧ
	virtual void OnRspDelUserRightsAssign(CThostFtdcUserRightsAssignField *pUserRightsAssign, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������Ӿ��ù�˾�µ�Ȩ�޷�����Ӧ
	virtual void OnRspInsBrokerUserRightAssign(CThostFtdcBrokerUserRightAssignField *pBrokerUserRightAssign, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸ľ��ù�˾�µ�Ȩ�޷�����Ӧ
	virtual void OnRspUpdBrokerUserRightAssign(CThostFtdcBrokerUserRightAssignField *pBrokerUserRightAssign, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ�����ù�˾�µ�Ȩ�޷�����Ӧ
	virtual void OnRspDelBrokerUserRightAssign(CThostFtdcBrokerUserRightAssignField *pBrokerUserRightAssign, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���з��������ʽ�ת�ڻ�֪ͨ
	virtual void OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer) {};

	///���з����ڻ��ʽ�ת����֪ͨ
	virtual void OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer) {};

	///���з����������ת�ڻ�֪ͨ
	virtual void OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal) {};

	///���з�������ڻ�ת����֪ͨ
	virtual void OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal) {};

	///�ڻ����������ʽ�ת�ڻ�֪ͨ
	virtual void OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer) {};

	///�ڻ������ڻ��ʽ�ת����֪ͨ
	virtual void OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer) {};

	///ϵͳ����ʱ�ڻ����ֹ������������ת�ڻ��������д�����Ϻ��̷��ص�֪ͨ
	virtual void OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal) {};

	///ϵͳ����ʱ�ڻ����ֹ���������ڻ�ת�����������д�����Ϻ��̷��ص�֪ͨ
	virtual void OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal) {};

	///�ڻ������ѯ�������֪ͨ
	virtual void OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount) {};

	///����ǩ��֪ͨ
	virtual void OnRtnFutureSignIn(CThostFtdcNotifyFutureSignInField *pNotifyFutureSignIn) {};

	///����ǩ��֪ͨ
	virtual void OnRtnFutureSignOut(CThostFtdcNotifyFutureSignOutField *pNotifyFutureSignOut) {};

	///��Կͬ��֪ͨ
	virtual void OnRtnSyncKey(CThostFtdcNotifySyncKeyField *pNotifySyncKey) {};

	///�ڻ����������ʽ�ת�ڻ�����ر�
	virtual void OnErrRtnBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo) {};

	///�ڻ������ڻ��ʽ�ת���д���ر�
	virtual void OnErrRtnFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo) {};

	///ϵͳ����ʱ�ڻ����ֹ������������ת�ڻ�����ر�
	virtual void OnErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo) {};

	///ϵͳ����ʱ�ڻ����ֹ���������ڻ�ת���д���ر�
	virtual void OnErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo) {};

	///�ڻ������ѯ����������ر�
	virtual void OnErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo) {};

	///�ڻ���������ǩ������ر�
	virtual void OnErrRtnFutureSignIn(CThostFtdcFutureSignIOField *pFutureSignIO, CThostFtdcRspInfoField *pRspInfo) {};

	///�ڻ���������ǩ�˴���ر�
	virtual void OnErrRtnFutureSignOut(CThostFtdcFutureSignIOField *pFutureSignIO, CThostFtdcRspInfoField *pRspInfo) {};

	///�ڻ�������Կͬ������ر�
	virtual void OnErrRtnSyncKey(CThostFtdcReqSyncKeyField *pReqSyncKey, CThostFtdcRspInfoField *pRspInfo) {};

	///�ڻ������������ת�ڻ��������д�����Ϻ��̷��ص�֪ͨ
	virtual void OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal) {};

	///�ڻ���������ڻ�ת�����������д�����Ϻ��̷��ص�֪ͨ
	virtual void OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal) {};

	///���з���ת�ʿ���Ӧ��
	virtual void OnRspOpenAccountByBank(CThostFtdcReqOpenAccountField *pReqOpenAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���з���ת������Ӧ��
	virtual void OnRspCancelAccountByBank(CThostFtdcReqOpenAccountField *pReqOpenAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���з����������˻�Ӧ��
	virtual void OnRspChangeAccountByBank(CThostFtdcReqChangeAccountField *pReqChangeAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���з��������ʽ�ת�ڻ�Ӧ��
	virtual void OnRspFromBankToFutureByBank(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���з����ڻ��ʽ�ת����Ӧ��
	virtual void OnRspFromFutureToBankByBank(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�ڻ����������ʽ�ת�ڻ�Ӧ��
	virtual void OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�ڻ������ڻ��ʽ�ת����Ӧ��
	virtual void OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��������ת�ڻ�Ӧ��
	virtual void OnRspRepealFromBankToFuture(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ڻ�ת����Ӧ��
	virtual void OnRspRepealFromFutureToBank(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ϵͳ����ʱ�ڻ����ֹ������������ת�ڻ�Ӧ��
	virtual void OnRspRepealFromBankToFutureByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ϵͳ����ʱ�ڻ����ֹ���������ڻ�ת����Ӧ��
	virtual void OnRspRepealFromFutureToBankByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�ڻ������ѯ�������Ӧ��
	virtual void OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���з����ѯ�˻���Ӧ��ϵӦ��
	virtual void OnRspQueryAccountRelationByBank(CThostFtdcRspQueryAccountField *pRspQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�ڻ�������˿ͻ���ѯ����ֱͨ����ͨ���Ӧ��
	virtual void OnRspQueryFBTOpenAccountStatusByFuture(CThostFtdcRspQueryAccountField *pRspQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���з�����֤�ڻ�Ͷ���˺����벢��ѯ�˻���ϢӦ��
	virtual void OnRspVerifyFutureAccPWAndQueryInfoByBank(CThostFtdcRspQueryAccountField *pRspQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ǩ��Ӧ��
	virtual void OnRspFutureSignIn(CThostFtdcFutureSignIOField *pFutureSignIO, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ǩ��Ӧ��
	virtual void OnRspFutureSignOut(CThostFtdcFutureSignIOField *pFutureSignIO, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���з��������ļ�׼������Ӧ��
	virtual void OnRspDayEndFileReadyByBank(CThostFtdcReqDayEndFileReadyField *pReqDayEndFileReady, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���ڱ������׺��ķ�����֤�ڻ��ʽ�����Ӧ��
	virtual void OnRspVerifyFuturePassword(CThostFtdcVerifyFuturePasswordField *pVerifyFuturePassword, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���ڱ������׺��ķ�����֤�ͻ���ϢӦ��
	virtual void OnRspVerifyCustInfo(CThostFtdcVerifyCustInfoField *pVerifyCustInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���ڱ������׺��ķ�����֤�ڻ��ʽ�����Ϳͻ���ϢӦ��
	virtual void OnRspVerifyFuturePasswordAndCustInfo(CThostFtdcVerifyFuturePasswordAndCustInfoField *pVerifyFuturePasswordAndCustInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���׺��������ڱ��̷�����Կͬ��Ӧ��
	virtual void OnRspSyncKey(CThostFtdcReqSyncKeyField *pReqSyncKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���з������ڿ���֪ͨ
	virtual void OnRtnOpenAccountByBank(CThostFtdcOpenAccountField *pOpenAccount) {};

	///���з�����������֪ͨ
	virtual void OnRtnCancelAccountByBank(CThostFtdcCancelAccountField *pCancelAccount) {};

	///���з����������˺�֪ͨ
	virtual void OnRtnChangeAccountByBank(CThostFtdcChangeAccountField *pChangeAccount) {};
};

class USER_API_EXPORT CThostFtdcUserApi
{
public:
	///����UserApi
	///@param pszFlowPath ����������Ϣ�ļ���Ŀ¼��Ĭ��Ϊ��ǰĿ¼
	///@return ��������UserApi
	//modify for udp marketdata
	static CThostFtdcUserApi *CreateFtdcUserApi(const char *pszFlowPath = "", const bool bIsUsingUdp=false);
	
	///ɾ���ӿڶ�����
	///@remark ����ʹ�ñ��ӿڶ���ʱ,���øú���ɾ���ӿڶ���
	virtual void Release() = 0;
	
	///��ʼ��
	///@remark ��ʼ�����л���,ֻ�е��ú�,�ӿڲſ�ʼ����
	virtual void Init() = 0;
	
	///�ȴ��ӿ��߳̽�������
	///@return �߳��˳�����
	virtual int Join() = 0;
	
	///��ȡ��ǰ������
	///@retrun ��ȡ���Ľ�����
	///@remark ֻ�е�¼�ɹ���,���ܵõ���ȷ�Ľ�����
	virtual const char *GetTradingDay() = 0;
	
	///ע��ǰ�û������ַ
	///@param pszFrontAddress��ǰ�û������ַ��
	///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001���� 
	///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
	virtual void RegisterFront(char *pszFrontAddress) = 0;
	
	///ע��ص��ӿ�
	///@param pSpi �����Իص��ӿ����ʵ��
	virtual void RegisterSpi(CThostFtdcUserSpi *pSpi) = 0;
	
	virtual int SubscribeMarketData(char *ppInstrumentID[], int nCount) = 0;

	virtual int UnSubscribeMarketData(char *ppInstrumentID[], int nCount) = 0;
	///����˽������
	///@param nResumeType ˽�����ش���ʽ  
	///        TERT_RESTART:�ӱ������տ�ʼ�ش�
	///        TERT_RESUME:���ϴ��յ�������
	///        TERT_QUICK:ֻ���͵�¼��˽����������
	///@remark �÷���Ҫ��Init����ǰ���á����������򲻻��յ�˽���������ݡ�
	virtual void SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType) = 0;
	
	///���Ĺ�������
	///@param nResumeType �������ش���ʽ  
	///        THOST_TERT_RESTART:�ӱ������տ�ʼ�ش�
	///        THOST_TERT_RESUME:���ϴ��յ�������
	///        THOST_TERT_QUICK:ֻ���͵�¼�󹫹���������
	///@remark �÷���Ҫ��Init����ǰ���á����������򲻻��յ������������ݡ�
	virtual void SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType) = 0;

  ///�ͻ�����֤����
	virtual int ReqAuthenticate(CThostFtdcReqAuthenticateField *pReqAuthenticateField, int nRequestID) = 0;

	///�û���¼����
	virtual int ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID) = 0;

	///�����������
	virtual int ReqDataDump(CThostFtdcSettlementRefField *pSettlementRef, int nRequestID) = 0;

	///װ�ؽ�����Ϣ����
	virtual int ReqLoadSettlementInfo(CThostFtdcLoadSettlementInfoField *pLoadSettlementInfo, int nRequestID) = 0;

	///�ǳ�����
	virtual int ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID) = 0;

	///ȫ���ǳ�����
	virtual int ReqLogoutAll(CThostFtdcLogoutAllField *pLogoutAll, int nRequestID) = 0;

	///ǿ���û��ǳ�����
	virtual int ReqForceUserLogout(CThostFtdcForceUserLogoutField *pForceUserLogout, int nRequestID) = 0;

	///�û������������
	virtual int ReqUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, int nRequestID) = 0;

	///ǿ���û��˳�����
	virtual int ReqForceUserExit(CThostFtdcForceUserLogoutField *pForceUserLogout, int nRequestID) = 0;

	///�ʽ��˻������������
	virtual int ReqTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, int nRequestID) = 0;

	///����¼������
	virtual int ReqOrderInsert(CThostFtdcInputOrderField *pInputOrder, int nRequestID) = 0;

	///Ԥ��¼������
	virtual int ReqParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, int nRequestID) = 0;

	///Ԥ�񳷵�¼������
	virtual int ReqParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, int nRequestID) = 0;

	///������������
	virtual int ReqOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, int nRequestID) = 0;

	///��ѯ��󱨵���������
	virtual int ReqQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, int nRequestID) = 0;

	///Ͷ���߽�����ȷ��
	virtual int ReqSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, int nRequestID) = 0;

	///���������ʽ�ת�ڻ�
	virtual int ReqTransferBankToFuture(CThostFtdcTransferHeaderField *pTransferHeader, CThostFtdcTransferBankToFutureReqField *pTransferBankToFutureReq, int nRequestID) = 0;

	///�����ڻ��ʽ�ת����
	virtual int ReqTransferFutureToBank(CThostFtdcTransferHeaderField *pTransferHeader, CThostFtdcTransferFutureToBankReqField *pTransferFutureToBankReq, int nRequestID) = 0;

	///�����ѯ�����ʽ�
	virtual int ReqTransferQryBank(CThostFtdcTransferHeaderField *pTransferHeader, CThostFtdcTransferQryBankReqField *pTransferQryBankReq, int nRequestID) = 0;

	///�����ѯ���н�����ϸ
	virtual int ReqTransferQryDetail(CThostFtdcTransferHeaderField *pTransferHeader, CThostFtdcTransferQryDetailReqField *pTransferQryDetailReq, int nRequestID) = 0;

	///����ɾ��Ԥ��
	virtual int ReqRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, int nRequestID) = 0;

	///����ɾ��Ԥ�񳷵�
	virtual int ReqRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, int nRequestID) = 0;

	///����У��Ͷ��������
	virtual int ReqVerifyInvestorPassword(CThostFtdcVerifyInvestorPasswordField *pVerifyInvestorPassword, int nRequestID) = 0;

	///���ݼ۸��ѯ��󱨵���������
	virtual int ReqQueryMaxOrderVolumeWithPrice(CThostFtdcQueryMaxOrderVolumeWithPriceField *pQueryMaxOrderVolumeWithPrice, int nRequestID) = 0;

	///�������Ӳ�Ʒ
	virtual int ReqInsProduct(CThostFtdcProductField *pProduct, int nRequestID) = 0;

	///�����޸Ĳ�Ʒ
	virtual int ReqUpdProduct(CThostFtdcProductField *pProduct, int nRequestID) = 0;

	///�������Ӻ�Լ
	virtual int ReqInsInstrument(CThostFtdcInstrumentField *pInstrument, int nRequestID) = 0;

	///�����޸ĺ�Լ
	virtual int ReqUpdInstrument(CThostFtdcInstrumentField *pInstrument, int nRequestID) = 0;

	///�������Ӿ��͹�˾
	virtual int ReqInsBroker(CThostFtdcBrokerField *pBroker, int nRequestID) = 0;

	///�����޸ľ��͹�˾
	virtual int ReqUpdBroker(CThostFtdcBrokerField *pBroker, int nRequestID) = 0;

	///�������ӽ���Ա
	virtual int ReqInsTrader(CThostFtdcTraderField *pTrader, int nRequestID) = 0;

	///�����޸Ľ���Ա
	virtual int ReqUpdTrader(CThostFtdcTraderField *pTrader, int nRequestID) = 0;

	///�������ӻ�Ա���͹�˾��ϵ
	virtual int ReqInsPartBroker(CThostFtdcPartBrokerField *pPartBroker, int nRequestID) = 0;

	///�����޸Ļ�Ա���͹�˾��ϵ
	virtual int ReqUpdPartBroker(CThostFtdcPartBrokerField *pPartBroker, int nRequestID) = 0;

	///�������ӹ����û�
	virtual int ReqInsSuperUser(CThostFtdcSuperUserField *pSuperUser, int nRequestID) = 0;

	///�����޸Ĺ����û�
	virtual int ReqUpdSuperUser(CThostFtdcSuperUserField *pSuperUser, int nRequestID) = 0;

	///�������ӹ����û�Ȩ��
	virtual int ReqInsSuperUserFunction(CThostFtdcSuperUserFunctionField *pSuperUserFunction, int nRequestID) = 0;

	///����ɾ�������û�Ȩ��
	virtual int ReqDelSuperUserFunction(CThostFtdcSuperUserFunctionField *pSuperUserFunction, int nRequestID) = 0;

	///��������Ͷ����
	virtual int ReqInsInvestor(CThostFtdcInvestorField *pInvestor, int nRequestID) = 0;

	///�����޸�Ͷ����
	virtual int ReqUpdInvestor(CThostFtdcInvestorField *pInvestor, int nRequestID) = 0;

	///�������ӽ��ױ���
	virtual int ReqInsTradingCode(CThostFtdcTradingCodeField *pTradingCode, int nRequestID) = 0;

	///�����޸Ľ��ױ���
	virtual int ReqUpdTradingCode(CThostFtdcTradingCodeField *pTradingCode, int nRequestID) = 0;

	///����ɾ�����ױ���
	virtual int ReqDelTradingCode(CThostFtdcTradingCodeField *pTradingCode, int nRequestID) = 0;

	///��������Ͷ���߷���
	virtual int ReqInsInvestorGroup(CThostFtdcInvestorGroupField *pInvestorGroup, int nRequestID) = 0;

	///�����޸�Ͷ���߷���
	virtual int ReqUpdInvestorGroup(CThostFtdcInvestorGroupField *pInvestorGroup, int nRequestID) = 0;

	///�����޸Ľ����˺�
	virtual int ReqUpdTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, int nRequestID) = 0;

	///�����޸�Ͷ���ֲ߳�
	virtual int ReqUpdInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, int nRequestID) = 0;

	///�������Ӻ�Լ��֤����
	virtual int ReqInsInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, int nRequestID) = 0;

	///�����޸ĺ�Լ��֤����
	virtual int ReqUpdInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, int nRequestID) = 0;

	///����ɾ����Լ��֤����
	virtual int ReqDelInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, int nRequestID) = 0;

	///�������Ӻ�Լ��������
	virtual int ReqInsInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, int nRequestID) = 0;

	///�����޸ĺ�Լ��������
	virtual int ReqUpdInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, int nRequestID) = 0;

	///����ɾ����Լ��������
	virtual int ReqDelInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, int nRequestID) = 0;

	///�������Ӻ�Լ����Ȩ��
	virtual int ReqInsInstrumentTradingRight(CThostFtdcInstrumentTradingRightField *pInstrumentTradingRight, int nRequestID) = 0;

	///�����޸ĺ�Լ����Ȩ��
	virtual int ReqUpdInstrumentTradingRight(CThostFtdcInstrumentTradingRightField *pInstrumentTradingRight, int nRequestID) = 0;

	///����ɾ����Լ����Ȩ��
	virtual int ReqDelInstrumentTradingRight(CThostFtdcInstrumentTradingRightField *pInstrumentTradingRight, int nRequestID) = 0;

	///�������Ӿ��͹�˾�û�
	virtual int ReqInsBrokerUser(CThostFtdcBrokerUserField *pBrokerUser, int nRequestID) = 0;

	///�����޸ľ��͹�˾�û�
	virtual int ReqUpdBrokerUser(CThostFtdcBrokerUserField *pBrokerUser, int nRequestID) = 0;

	///����ɾ�����͹�˾�û�
	virtual int ReqDelBrokerUser(CThostFtdcBrokerUserField *pBrokerUser, int nRequestID) = 0;

	///�������Ӿ��͹�˾�û�����
	virtual int ReqInsBrokerUserPassword(CThostFtdcBrokerUserPasswordField *pBrokerUserPassword, int nRequestID) = 0;

	///�����޸ľ��͹�˾�û�����
	virtual int ReqUpdBrokerUserPassword(CThostFtdcBrokerUserPasswordField *pBrokerUserPassword, int nRequestID) = 0;

	///����ɾ�����͹�˾�û�����
	virtual int ReqDelBrokerUserPassword(CThostFtdcBrokerUserPasswordField *pBrokerUserPassword, int nRequestID) = 0;

	///�������Ӿ��͹�˾�û�Ȩ��
	virtual int ReqInsBrokerUserFunction(CThostFtdcBrokerUserFunctionField *pBrokerUserFunction, int nRequestID) = 0;

	///����ɾ�����͹�˾�û�Ȩ��
	virtual int ReqDelBrokerUserFunction(CThostFtdcBrokerUserFunctionField *pBrokerUserFunction, int nRequestID) = 0;

	///����ɾ��Ͷ����
	virtual int ReqDelInvestor(CThostFtdcInvestorField *pInvestor, int nRequestID) = 0;

	///����ͬ�������
	virtual int ReqSyncDeposit(CThostFtdcSyncDepositField *pSyncDeposit, int nRequestID) = 0;

	///�������Ӻ�Լ��֤���ʵ���
	virtual int ReqInsInstrumentMarginRateAdjust(CThostFtdcInstrumentMarginRateAdjustField *pInstrumentMarginRateAdjust, int nRequestID) = 0;

	///�����޸ĺ�Լ��֤���ʵ���
	virtual int ReqUpdInstrumentMarginRateAdjust(CThostFtdcInstrumentMarginRateAdjustField *pInstrumentMarginRateAdjust, int nRequestID) = 0;

	///����ɾ����Լ��֤���ʵ���
	virtual int ReqDelInstrumentMarginRateAdjust(CThostFtdcInstrumentMarginRateAdjustField *pInstrumentMarginRateAdjust, int nRequestID) = 0;

	///�������ӽ�������֤����
	virtual int ReqInsExchangeMarginRate(CThostFtdcExchangeMarginRateField *pExchangeMarginRate, int nRequestID) = 0;

	///�����޸Ľ�������֤����
	virtual int ReqUpdExchangeMarginRate(CThostFtdcExchangeMarginRateField *pExchangeMarginRate, int nRequestID) = 0;

	///����ɾ����������֤����
	virtual int ReqDelExchangeMarginRate(CThostFtdcExchangeMarginRateField *pExchangeMarginRate, int nRequestID) = 0;

	///�������ӽ�������֤���ʵ���
	virtual int ReqInsExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust, int nRequestID) = 0;

	///�����޸Ľ�������֤���ʵ���
	virtual int ReqUpdExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust, int nRequestID) = 0;

	///����ɾ����������֤���ʵ���
	virtual int ReqDelExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust, int nRequestID) = 0;

	///��������Ͷ�����˻�
	virtual int ReqInsInvestorAccount(CThostFtdcInvestorAccountField *pInvestorAccount, int nRequestID) = 0;

	///�����޸�Ͷ�����˻�
	virtual int ReqUpdInvestorAccount(CThostFtdcInvestorAccountField *pInvestorAccount, int nRequestID) = 0;

	///����ɾ��Ͷ�����˻�
	virtual int ReqDelInvestorAccount(CThostFtdcInvestorAccountField *pInvestorAccount, int nRequestID) = 0;

	///�������Ӹ���ӯ���㷨
	virtual int ReqInsPositionProfitAlgorithm(CThostFtdcPositionProfitAlgorithmField *pPositionProfitAlgorithm, int nRequestID) = 0;

	///�����޸ĸ���ӯ���㷨
	virtual int ReqUpdPositionProfitAlgorithm(CThostFtdcPositionProfitAlgorithmField *pPositionProfitAlgorithm, int nRequestID) = 0;

	///����ɾ������ӯ���㷨
	virtual int ReqDelPositionProfitAlgorithm(CThostFtdcPositionProfitAlgorithmField *pPositionProfitAlgorithm, int nRequestID) = 0;

	///�������ӻ�Ա�ʽ��ۿ�
	virtual int ReqInsDiscount(CThostFtdcDiscountField *pDiscount, int nRequestID) = 0;

	///�����޸Ļ�Ա�ʽ��ۿ�
	virtual int ReqUpdDiscount(CThostFtdcDiscountField *pDiscount, int nRequestID) = 0;

	///����ɾ����Ա�ʽ��ۿ�
	virtual int ReqDelDiscount(CThostFtdcDiscountField *pDiscount, int nRequestID) = 0;

	///����ͬ�����ö��
	virtual int ReqSyncCredit(CThostFtdcSyncDepositField *pSyncDeposit, int nRequestID) = 0;

	///����ͬ����Ѻ���
	virtual int ReqSyncMortgage(CThostFtdcSyncDepositField *pSyncDeposit, int nRequestID) = 0;

	///���������ʽ��˻�����
	virtual int ReqInsTradingAccountPassword(CThostFtdcTradingAccountPasswordField *pTradingAccountPassword, int nRequestID) = 0;

	///�����޸��ʽ��˻�����
	virtual int ReqUpdTradingAccountPassword(CThostFtdcTradingAccountPasswordField *pTradingAccountPassword, int nRequestID) = 0;

	///����ɾ���ʽ��˻�����
	virtual int ReqDelTradingAccountPassword(CThostFtdcTradingAccountPasswordField *pTradingAccountPassword, int nRequestID) = 0;

	///�������ӿͻ�֪ͨ
	virtual int ReqInsNotice(CThostFtdcNoticeField *pNotice, int nRequestID) = 0;

	///�����޸Ŀͻ�֪ͨ
	virtual int ReqUpdNotice(CThostFtdcNoticeField *pNotice, int nRequestID) = 0;

	///����ɾ���ͻ�֪ͨ
	virtual int ReqDelNotice(CThostFtdcNoticeField *pNotice, int nRequestID) = 0;

	///���������û�Ȩ��
	virtual int ReqInsUserRight(CThostFtdcUserRightField *pUserRight, int nRequestID) = 0;

	///�����޸��û�Ȩ��
	virtual int ReqUpdUserRight(CThostFtdcUserRightField *pUserRight, int nRequestID) = 0;

	///����ɾ���û�Ȩ��
	virtual int ReqDelUserRight(CThostFtdcUserRightField *pUserRight, int nRequestID) = 0;

	///����ͬ������
	virtual int ReqSyncCharge(CThostFtdcSyncDepositField *pSyncDeposit, int nRequestID) = 0;

	///�������Ӿ��͹�˾�����ʽ��㷨��
	virtual int ReqInsBrokerWithdrawAlgorithm(CThostFtdcBrokerWithdrawAlgorithmField *pBrokerWithdrawAlgorithm, int nRequestID) = 0;

	///�����޸ľ��͹�˾�����ʽ��㷨��
	virtual int ReqUpdBrokerWithdrawAlgorithm(CThostFtdcBrokerWithdrawAlgorithmField *pBrokerWithdrawAlgorithm, int nRequestID) = 0;

	///����ɾ�����͹�˾�����ʽ��㷨��
	virtual int ReqDelBrokerWithdrawAlgorithm(CThostFtdcBrokerWithdrawAlgorithmField *pBrokerWithdrawAlgorithm, int nRequestID) = 0;

	///��������Ͷ���߿����ʽ��㷨��
	virtual int ReqInsInvestorWithdrawAlgorithm(CThostFtdcInvestorWithdrawAlgorithmField *pInvestorWithdrawAlgorithm, int nRequestID) = 0;

	///�����޸�Ͷ���߿����ʽ��㷨��
	virtual int ReqUpdInvestorWithdrawAlgorithm(CThostFtdcInvestorWithdrawAlgorithmField *pInvestorWithdrawAlgorithm, int nRequestID) = 0;

	///����ɾ��Ͷ���߿����ʽ��㷨��
	virtual int ReqDelInvestorWithdrawAlgorithm(CThostFtdcInvestorWithdrawAlgorithmField *pInvestorWithdrawAlgorithm, int nRequestID) = 0;

	///���������û�IP
	virtual int ReqInsUserIP(CThostFtdcUserIPField *pUserIP, int nRequestID) = 0;

	///�����޸��û�IP
	virtual int ReqUpdUserIP(CThostFtdcUserIPField *pUserIP, int nRequestID) = 0;

	///����ɾ���û�IP
	virtual int ReqDelUserIP(CThostFtdcUserIPField *pUserIP, int nRequestID) = 0;

	///�������ӿͻ�֪ͨ
	virtual int ReqInsTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, int nRequestID) = 0;

	///�����޸Ŀͻ�֪ͨ
	virtual int ReqUpdTradingNotice(CThostFtdcNoticeField *pNotice, int nRequestID) = 0;

	///����ɾ���ͻ�֪ͨ
	virtual int ReqDelTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, int nRequestID) = 0;

	///���������û���̬���Ʋ���
	virtual int ReqInsBrokerUserOTPParam(CThostFtdcBrokerUserOTPParamField *pBrokerUserOTPParam, int nRequestID) = 0;

	///�����޸��û���̬���Ʋ���
	virtual int ReqUpdBrokerUserOTPParam(CThostFtdcBrokerUserOTPParamField *pBrokerUserOTPParam, int nRequestID) = 0;

	///����ɾ���û���̬���Ʋ���
	virtual int ReqDelBrokerUserOTPParam(CThostFtdcBrokerUserOTPParamField *pBrokerUserOTPParam, int nRequestID) = 0;

	///�����ֹ�ͬ���û���̬����
	virtual int ReqManualSyncBrokerUserOTP(CThostFtdcManualSyncBrokerUserOTPField *pManualSyncBrokerUserOTP, int nRequestID) = 0;

	///��������Ͷ������������ģ��
	virtual int ReqInsCommRateModel(CThostFtdcCommRateModelField *pCommRateModel, int nRequestID) = 0;

	///�����޸�Ͷ������������ģ��
	virtual int ReqUpdCommRateModel(CThostFtdcCommRateModelField *pCommRateModel, int nRequestID) = 0;

	///����ɾ��Ͷ������������ģ��
	virtual int ReqDelCommRateModel(CThostFtdcCommRateModelField *pCommRateModel, int nRequestID) = 0;

	///���������ֱ�����ת������
	virtual int ReqInsDRTransfer(CThostFtdcDRTransferField *pDRTransfer, int nRequestID) = 0;

	///�����ѯ����
	virtual int ReqQryOrder(CThostFtdcQryOrderField *pQryOrder, int nRequestID) = 0;

	///�����ѯ�ɽ�
	virtual int ReqQryTrade(CThostFtdcQryTradeField *pQryTrade, int nRequestID) = 0;

	///�����ѯͶ���ֲ߳�
	virtual int ReqQryInvestorPosition(CThostFtdcQryInvestorPositionField *pQryInvestorPosition, int nRequestID) = 0;

	///�����ѯ�ʽ��˻�
	virtual int ReqQryTradingAccount(CThostFtdcQryTradingAccountField *pQryTradingAccount, int nRequestID) = 0;

	///�����ѯͶ����
	virtual int ReqQryInvestor(CThostFtdcQryInvestorField *pQryInvestor, int nRequestID) = 0;

	///�����ѯ���ױ���
	virtual int ReqQryTradingCode(CThostFtdcQryTradingCodeField *pQryTradingCode, int nRequestID) = 0;

	///�����ѯͶ���߷���
	virtual int ReqQryInvestorGroup(CThostFtdcQryInvestorGroupField *pQryInvestorGroup, int nRequestID) = 0;

	///�����ѯ��Լ��֤����
	virtual int ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate, int nRequestID) = 0;

	///�����ѯ��Լ��������
	virtual int ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate, int nRequestID) = 0;

	///�����ѯ��Լ����Ȩ����
	virtual int ReqQryInstrumentTradingRight(CThostFtdcQryInstrumentTradingRightField *pQryInstrumentTradingRight, int nRequestID) = 0;

	///�����ѯ���͹�˾
	virtual int ReqQryBroker(CThostFtdcQryBrokerField *pQryBroker, int nRequestID) = 0;

	///�����ѯ����Ա
	virtual int ReqQryTrader(CThostFtdcQryTraderField *pQryTrader, int nRequestID) = 0;

	///�����ѯ���͹�˾��Ա����
	virtual int ReqQryPartBroker(CThostFtdcQryPartBrokerField *pQryPartBroker, int nRequestID) = 0;

	///�����ѯ�����û�����Ȩ��
	virtual int ReqQrySuperUserFunction(CThostFtdcQrySuperUserFunctionField *pQrySuperUserFunction, int nRequestID) = 0;

	///�����ѯ�û��Ự
	virtual int ReqQryUserSession(CThostFtdcQryUserSessionField *pQryUserSession, int nRequestID) = 0;

	///�����ѯǰ��״̬
	virtual int ReqQryFrontStatus(CThostFtdcQryFrontStatusField *pQryFrontStatus, int nRequestID) = 0;

	///�����ѯ����������
	virtual int ReqQryExchangeOrder(CThostFtdcQryExchangeOrderField *pQryExchangeOrder, int nRequestID) = 0;

	///�����ѯ��������
	virtual int ReqQryOrderAction(CThostFtdcQryOrderActionField *pQryOrderAction, int nRequestID) = 0;

	///�����ѯ��������������
	virtual int ReqQryExchangeOrderAction(CThostFtdcQryExchangeOrderActionField *pQryExchangeOrderAction, int nRequestID) = 0;

	///�����ѯ�����û�
	virtual int ReqQrySuperUser(CThostFtdcQrySuperUserField *pQrySuperUser, int nRequestID) = 0;

	///�����ѯ������
	virtual int ReqQryExchange(CThostFtdcQryExchangeField *pQryExchange, int nRequestID) = 0;

	///�����ѯ��Ʒ
	virtual int ReqQryProduct(CThostFtdcQryProductField *pQryProduct, int nRequestID) = 0;

	///�����ѯ��Լ
	virtual int ReqQryInstrument(CThostFtdcQryInstrumentField *pQryInstrument, int nRequestID) = 0;

	///�����ѯ����
	virtual int ReqQryDepthMarketData(CThostFtdcQryDepthMarketDataField *pQryDepthMarketData, int nRequestID) = 0;

	///�����ѯ���͹�˾�û�
	virtual int ReqQryBrokerUser(CThostFtdcQryBrokerUserField *pQryBrokerUser, int nRequestID) = 0;

	///�����ѯ���͹�˾�û�Ȩ��
	virtual int ReqQryBrokerUserFunction(CThostFtdcQryBrokerUserFunctionField *pQryBrokerUserFunction, int nRequestID) = 0;

	///�����ѯ����Ա���̻�
	virtual int ReqQryTraderOffer(CThostFtdcQryTraderOfferField *pQryTraderOffer, int nRequestID) = 0;

	///�����ѯ�������ˮ
	virtual int ReqQrySyncDeposit(CThostFtdcQrySyncDepositField *pQrySyncDeposit, int nRequestID) = 0;

	///�����ѯͶ���߽�����
	virtual int ReqQrySettlementInfo(CThostFtdcQrySettlementInfoField *pQrySettlementInfo, int nRequestID) = 0;

	///�����ѯ��Լ״̬
	virtual int ReqQryInstrumentStatus(CThostFtdcQryInstrumentStatusField *pQryInstrumentStatus, int nRequestID) = 0;

	///�����ѯǿƽ�����Ͷ���ֲ߳�
	virtual int ReqQryForceClosePosition(CThostFtdcQryInvestorPositionField *pQryInvestorPosition, int nRequestID) = 0;

	///�����ѯת������
	virtual int ReqQryTransferBank(CThostFtdcQryTransferBankField *pQryTransferBank, int nRequestID) = 0;

	///�����ѯͶ���ֲ߳���ϸ
	virtual int ReqQryInvestorPositionDetail(CThostFtdcQryInvestorPositionDetailField *pQryInvestorPositionDetail, int nRequestID) = 0;

	///�����ѯ���鱨�̻�
	virtual int ReqQryMDTraderOffer(CThostFtdcQryMDTraderOfferField *pQryMDTraderOffer, int nRequestID) = 0;

	///�����ѯ�ͻ�֪ͨ
	virtual int ReqQryNotice(CThostFtdcQryNoticeField *pQryNotice, int nRequestID) = 0;

	///�����ѯ������Ϣȷ��
	virtual int ReqQrySettlementInfoConfirm(CThostFtdcQrySettlementInfoConfirmField *pQrySettlementInfoConfirm, int nRequestID) = 0;

	///�����ѯ��Ϻ�Լ����
	virtual int ReqQryCombinationLeg(CThostFtdcQryCombinationLegField *pQryCombinationLeg, int nRequestID) = 0;

	///�����ѯ����ͬ��״̬
	virtual int ReqQrySyncStatus(CThostFtdcQrySyncStatusField *pQrySyncStatus, int nRequestID) = 0;

	///�����ѯ��ϵ��
	virtual int ReqQryLinkMan(CThostFtdcQryLinkManField *pQryLinkMan, int nRequestID) = 0;

	///�����ѯ��ϵ��
	virtual int ReqQryBrokerUserEvent(CThostFtdcQryBrokerUserEventField *pQryBrokerUserEvent, int nRequestID) = 0;

	///�����ѯͶ���ֲ߳���ϸ
	virtual int ReqQryInvestorPositionCombineDetail(CThostFtdcQryInvestorPositionCombineDetailField *pQryInvestorPositionCombineDetail, int nRequestID) = 0;

	///�����ѯ���󱨵�
	virtual int ReqQryErrOrder(CThostFtdcQryErrOrderField *pQryErrOrder, int nRequestID) = 0;

	///�����ѯ���󱨵�����
	virtual int ReqQryErrOrderAction(CThostFtdcQryErrOrderActionField *pQryErrOrderAction, int nRequestID) = 0;

	///�����ѯ������״̬
	virtual int ReqQryExchangeSequence(CThostFtdcQryExchangeSequenceField *pQryExchangeSequence, int nRequestID) = 0;

	///�����ѯ��֤����ϵͳ���͹�˾��Կ
	virtual int ReqQryCFMMCBrokerKey(CThostFtdcQryCFMMCBrokerKeyField *pQryCFMMCBrokerKey, int nRequestID) = 0;

	///�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
	virtual int ReqQryCFMMCTradingAccountKey(CThostFtdcQryCFMMCTradingAccountKeyField *pQryCFMMCTradingAccountKey, int nRequestID) = 0;

	///�����ѯͶ������������ģ��
	virtual int ReqQryCommRateModel(CThostFtdcQryCommRateModelField *pQryCommRateModel, int nRequestID) = 0;

	///�����ѯ�ֵ��۵���Ϣ
	virtual int ReqQryEWarrantOffset(CThostFtdcQryEWarrantOffsetField *pQryEWarrantOffset, int nRequestID) = 0;

	///�����ѯ��ʷ����
	virtual int ReqHisQryOrder(CThostFtdcQryHisOrderField *pQryHisOrder, int nRequestID) = 0;

	///�����ѯת����ˮ
	virtual int ReqQryTransferSerial(CThostFtdcQryTransferSerialField *pQryTransferSerial, int nRequestID) = 0;

	///�����ѯ����ǩԼ��ϵ
	virtual int ReqQryAccountregister(CThostFtdcQryAccountregisterField *pQryAccountregister, int nRequestID) = 0;

	///���ݽ��������붩����������
	virtual int ReqSubMarketDataByExchange(CThostFtdcExchangeField *pExchange, int nRequestID) = 0;

	///���ݽ���������ȡ��������������
	virtual int ReqUnSubMarketDataByExchange(CThostFtdcExchangeField *pExchange, int nRequestID) = 0;

	///�����ѯǩԼ����
	virtual int ReqQryContractBank(CThostFtdcQryContractBankField *pQryContractBank, int nRequestID) = 0;

	///�����ѯԤ��
	virtual int ReqQryParkedOrder(CThostFtdcQryParkedOrderField *pQryParkedOrder, int nRequestID) = 0;

	///�����ѯԤ�񳷵�
	virtual int ReqQryParkedOrderAction(CThostFtdcQryParkedOrderActionField *pQryParkedOrderAction, int nRequestID) = 0;

	///�����ѯ����֪ͨ
	virtual int ReqQryTradingNotice(CThostFtdcQryTradingNoticeField *pQryTradingNotice, int nRequestID) = 0;

	///�����ѯ���͹�˾���ײ���
	virtual int ReqQryBrokerTradingParams(CThostFtdcQryBrokerTradingParamsField *pQryBrokerTradingParams, int nRequestID) = 0;

	///�����ѯ���͹�˾�����㷨
	virtual int ReqQryBrokerTradingAlgos(CThostFtdcQryBrokerTradingAlgosField *pQryBrokerTradingAlgos, int nRequestID) = 0;

	///�����ѯ���͹�˾�ʽ�
	virtual int ReqQueryBrokerDeposit(CThostFtdcQueryBrokerDepositField *pQueryBrokerDeposit, int nRequestID) = 0;

	///���������û��µ�Ȩ�޷���
	virtual int ReqInsUserRightsAssign(CThostFtdcUserRightsAssignField *pUserRightsAssign, int nRequestID) = 0;

	///�����޸��û��µ�Ȩ�޷���
	virtual int ReqUpdUserRightsAssign(CThostFtdcUserRightsAssignField *pUserRightsAssign, int nRequestID) = 0;

	///����ɾ���û��µ�Ȩ�޷���
	virtual int ReqDelUserRightsAssign(CThostFtdcUserRightsAssignField *pUserRightsAssign, int nRequestID) = 0;

	///�������Ӿ��ù�˾�µ�Ȩ�޷���
	virtual int ReqInsBrokerUserRightAssign(CThostFtdcBrokerUserRightAssignField *pBrokerUserRightAssign, int nRequestID) = 0;

	///�����޸ľ��ù�˾�µ�Ȩ�޷���
	virtual int ReqUpdBrokerUserRightAssign(CThostFtdcBrokerUserRightAssignField *pBrokerUserRightAssign, int nRequestID) = 0;

	///����ɾ�����ù�˾�µ�Ȩ�޷���
	virtual int ReqDelBrokerUserRightAssign(CThostFtdcBrokerUserRightAssignField *pBrokerUserRightAssign, int nRequestID) = 0;

	///���з���ת�ʿ�������
	virtual int ReqOpenAccountByBank(CThostFtdcReqOpenAccountField *pReqOpenAccount, int nRequestID) = 0;

	///���з���ת����������
	virtual int ReqCancelAccountByBank(CThostFtdcReqOpenAccountField *pReqOpenAccount, int nRequestID) = 0;

	///���з����������˻�����
	virtual int ReqChangeAccountByBank(CThostFtdcReqChangeAccountField *pReqChangeAccount, int nRequestID) = 0;

	///���з��������ʽ�ת�ڻ�����
	virtual int ReqFromBankToFutureByBank(CThostFtdcReqTransferField *pReqTransfer, int nRequestID) = 0;

	///���з����ڻ��ʽ�ת��������
	virtual int ReqFromFutureToBankByBank(CThostFtdcReqTransferField *pReqTransfer, int nRequestID) = 0;

	///�ڻ����������ʽ�ת�ڻ�����
	virtual int ReqFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID) = 0;

	///�ڻ������ڻ��ʽ�ת��������
	virtual int ReqFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID) = 0;

	///��������ת�ڻ�����
	virtual int ReqRepealFromBankToFuture(CThostFtdcReqRepealField *pReqRepeal, int nRequestID) = 0;

	///�����ڻ�ת��������
	virtual int ReqRepealFromFutureToBank(CThostFtdcReqRepealField *pReqRepeal, int nRequestID) = 0;

	///ϵͳ����ʱ�ڻ����ֹ������������ת�ڻ�����
	virtual int ReqRepealFromBankToFutureByFutureManual(CThostFtdcReqRepealField *pReqRepeal, int nRequestID) = 0;

	///ϵͳ����ʱ�ڻ����ֹ���������ڻ�ת��������
	virtual int ReqRepealFromFutureToBankByFutureManual(CThostFtdcReqRepealField *pReqRepeal, int nRequestID) = 0;

	///�ڻ������ѯ�����������
	virtual int ReqQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, int nRequestID) = 0;

	///���з����ѯ�˻���Ӧ��ϵ����
	virtual int ReqQueryAccountRelationByBank(CThostFtdcReqQueryAccountField *pReqQueryAccount, int nRequestID) = 0;

	///�ڻ�������˿ͻ���ѯ����ֱͨ����ͨ�������
	virtual int ReqQueryFBTOpenAccountStatusByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, int nRequestID) = 0;

	///���з�����֤�ڻ�Ͷ���˺����벢��ѯ�˻���Ϣ����
	virtual int ReqVerifyFutureAccPWAndQueryInfoByBank(CThostFtdcReqQueryAccountField *pReqQueryAccount, int nRequestID) = 0;

	///����ǩ������
	virtual int ReqFutureSignIn(CThostFtdcFutureSignIOField *pFutureSignIO, int nRequestID) = 0;

	///����ǩ������
	virtual int ReqFutureSignOut(CThostFtdcFutureSignIOField *pFutureSignIO, int nRequestID) = 0;

	///���з��������ļ�׼����������
	virtual int ReqDayEndFileReadyByBank(CThostFtdcReqDayEndFileReadyField *pReqDayEndFileReady, int nRequestID) = 0;

	///���ڱ������׺��ķ�����֤�ڻ��ʽ���������
	virtual int ReqVerifyFuturePassword(CThostFtdcVerifyFuturePasswordField *pVerifyFuturePassword, int nRequestID) = 0;

	///���ڱ������׺��ķ�����֤�ͻ���Ϣ����
	virtual int ReqVerifyCustInfo(CThostFtdcVerifyCustInfoField *pVerifyCustInfo, int nRequestID) = 0;

	///���ڱ������׺��ķ�����֤�ڻ��ʽ�����Ϳͻ���Ϣ����
	virtual int ReqVerifyFuturePasswordAndCustInfo(CThostFtdcVerifyFuturePasswordAndCustInfoField *pVerifyFuturePasswordAndCustInfo, int nRequestID) = 0;

	///���׺��������ڱ��̷�����Կͬ������
	virtual int ReqSyncKey(CThostFtdcReqSyncKeyField *pReqSyncKey, int nRequestID) = 0;
protected:
	~CThostFtdcUserApi(){};
};

#endif
