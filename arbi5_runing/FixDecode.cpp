// FixDecode.cpp: implementation of the FixDealer class.
//
//////////////////////////////////////////////////////////////////////

#include "FixDecode.h"
#include "LogStore.h"
#include <process.h>
#include <assert.h>


static char  g_message[526] ;		        // “≈¡Ùœ˚œ¢
LISTFIXMSG g_listret;
FixDecode::FixDecode()
{
}

FixDecode::~FixDecode()
{
}

LISTFIXMSGALL FixDecode::fixDecodeMsg(string sbody)
{
	LISTFIXMSGALL ret;
	LISTFIXMSG listret;
	strings pars;	
	strings::iterator it;		
	
	int npar = 0;
	int code = 0;
	LISTFIXMSG listpart;
	char szpart[2] = {0x01,0x00};
	FixPubTools::divide_string(sbody.c_str() ,pars,szpart,TRUE);
	for(npar = 0,it = pars.begin(); it != pars.end();it++,npar++)
	{
		TFIXMSG  tFixMsg = fix_decode_tag_value((*it));
		if (npar == 0)
		{
			if (atoi(tFixMsg.msg_tag.c_str()) != 8)
				listret = g_listret;
			else
				g_listret.clear();
		}
		listret.push_back(tFixMsg);
		if (atoi(tFixMsg.msg_tag.c_str()) == 10)
		{
			ret.push_back(listret);
			listret.clear();
		}
		g_listret = listret;
	}
	
	return ret;
	
}

TFIXMSG FixDecode::fix_decode_tag_value(string sparts)
{
	TFIXMSG tret;
	strings pars;	
	strings::iterator it;		
	int npar = 0;
	FixPubTools::divide_string(sparts.c_str() ,pars,"=",TRUE);
	for(npar = 0,it = pars.begin(); it != pars.end();it++,npar++)
	{
		switch(npar) {
		case 0:
			tret.msg_tag = (*it);
			break;
		case 1:
			tret.msg_value = (*it);
			break;
		default:
			break;
		}
	}
	return tret;
	
}

string FixDecode::fixDecodeMsg2Txt(string sbody)
{
	string ret;
	strings pars;
	string sinfo;
	strings::iterator it;		
	
	TFIXMSG tFM;
	int npar = 0;
	int code = 0;
	LISTFIXMSG listpart;
	char szpart[2] = {0x01,0x00};
	char szline[526];
	memset(szline, 0, sizeof(szline));
	FixPubTools::divide_string(sbody.c_str() ,pars,szpart,TRUE);
	for(npar = 0,it = pars.begin(); it != pars.end();it++,npar++)
	{
		tFM   = fix_decode_tag_value((*it));
		sinfo = fix_get_taginfo(tFM.msg_tag);
		memset(szline, 0, sizeof(szline));
		sprintf(szline, "%-5s :  %-20s :  %s\r\n",tFM.msg_tag.c_str(), tFM.msg_value.c_str(),sinfo.c_str());
		ret += szline;
	}
	return ret;
}

string FixDecode::fix_get_taginfo(string stag)
{
	string ret;
	int ntag = atoi(stag.c_str());
	switch(ntag) {
	case 1563 : ret = " RelatedPartyID" ; break;
	case 1090 : ret = " MaxPriceLevels" ; break;
	case 1278 : ret = " DerivativeEncodedIssuer" ; break;
	case 936  : ret = " NoCompIDs" ; break;
	case 214  : ret = " SettlInstRefID" ; break;
	case 524  : ret = " NestedPartyID" ; break;
	case 1458 : ret = " DetachmentPoint" ; break;
	case 978  : ret = " LateIndicator" ; break;
	case 1621 : ret = " RiskEncodedSecurityDesc" ; break;
	case 1601 : ret = " RelationshipRiskSecuritySubType" ; break;
	case 1465 : ret = " SecurityListID" ; break;
	case 1442 : ret = " DerivativeFlowScheduleType" ; break;
	case 1359 : ret = " EncodedSymbolLen" ; break;
	case 1244 : ret = " FlexibleIndicator" ; break;
	case 1232 : ret = " NoExecInstRules" ; break;
	case 1012 : ret = " SideTrdRegTimestamp" ; break;
	case 668  : ret = " DeliveryForm" ; break;
	case 378  : ret = " ExecRestatementReason" ; break;
	case 633  : ret = " MidYield" ; break;
	case 231  : ret = " ContractMultiplier" ; break;
	case 1518 : ret = " PartyAltIDSource" ; break;
	case 1157 : ret = " CcyAmt" ; break;
	case 808  : ret = " AllocIntermedReqType" ; break;
	case 756  : ret = " NoNested2PartyIDs" ; break;
	case 306  : ret = " UnderlyingIssuer" ; break;
	case 685  : ret = " LegOrderQty" ; break;
	case 562  : ret = " MinTradeVol" ; break;
	case 119  : ret = " SettlCurrAmt" ; break;
	case 1295 : ret = " DerivativeInstrumentPartyRole" ; break;
	case 698  : ret = " YieldRedemptionPriceType" ; break;
	case 1476 : ret = " NewsRefID" ; break;
	case 1471 : ret = " SecurityListTypeSource" ; break;
	case 1346 : ret = " ApplReqID" ; break;
	case 1319 : ret = " DerivativeFuturesValuationMethod" ; break;
	case 604  : ret = " NoLegSecurityAltID" ; break;
	case 1249 : ret = " DerivativeSecurityType" ; break;
	case 896  : ret = " CollInquiryQualifier" ; break;
	case 96   : ret = " RawData" ; break;
	case 187  : ret = " CashSettlAgentContactPhone" ; break;
	case 255  : ret = " CreditRating" ; break;
	case 1385 : ret = " ContingencyType" ; break;
	case 947  : ret = " StrikeCurrency" ; break;
	case 1020 : ret = " TradeVolume" ; break;
	case 1014 : ret = " SideTrdRegTimestampSrc" ; break;
	case 743  : ret = " DeliveryDate" ; break;
	case 94   : ret = " EmailType" ; break;
	case 353  : ret = " EncodedListExecInst" ; break;
	case 438  : ret = " ContraTradeTime" ; break;
	case 1229 : ret = " MaturityMonthYearIncrement" ; break;
		
	case 1118 : ret = " RootPartyIDSource" ; break;
		
	case 241 : ret = " UnderlyingCouponPaymentDate" ; break;
		
	case 632 : ret = " BidYield" ; break;
		
	case 25 : ret = " IOIQltyInd" ; break;
		
	case 106 : ret = " Issuer" ; break;
		
	case 489 : ret = " CardNumber" ; break;
		
	case 1562 : ret = " NoRelatedPartyIDs" ; break;
		
	case 683 : ret = " NoLegStipulations" ; break;
		
	case 616 : ret = " LegSecurityExchange" ; break;
		
	case 152 : ret = " CashOrderQty" ; break;
		
	case 159 : ret = " AccruedInterestAmt" ; break;
		
	case 289 : ret = " MDEntrySeller" ; break;
		
	case 566 : ret = " LegPrice" ; break;
		
	case 1603 : ret = " RelationshipRiskMaturityTime" ; break;
		
	case 128 : ret = " DeliverToCompID" ; break;
		
	case 143 : ret = " TargetLocationID" ; break;
		
	case 643 : ret = " OfferForwardPoints2" ; break;
		
	case 319 : ret = " RatioQty" ; break;
		
	case 563 : ret = " MultiLegRptTypeReq" ; break;
		
	case 79 : ret = " AllocAccount" ; break;
		
	case 387 : ret = " TotalVolumeTraded" ; break;
		
	case 33 : ret = " LinesOfText" ; break;
		
	case 581 : ret = " AccountType" ; break;
		
	case 290 : ret = " MDEntryPositionNo" ; break;
		
	case 327 : ret = " HaltReasonInt" ; break;
		
	case 64 : ret = " FutSettDate" ; break;
		
	case 107 : ret = " SecurityDesc" ; break;
		
	case 110 : ret = " MinQty" ; break;
		
	case 120 : ret = " SettlCurrency" ; break;
		
	case 211 : ret = " PegOffsetValue" ; break;
		
	case 1220 : ret = " DerivativeSecurityAltIDSource" ; break;
		
	case 801 : ret = " NoSettlPartySubIDs" ; break;
		
	case 755 : ret = " AllocReportID" ; break;
		
	case 608 : ret = " LegCFICode" ; break;
		
	case 588 : ret = " LegFutSettDate" ; break;
		
	case 677 : ret = " LegBenchmarkCurveName" ; break;
		
	case 635 : ret = " ClearingFeeIndicator" ; break;
		
	case 92 : ret = " BrokerOfCredit" ; break;
		
	case 1466 : ret = " SecurityListRefID" ; break;
		
	case 1405 : ret = " UnderlyingLegMaturityTime" ; break;
		
	case 805 : ret = " NestedPartySubIDType" ; break;
		
	case 394 : ret = " BidType" ; break;
		
	case 280 : ret = " MDEntryRefID" ; break;
		
	case 1423 : ret = " UnderlyingUnitOfMeasureQty" ; break;
		
	case 1345 : ret = " UnderlyingLegMaturityDate" ; break;
		
	case 1206 : ret = " StartTickPriceRange" ; break;
		
	case 955 : ret = " LegContractSettlMonth" ; break;
		
	case 307 : ret = " UnderlyingSecurityDesc" ; break;
		
	case 501 : ret = " CashDistribPayRef" ; break;
		
	case 692 : ret = " QuotePriceType" ; break;
		
	case 361 : ret = " EncodedAllocText" ; break;
		
	case 313 : ret = " UnderlyingMaturityMonthYear" ; break;
		
	case 1560 : ret = " RiskWarningLevelPercent" ; break;
		
	case 1456 : ret = " UnderlyingOriginalNotionalPercentageOutstanding" ; break;
		
	case 1378 : ret = " MultilegPriceMethod" ; break;
		
	case 1361 : ret = " TotNoFills" ; break;
		
	case 1254 : ret = " DerivativeSettleOnOpenFlag" ; break;
		
	case 244 : ret = " UnderlyingRepurchaseTerm" ; break;
		
	case 1561 : ret = " RiskWarningLevelName" ; break;
		
	case 1258 : ret = " DerivativeCountryOfIssue" ; break;
		
	case 1198 : ret = " ListMethod" ; break;
		
	case 877 : ret = " UnderlyingCPProgram" ; break;
		
	case 811 : ret = " PriceDelta" ; break;
		
	case 45 : ret = " RefSeqNum" ; break;
		
	case 754 : ret = " AutoAcceptIndicator" ; break;
		
	case 547 : ret = " MDImplicitDelete" ; break;
		
	case 232 : ret = " NoStipulations" ; break;
		
	case 715 : ret = " ClearingBusinessDate" ; break;
		
	case 1582 : ret = " NoRelationshipRiskLimits" ; break;
		
	case 283 : ret = " LocationID" ; break;
		
	case 15 : ret = " Currency" ; break;
		
	case 216 : ret = " RoutingType" ; break;
		
	case 316 : ret = " UnderlyingStrikePrice" ; break;
		
	case 418 : ret = " BidTradeType" ; break;
		
	case 1588 : ret = " RelationshipRiskInstrumentOperator" ; break;
		
	case 1459 : ret = " UnderlyingAttachmentPoint" ; break;
		
	case 1170 : ret = " TotNoRejQuotes" ; break;
		
	case 790 : ret = " OrdStatusReqID" ; break;
		
	case 49 : ret = " SenderCompID" ; break;
		
	case 103 : ret = " OrdRejReason" ; break;
		
	case 1302 : ret = " MaturityMonthYearIncrementUnits" ; break;
		
	case 1083 : ret = " DisplayWhen" ; break;
		
	case 815 : ret = " ApplQueueAction" ; break;
		
	case 514 : ret = " RegistTransType" ; break;
		
	case 505 : ret = " PaymentRemitterID" ; break;
		
	case 423 : ret = " PriceType" ; break;
		
	case 1393 : ret = " MarketReqID" ; break;
		
	case 1312 : ret = " NoNestedInstrAttrib" ; break;
		
	case 762 : ret = " SecuritySubType" ; break;
		
	case 11 : ret = " ClOrdID" ; break;
		
	case 205 : ret = " MaturityDay" ; break;
		
	case 1454 : ret = " UnderlyingSeniority" ; break;
		
	case 1396 : ret = " MarketSegmentDesc" ; break;
		
	case 1310 : ret = " NoMarketSegments" ; break;
		
	case 1159 : ret = " SettlObligMode" ; break;
		
	case 980 : ret = " SecurityUpdateAction" ; break;
		
	case 935 : ret = " NetworkRequestType" ; break;
		
	case 402 : ret = " LiquidityPctLow" ; break;
		
	case 452 : ret = " PartyRole" ; break;
		
	case 623 : ret = " LegRatioQty" ; break;
		
	case 155 : ret = " SettlCurrFxRate" ; break;
		
	case 1565 : ret = " RelatedPartyRole" ; break;
		
	case 1436 : ret = " LegContractMultiplierUnit" ; break;
		
	case 91 : ret = " SecureData" ; break;
		
	case 142 : ret = " SenderLocationID" ; break;
		
	case 1025 : ret = " FirstPx" ; break;
		
	case 619 : ret = " EncodedLegIssuer" ; break;
		
	case 744 : ret = " AssignmentMethod" ; break;
		
	case 217 : ret = " RoutingID" ; break;
		
	case 1594 : ret = " RelationshipRiskSecurityAltID" ; break;
		
	case 1570 : ret = " RelatedPartyAltID" ; break;
		
	case 959 : ret = " StrategyParameterType" ; break;
		
	case 98 : ret = " EncryptMethod" ; break;
		
	case 593 : ret = " UnderlyingStateOrProvinceOfIssue" ; break;
		
	case 1399 : ret = " ApplNewSeqNum" ; break;
		
	case 1280 : ret = " DerivativeEncodedSecurityDescLen" ; break;
		
	case 1245 : ret = " TradingCurrency" ; break;
		
	case 1230 : ret = " SecondaryHighLimitPrice" ; break;
		
	case 799 : ret = " OrderAvgPx" ; break;
		
	case 707 : ret = " PosAmtType" ; break;
		
	case 141 : ret = " ResetSeqNumFlag" ; break;
		
	case 627 : ret = " NoHops" ; break;
		
	case 946 : ret = " CollInquiryResult" ; break;
		
	case 916 : ret = " StartDate" ; break;
		
	case 905 : ret = " CollAsgnRespType" ; break;
		
	case 800 : ret = " OrderBookingQty" ; break;
		
	case 735 : ret = " NoQuoteQualifiers" ; break;
		
	case 325 : ret = " UnsolicitedIndicator" ; break;
		
	case 1131 : ret = " RefCstmApplVerID" ; break;
		
	case 1427 : ret = " SideExecID" ; break;
		
	case 1328 : ret = " RejectText" ; break;
		
	case 1139 : ret = " ExchangeSpecialInstructions" ; break;
		
	case 1003 : ret = " TradeID" ; break;
		
	case 991 : ret = " RndPx" ; break;
		
	case 368 : ret = " QuoteEntryRejectReason" ; break;
		
	case 528 : ret = " OrderCapacity" ; break;
		
	case 1009 : ret = " SideLastQty" ; break;
		
	case 1269 : ret = " DerivativeUnitOfMeasure" ; break;
		
	case 670 : ret = " NoLegAllocs" ; break;
		
	case 297 : ret = " QuoteAckStatus" ; break;
		
	case 1042 : ret = " SecondaryFirmTradeID" ; break;
		
	case 924 : ret = " UserRequestType" ; break;
		
	case 855 : ret = " SecondaryTrdType" ; break;
		
	case 487 : ret = " TradeReportTransType" ; break;
		
	case 4 : ret = " AdvSide" ; break;
		
	case 1580 : ret = " RelatedContextPartySubID" ; break;
		
	case 1250 : ret = " DerivativeSecuritySubType" ; break;
		
	case 1114 : ret = " TriggerTradingSessionSubID" ; break;
		
	case 820 : ret = " TradeLinkID" ; break;
		
	case 679 : ret = " LegBenchmarkPrice" ; break;
		
	case 630 : ret = " HopRefID" ; break;
		
	case 494 : ret = " Designation" ; break;
		
	case 568 : ret = " TradeRequestID" ; break;
		
	case 1583 : ret = " RelationshipRiskLimitType" ; break;
		
	case 1539 : ret = " RiskSecurityIDSource" ; break;
		
	case 1440 : ret = " LegFlowScheduleType" ; break;
		
	case 1421 : ret = " LegPriceUnitOfMeasure" ; break;
		
	case 1416 : ret = " Nested4PartyIDSource" ; break;
		
	case 203 : ret = " CoveredOrUncovered" ; break;
		
	case 660 : ret = " AcctIDSource" ; break;
		
	case 646 : ret = " MktOfferPx" ; break;
		
	case 862 : ret = " NoCapacities" ; break;
		
	case 569 : ret = " TradeRequestType" ; break;
		
	case 539 : ret = " NoNestedPartyIDs" ; break;
		
	case 340 : ret = " TradSesStatus" ; break;
		
	case 1455 : ret = " UnderlyingNotionalPercentageOutstanding" ; break;
		
	case 1350 : ret = " ApplLastSeqNum" ; break;
		
	case 1094 : ret = " PegPriceType" ; break;
		
	case 958 : ret = " StrategyParameterName" ; break;
		
	case 1502 : ret = " StreamAsgnRejReason" ; break;
		
	case 1089 : ret = " MatchIncrement" ; break;
		
	case 951 : ret = " Nested3PartyRole" ; break;
		
	case 810 : ret = " UnderlyingPx" ; break;
		
	case 639 : ret = " PriceImprovement" ; break;
		
	case 1197 : ret = " ValuationMethod" ; break;
		
	case 1216 : ret = " DerivativeSecurityID" ; break;
		
	case 981 : ret = " NoExpiration" ; break;
		
	case 56 : ret = " TargetCompID" ; break;
		
	case 288 : ret = " MDEntryBuyer" ; break;
		
	case 1608 : ret = " RelationshipRiskCouponRate" ; break;
		
	case 1296 : ret = " NoDerivativeInstrumentPartySubIDs" ; break;
		
	case 1236 : ret = " NoMaturityRules" ; break;
		
	case 1166 : ret = " QuoteMsgID" ; break;
		
	case 1100 : ret = " TriggerType" ; break;
		
	case 1092 : ret = " PriceProtectionScope" ; break;
		
	case 832 : ret = " TotNumAssignmentReports" ; break;
		
	case 655 : ret = " ContraLegRefID" ; break;
		
	case 751 : ret = " TradeReportRejectReason" ; break;
		
	case 572 : ret = " TradeReportRefID" ; break;
		
	case 1470 : ret = " SecurityListType" ; break;
		
	case 1217 : ret = " DerivativeSecurityIDSource" ; break;
		
	case 745 : ret = " AssignmentUnit" ; break;
		
	case 571 : ret = " TradeReportID" ; break;
		
	case 82 : ret = " NoRpts" ; break;
		
	case 680 : ret = " LegBenchmarkPriceType" ; break;
		
	case 356 : ret = " EncodedSubjectLen" ; break;
		
	case 1185 : ret = " SecurityXML" ; break;
		
	case 990 : ret = " LegReportID" ; break;
		
	case 954 : ret = " Nested3PartySubIDType" ; break;
		
	case 761 : ret = " BenchmarkSecurityIDSource" ; break;
		
	case 300 : ret = " QuoteRejectReason" ; break;
		
	case 108 : ret = " HeartBtInt" ; break;
		
	case 189 : ret = " BidForwardPoints" ; break;
		
	case 97 : ret = " PossResend" ; break;
		
	case 55 : ret = " Symbol" ; break;
		
	case 365 : ret = " EncodedUnderlyingSecurityDesc" ; break;
		
	case 1574 : ret = " RelatedPartyAltSubIDType" ; break;
		
	case 1394 : ret = " MarketReportID" ; break;
		
	case 845 : ret = " DiscretionPrice" ; break;
		
	case 520 : ret = " ContAmtValue" ; break;
		
	case 465 : ret = " QuantityType" ; break;
		
	case 1487 : ret = " ComplexEventPriceBoundaryMethod" ; break;
		
	case 1144 : ret = " ImpliedMarketIndicator" ; break;
		
	case 1136 : ret = " AllocClearingFeeIndicator" ; break;
		
	case 303 : ret = " QuoteRequestType" ; break;
		
	case 560 : ret = " SecurityRequestResult" ; break;
		
	case 529 : ret = " OrderRestrictions" ; break;
		
	case 1016 : ret = " NoSideTrdRegTS" ; break;
		
	case 58 : ret = " Text" ; break;
		
	case 354 : ret = " EncodedTextLen" ; break;
		
	case 433 : ret = " ListExecInstType" ; break;
		
	case 198 : ret = " SecondaryOrderID" ; break;
		
	case 76 : ret = " ExecBroker" ; break;
		
	case 1618 : ret = " RelationshipRiskEncodedSecurityDescLen" ; break;
		
	case 1184 : ret = " SecurityXMLLen" ; break;
		
	case 1181 : ret = " ApplSeqNum" ; break;
		
	case 1140 : ret = " MaxTradeVol" ; break;
		
	case 1066 : ret = " OfferSwapPoints" ; break;
		
	case 786 : ret = " SettlPartySubIDType" ; break;
		
	case 477 : ret = " DistribPaymentMethod" ; break;
		
	case 533 : ret = " TotalAffectedOrders" ; break;
		
	case 1204 : ret = " StrikeIncrement" ; break;
		
	case 1032 : ret = " OrderHandlingInstSource" ; break;
		
	case 797 : ret = " CopyMsgIndicator" ; break;
		
	case 85 : ret = " NoDlvyInst" ; break;
		
	case 299 : ret = " QuoteEntryID" ; break;
		
	case 940 : ret = " AffirmStatus" ; break;
		
	case 482 : ret = " MailingInst" ; break;
		
	case 135 : ret = " OfferSize" ; break;
		
	case 609 : ret = " LegSecurityType" ; break;
		
	case 1533 : ret = " RiskLimitPlatform" ; break;
		
	case 1432 : ret = " OrigCustOrderCapacity" ; break;
		
	case 1002 : ret = " AllocMethod" ; break;
		
	case 976 : ret = " QuantityDate" ; break;
		
	case 850 : ret = " TargetStrategyPerformance" ; break;
		
	case 490 : ret = " CardExpDate" ; break;
		
	case 664 : ret = " ConfirmID" ; break;
		
	case 170 : ret = " StandInstDbName" ; break;
		
	case 424 : ret = " DayOrderQty" ; break;
		
	case 1149 : ret = " HighLimitPrice" ; break;
		
	case 1041 : ret = " FirmTradeID" ; break;
		
	case 989 : ret = " SecondaryIndividualAllocID" ; break;
		
	case 913 : ret = " AgreementDesc" ; break;
		
	case 531 : ret = " MassCancelResponse" ; break;
		
	case 675 : ret = " LegSettlCurrency" ; break;
		
	case 12 : ret = " Commission" ; break;
		
	case 1498 : ret = " StreamAsgnReqType" ; break;
		
	case 1065 : ret = " BidSwapPoints" ; break;
		
	case 781 : ret = " NoSettlPartyIDs" ; break;
		
	case 65 : ret = " SymbolSfx" ; break;
		
	case 379 : ret = " BusinessRejectRefID" ; break;
		
	case 640 : ret = " Price2" ; break;
		
	case 1443 : ret = " FillLiquidityInd" ; break;
		
	case 1369 : ret = " MassActionReportID" ; break;
		
	case 1275 : ret = " DerivativeIssuer" ; break;
		
	case 1133 : ret = " ExDestinationIDSource" ; break;
		
	case 904 : ret = " CollRespID" ; break;
		
	case 559 : ret = " SecurityListRequestType" ; break;
		
	case 622 : ret = " EncodedLegSecurityDesc" ; break;
		
	case 1578 : ret = " RelatedContextPartyRole" ; break;
		
	case 988 : ret = " UnderlyingSettlementStatus" ; break;
		
	case 455 : ret = " SecurityAltID" ; break;
		
	case 508 : ret = " RegistRefID" ; break;
		
	case 1607 : ret = " RelationshipRiskFlexibleIndicator" ; break;
		
	case 1318 : ret = " DerivativePriceQuoteMethod" ; break;
		
	case 1597 : ret = " RelationshipRiskProductComplex" ; break;
		
	case 1428 : ret = " OrderDelay" ; break;
		
	case 1370 : ret = " NoNotAffectedOrders" ; break;
		
	case 949 : ret = " Nested3PartyID" ; break;
		
	case 895 : ret = " CollAsgnReason" ; break;
		
	case 450 : ret = " TotalVolumeTradedTime" ; break;
		
	case 207 : ret = " SecurityExchange" ; break;
		
	case 731 : ret = " SettlPriceType" ; break;
		
	case 1147 : ret = " UnitOfMeasureQty" ; break;
		
	case 923 : ret = " UserRequestID" ; break;
		
	case 669 : ret = " LastParPx" ; break;
		
	case 1226 : ret = " EndMaturityMonthYear" ; break;
		
	case 1048 : ret = " DealingCapacity" ; break;
		
	case 140 : ret = " PrevClosePx" ; break;
		
	case 579 : ret = " TradeInputDevice" ; break;
		
	case 425 : ret = " DayCumQty" ; break;
		
	case 178 : ret = " SecuritySettlAgentAcctNum" ; break;
		
	case 556 : ret = " LegCurrency" ; break;
		
	case 1404 : ret = " EncryptedNewPassword" ; break;
		
	case 1268 : ret = " DerivativeMinPriceIncrementAmount" ; break;
		
	case 952 : ret = " NoNested3PartySubIDs" ; break;
		
	case 931 : ret = " RefSubID" ; break;
		
	case 784 : ret = " SettlPartyRole" ; break;
		
	case 498 : ret = " CashDistribAgentName" ; break;
		
	case 614 : ret = " LegContractMultiplier" ; break;
		
	case 415 : ret = " ProgPeriodInterval" ; break;
		
	case 587 : ret = " LegSettlType" ; break;
		
	case 144 : ret = " OnBehalfOfLocationID" ; break;
		
	case 116 : ret = " OnBehalfOfSubID" ; break;
		
	case 1586 : ret = " RelationshipRiskLimitPlatform" ; break;
		
	case 1567 : ret = " RelatedPartySubID" ; break;
		
	case 1496 : ret = " ComplexEventEndTime" ; break;
		
	case 1447 : ret = " RateSourceType" ; break;
		
	case 1259 : ret = " DerivativeStateOrProvinceOfIssue" ; break;
		
	case 824 : ret = " TradeLegRefID" ; break;
		
	case 1504 : ret = " RelSymTransactTime" ; break;
		
	case 1494 : ret = " NoComplexEventTimes" ; break;
		
	case 1074 : ret = " LegCalculatedCcyLastQty" ; break;
		
	case 950 : ret = " Nested3PartyIDSource" ; break;
		
	case 873 : ret = " DatedDate" ; break;
		
	case 162 : ret = " SettlInstID" ; break;
		
	case 746 : ret = " OpenInterest" ; break;
		
	case 436 : ret = " UnderlyingContractMultiplier" ; break;
		
	case 304 : ret = " TotQuoteEntries" ; break;
		
	case 1520 : ret = " PartyAltSubID" ; break;
		
	case 1168 : ret = " TotNoCxldQuotes" ; break;
		
	case 266 : ret = " AggregatedBook" ; break;
		
	case 476 : ret = " PaymentRef" ; break;
		
	case 504 : ret = " PaymentDate" ; break;
		
	case 516 : ret = " OrderPercent" ; break;
		
	case 706 : ret = " PosQtyStatus" ; break;
		
	case 1551 : ret = " RiskRestructuringType" ; break;
		
	case 1413 : ret = " NoNested4PartySubIDs" ; break;
		
	case 1171 : ret = " PrivateQuote" ; break;
		
	case 1040 : ret = " SecondaryTradeID" ; break;
		
	case 181 : ret = " SecuritySettlAgentContactPhone" ; break;
		
	case 1397 : ret = " EncodedMktSegmDescLen" ; break;
		
	case 1154 : ret = " SideCurrency" ; break;
		
	case 687 : ret = " LegQty" ; break;
		
	case 35 : ret = " MsgType" ; break;
		
	case 386 : ret = " NoTradingSessions" ; break;
		
	case 23 : ret = " IOIid" ; break;
		
	case 442 : ret = " MultiLegReportingType" ; break;
		
	case 22 : ret = " IDSource" ; break;
		
	case 688 : ret = " LegStipulationType" ; break;
		
	case 1438 : ret = " DerivativeContractMultiplierUnit" ; break;
		
	case 1300 : ret = " MarketSegmentID" ; break;
		
	case 39 : ret = " OrdStatus" ; break;
		
	case 541 : ret = " MaturityDate" ; break;
		
	case 1349 : ret = " ApplTotalMessageCount" ; break;
		
	case 1053 : ret = " InstrumentPartySubID" ; break;
		
	case 204 : ret = " CustomerOrFirm" ; break;
		
	case 718 : ret = " AdjustmentType" ; break;
		
	case 1519 : ret = " NoPartyAltSubIDs" ; break;
		
	case 1059 : ret = " UnderlyingInstrumentPartyID" ; break;
		
	case 1015 : ret = " AsOfIndicator" ; break;
		
	case 649 : ret = " QuoteStatusReqID" ; break;
		
	case 564 : ret = " LegPositionEffect" ; break;
		
	case 270 : ret = " MDEntryPx" ; break;
		
	case 1374 : ret = " MassActionScope" ; break;
		
	case 1134 : ret = " ReportedPxDiff" ; break;
		
	case 987 : ret = " UnderlyingSettlementDate" ; break;
		
	case 804 : ret = " NoNestedPartySubIDs" ; break;
		
	case 795 : ret = " AllocReportRefID" ; break;
		
	case 238 : ret = " Concession" ; break;
		
	case 351 : ret = " EncodedSecurityDesc" ; break;
		
	case 19 : ret = " ExecRefID" ; break;
		
	case 1430 : ret = " VenueType" ; break;
		
	case 1373 : ret = " MassActionType" ; break;
		
	case 723 : ret = " PosMaintResult" ; break;
		
	case 27 : ret = " IOIShares" ; break;
		
	case 699 : ret = " BenchmarkSecurityID" ; break;
		
	case 1418 : ret = " LegLastQty" ; break;
		
	case 736 : ret = " AllocSettlCurrency" ; break;
		
	case 596 : ret = " LegCountryOfIssue" ; break;
		
	case 1283 : ret = " DerivativeSecurityXML" ; break;
		
	case 1223 : ret = " StrikeRuleID" ; break;
		
	case 930 : ret = " RefCompID" ; break;
		
	case 657 : ret = " SettlCurrOfferFxRate" ; break;
		
	case 634 : ret = " OfferYield" ; break;
		
	case 1576 : ret = " RelatedContextPartyID" ; break;
		
	case 1463 : ret = " TargetPartyIDSource" ; break;
		
	case 1403 : ret = " EncryptedNewPasswordLen" ; break;
		
	case 702 : ret = " NoPositions" ; break;
		
	case 540 : ret = " TotalAccruedInterestAmt" ; break;
		
	case 317 : ret = " UnderlyingOptAttribute" ; break;
		
	case 1314 : ret = " DerivativeInstrAttribValue" ; break;
		
	case 1050 : ret = " InstrumentPartyIDSource" ; break;
		
	case 836 : ret = " PegOffsetType" ; break;
		
	case 532 : ret = " MassCancelRejectReason" ; break;
		
	case 725 : ret = " ResponseTransportType" ; break;
		
	case 603 : ret = " LegSecurityIDSource" ; break;
		
	case 260 : ret = " BasisFeaturePrice" ; break;
		
	case 224 : ret = " CouponPaymentDate" ; break;
		
	case 567 : ret = " TradSesStatusRejReason" ; break;
		
	case 1460 : ret = " UnderlyingDetachmentPoint" ; break;
		
	case 1222 : ret = " MaturityRuleID" ; break;
		
	case 243 : ret = " UnderlyingRepoCollateralSecurityType" ; break;
		
	case 1239 : ret = " NoTimeInForceRules" ; break;
		
	case 1120 : ret = " NoRootPartySubIDs" ; break;
		
	case 1087 : ret = " DisplayMinIncr" ; break;
		
	case 770 : ret = " TrdRegTimestampType" ; break;
		
	case 607 : ret = " LegProduct" ; break;
		
	case 1128 : ret = " ApplVerID" ; break;
		
	case 21 : ret = " HandlInst" ; break;
		
	case 1324 : ret = " ListUpdateAction" ; break;
		
	case 1211 : ret = " NestedInstrAttribValue" ; break;
		
	case 625 : ret = " TradingSessionSubID" ; break;
		
	case 644 : ret = " RFQReqID" ; break;
		
	case 1609 : ret = " RelationshipRiskSecurityExchange" ; break;
		
	case 1331 : ret = " UnderlyingLegSymbolSfx" ; break;
		
	case 441 : ret = " LiquidityNumSecurities" ; break;
		
	case 384 : ret = " NoMsgTypes" ; break;
		
	case 341 : ret = " TradSesStartTime" ; break;
		
	case 269 : ret = " MDEntryType" ; break;
		
	case 918 : ret = " AgreementCurrency" ; break;
		
	case 835 : ret = " PegMoveType" ; break;
		
	case 831 : ret = " AsgnReqID" ; break;
		
	case 218 : ret = " Spread" ; break;
		
	case 360 : ret = " EncodedAllocTextLen" ; break;
		
	case 407 : ret = " OutsideIndexPct" ; break;
		
	case 1243 : ret = " DerivFlexProductEligibilityIndicator" ; break;
		
	case 819 : ret = " AvgPxIndicator" ; break;
		
	case 199 : ret = " NoIOIQualifiers" ; break;
		
	case 480 : ret = " CancellationRights" ; break;
		
	case 67 : ret = " ListSeqNo" ; break;
		
	case 491 : ret = " CardIssNum" ; break;
		
	case 1546 : ret = " RiskCFICode" ; break;
		
	case 1398 : ret = " EncodedMktSegmDesc" ; break;
		
	case 1287 : ret = " DerivativeEventType" ; break;
		
	case 1251 : ret = " DerivativeMaturityMonthYear" ; break;
		
	case 1005 : ret = " SideTradeReportID" ; break;
		
	case 296 : ret = " NoQuoteSets" ; break;
		
	case 1610 : ret = " RelationshipRiskSecurityDesc" ; break;
		
	case 1411 : ret = " Nested4PartySubIDType" ; break;
		
	case 1364 : ret = " FillPx" ; break;
		
	case 1304 : ret = " StrikeExerciseStyle" ; break;
		
	case 284 : ret = " DeskID" ; break;
		
	case 413 : ret = " CrossPercent" ; break;
		
	case 200 : ret = " MaturityMonthYear" ; break;
		
	case 1486 : ret = " ComplexEventPrice" ; break;
		
	case 1475 : ret = " NoNewsRefIDs" ; break;
		
	case 1038 : ret = " UnderlyingCapValue" ; break;
		
	case 876 : ret = " CPRegType" ; break;
		
	case 499 : ret = " CashDistribAgentCode" ; break;
		
	case 484 : ret = " ExecPriceType" ; break;
		
	case 1366 : ret = " LegAllocID" ; break;
		
	case 273 : ret = " MDEntryTime" ; break;
		
	case 393 : ret = " TotalNumSecurities" ; break;
		
	case 780 : ret = " AllocSettlInstType" ; break;
		
	case 1462 : ret = " TargetPartyID" ; break;
		
	case 1262 : ret = " DerivativeStrikeCurrency" ; break;
		
	case 1176 : ret = " StatsType" ; break;
		
	case 1156 : ret = " ApplExtID" ; break;
		
	case 1153 : ret = " SettlementCycleNo" ; break;
		
	case 941 : ret = " UnderlyingStrikeCurrency" ; break;
		
	case 339 : ret = " TradSesMode" ; break;
		
	case 165 : ret = " SettlInstSource" ; break;
		
	case 1521 : ret = " PartyAltSubIDType" ; break;
		
	case 1392 : ret = " UnderlyingLegSecurityDesc" ; break;
		
	case 1292 : ret = " NoDerivativeInstrumentParties" ; break;
		
	case 1289 : ret = " DerivativeEventTime" ; break;
		
	case 1208 : ret = " TickIncrement" ; break;
		
	case 1058 : ret = " NoUndlyInstrumentParties" ; break;
		
	case 982 : ret = " ExpType" ; break;
		
	case 526 : ret = " SecondaryClOrdID" ; break;
		
	case 163 : ret = " SettlInstTransType" ; break;
		
	case 659 : ret = " SideComplianceID" ; break;
		
	case 749 : ret = " TradeRequestResult" ; break;
		
	case 713 : ret = " OrigPosReqRefID" ; break;
		
	case 551 : ret = " OrigCrossID" ; break;
		
	case 578 : ret = " TradeInputSource" ; break;
		
	case 192 : ret = " OrderQty2" ; break;
		
	case 464 : ret = " TestMessageIndicator" ; break;
		
	case 1527 : ret = " ContextPartySubID" ; break;
		
	case 1288 : ret = " DerivativeEventDate" ; break;
		
	case 1072 : ret = " SideGrossTradeAmt" ; break;
		
	case 839 : ret = " PeggedPrice" ; break;
		
	case 827 : ret = " ExpirationCycle" ; break;
		
	case 796 : ret = " AllocCancReplaceReason" ; break;
		
	case 102 : ret = " CxlRejReason" ; break;
		
	case 8 : ret = " BeginString" ; break;
		
	case 129 : ret = " DeliverToSubID" ; break;
		
	case 1569 : ret = " NoRelatedPartyAltIDs" ; break;
		
	case 1544 : ret = " RiskProductComplex" ; break;
		
	case 1422 : ret = " LegPriceUnitOfMeasureQty" ; break;
		
	case 938 : ret = " NoCollInquiryQualifier" ; break;
		
	case 133 : ret = " OfferPx" ; break;
		
	case 449 : ret = " TotalVolumeTradedDate" ; break;
		
	case 518 : ret = " NoContAmts" ; break;
		
	case 648 : ret = " MinOfferSize" ; break;
		
	case 860 : ret = " AvgParPx" ; break;
		
	case 253 : ret = " LegFactor" ; break;
		
	case 1172 : ret = " RespondentType" ; break;
		
	case 1085 : ret = " DisplayLowQty" ; break;
		
	case 127 : ret = " DKReason" ; break;
		
	case 662 : ret = " BenchmarkPrice" ; break;
		
	case 66 : ret = " ListID" ; break;
		
	case 605 : ret = " LegSecurityAltID" ; break;
		
	case 77 : ret = " PositionEffect" ; break;
		
	case 1568 : ret = " RelatedPartySubIDType" ; break;
		
	case 1101 : ret = " TriggerAction" ; break;
		
	case 1080 : ret = " RefOrderID" ; break;
		
	case 577 : ret = " ClearingInstruction" ; break;
		
	case 175 : ret = " SettlInstCode" ; break;
		
	case 157 : ret = " NumDaysInterest" ; break;
		
	case 286 : ret = " OpenCloseSettlFlag" ; break;
		
	case 1491 : ret = " NoComplexEventDates" ; break;
		
	case 1277 : ret = " DerivativeEncodedIssuerLen" ; break;
		
	case 967 : ret = " StrikeMultiplier" ; break;
		
	case 841 : ret = " DiscretionMoveType" ; break;
		
	case 68 : ret = " ListNoOrds" ; break;
		
	case 1564 : ret = " RelatedPartyIDSource" ; break;
		
	case 1098 : ret = " PegSymbol" ; break;
		
	case 426 : ret = " DayAvgPx" ; break;
		
	case 148 : ret = " Headline" ; break;
		
	case 545 : ret = " NestedPartySubID" ; break;
		
//	case 491 : ret = " CardIssNo" ; break;
		
	case 206 : ret = " OptAttribute" ; break;
		
	case 641 : ret = " LastForwardPoints2" ; break;
		
	case 265 : ret = " MDUpdateType" ; break;
		
	case 274 : ret = " TickDirection" ; break;
		
	case 254 : ret = " LegRedemptionDate" ; break;
		
	case 202 : ret = " StrikePrice" ; break;
		
	case 349 : ret = " EncodedIssuer" ; break;
		
	case 235 : ret = " YieldType" ; break;
		
	case 1150 : ret = " TradingReferencePrice" ; break;
		
	case 1026 : ret = " MDEntrySpotRate" ; break;
		
	case 849 : ret = " ParticipationRate" ; break;
		
	case 840 : ret = " PegScope" ; break;
		
	case 738 : ret = " InterestAtMaturity" ; break;
		
	case 672 : ret = " LegIndividualAllocID" ; break;
		
	case 766 : ret = " AllowableOneSidednessValue" ; break;
		
	case 182 : ret = " CashSettlAgentName" ; break;
		
	case 437 : ret = " ContraTradeQty" ; break;
		
	case 1212 : ret = " LegMaturityTime" ; break;
		
	case 172 : ret = " SettlDeliveryType" ; break;
		
	case 1305 : ret = " SecondaryPriceLimitType" ; break;
		
	case 631 : ret = " MidPx" ; break;
		
	case 6 : ret = " AvgPx" ; break;
		
	case 843 : ret = " DiscretionLimitType" ; break;
		
	case 443 : ret = " StrikeTime" ; break;
		
	case 717 : ret = " SettlSessSubID" ; break;
		
	case 474 : ret = " MailingDtls" ; break;
		
	case 390 : ret = " BidID" ; break;
		
	case 1511 : ret = " PartyDetailsRequestResult" ; break;
		
	case 747 : ret = " ExerciseMethod" ; break;
		
	case 479 : ret = " CommCurrency" ; break;
		
	case 1165 : ret = " NoSettlOblig" ; break;
		
	case 1143 : ret = " MaxPriceVariation" ; break;
		
	case 636 : ret = " WorkingIndicator" ; break;
		
	case 185 : ret = " CashSettlAgentAcctName" ; break;
		
	case 331 : ret = " SellVolume" ; break;
		
	case 752 : ret = " SideMultiLegReportingType" ; break;
		
	case 1291 : ret = " DerivativeEventText" ; break;
		
	case 1099 : ret = " PegSecurityDesc" ; break;
		
	case 993 : ret = " AllocCustomerCapacity" ; break;
		
	case 774 : ret = " ConfirmRejReason" ; break;
		
	case 374 : ret = " BidRequestTransType" ; break;
		
	case 500 : ret = " CashDistribAgentAcctNumber" ; break;
		
	case 1384 : ret = " LegExecInst" ; break;
		
	case 1199 : ret = " CapPrice" ; break;
		
	case 251 : ret = " LegRepurchaseTerm" ; break;
		
	case 493 : ret = " RegistAcctType" ; break;
		
	case 1376 : ret = " MassActionRejectReason" ; break;
		
	case 1323 : ret = " DerivativePutOrCall" ; break;
		
	case 1241 : ret = " StartMaturityMonthYear" ; break;
		
	case 1043 : ret = " CollApplType" ; break;
		
	case 984 : ret = " NoUnderlyingAmounts" ; break;
		
	case 773 : ret = " ConfirmType" ; break;
		
	case 610 : ret = " LegMaturityMonthYear" ; break;
		
	case 46 : ret = " RelatdSym" ; break;
		
	case 1531 : ret = " RiskLimitAmount" ; break;
		
	case 1338 : ret = " UnderlyingLegSecuritySubType" ; break;
		
	case 457 : ret = " NoUnderlyingSecurityAltID" ; break;
		
	case 1599 : ret = " RelationshipRiskCFICode" ; break;
		
	case 1566 : ret = " NoRelatedPartySubIDs" ; break;
		
	case 1537 : ret = " RiskSymbolSfx" ; break;
		
	case 1070 : ret = " MDQuoteType" ; break;
		
	case 854 : ret = " QtyType" ; break;
		
	case 694 : ret = " QuoteRespType" ; break;
		
	case 130 : ret = " IOINaturalFlag" ; break;
		
	case 222 : ret = " BenchmarkCurvePoint" ; break;
		
	case 1327 : ret = " TradSesUpdateAction" ; break;
		
	case 973 : ret = " UnderlyingCashAmount" ; break;
		
	case 902 : ret = " CollAsgnID" ; break;
		
	case 825 : ret = " ExchangeRule" ; break;
		
	case 359 : ret = " EncodedHeadline" ; break;
		
	case 513 : ret = " RegistID" ; break;
		
	case 548 : ret = " CrossID" ; break;
		
	case 124 : ret = " NoExecs" ; break;
		
	case 1247 : ret = " DerivativeSecurityGroup" ; break;
		
	case 1082 : ret = " SecondaryDisplayQty" ; break;
		
	case 372 : ret = " RefMsgType" ; break;
		
	case 171 : ret = " StandInstDbID" ; break;
		
	case 621 : ret = " EncodedLegSecurityDescLen" ; break;
		
	case 1290 : ret = " DerivativeEventPx" ; break;
		
	case 1164 : ret = " SettlObligSource" ; break;
		
	case 829 : ret = " TrdSubType" ; break;
		
	case 362 : ret = " EncodedUnderlyingIssuerLen" ; break;
		
	case 20 : ret = " ExecTransType" ; break;
		
	case 7 : ret = " BeginSeqNo" ; break;
		
	case 589 : ret = " DayBookingInst" ; break;
		
	case 1439 : ret = " FlowScheduleType" ; break;
		
	case 1024 : ret = " MDOriginType" ; break;
		
	case 945 : ret = " CollInquiryStatus" ; break;
		
	case 870 : ret = " NoInstrAttrib" ; break;
		
	case 511 : ret = " RegistEmail" ; break;
		
	case 1497 : ret = " StreamAsgnReqID" ; break;
		
	case 875 : ret = " CPProgram" ; break;
		
	case 859 : ret = " ConfirmReqID" ; break;
		
	case 817 : ret = " AltMDSourceID" ; break;
		
	case 73 : ret = " NoOrders" ; break;
		
	case 502 : ret = " CashDistribAgentAcctName" ; break;
		
	case 1526 : ret = " NoContextPartySubIDs" ; break;
		
	case 1060 : ret = " UndlyInstrumentPartyIDSource" ; break;
		
	case 1039 : ret = " UnderlyingSettlMethod" ; break;
		
	case 267 : ret = " NoMDEntryTypes" ; break;
		
	case 1027 : ret = " MDEntryForwardPoints" ; break;
		
	case 724 : ret = " PosReqType" ; break;
		
	case 585 : ret = " MassStatusReqType" ; break;
		
	case 229 : ret = " TradeOriginationDate" ; break;
		
	case 730 : ret = " SettlPrice" ; break;
		
	case 179 : ret = " SecuritySettlAgentAcctName" ; break;
		
	case 1558 : ret = " RiskInstrumentMultiplier" ; break;
		
	case 1286 : ret = " NoDerivativeEvents" ; break;
		
	case 883 : ret = " UnderlyingEndPrice" ; break;
		
	case 263 : ret = " SubscriptionRequestType" ; break;
		
	case 557 : ret = " TotalNumSecurityTypes" ; break;
		
	case 1473 : ret = " NewsCategory" ; break;
		
	case 1343 : ret = " UnderlyingLegPutOrCall" ; break;
		
	case 149 : ret = " URLLink" ; break;
		
	case 1052 : ret = " NoInstrumentPartySubIDs" ; break;
		
	case 885 : ret = " UnderlyingCurrentValue" ; break;
		
	case 874 : ret = " InterestAccrualDate" ; break;
		
	case 193 : ret = " FutSettDate2" ; break;
		
	case 576 : ret = " NoClearingInstructions" ; break;
		
	case 318 : ret = " UnderlyingCurrency" ; break;
		
	case 956 : ret = " LegInterestAccrualDate" ; break;
		
	case 925 : ret = " NewPassword" ; break;
		
	case 240 : ret = " RedemptionDate" ; break;
		
	case 1357 : ret = " RefApplLastSeqNum" ; break;
		
	case 921 : ret = " StartCash" ; break;
		
	case 383 : ret = " MaxMessageSize" ; break;
		
	case 191 : ret = " OfferForwardPoints" ; break;
		
//	case 27 : ret = " IOIQty" ; break;
		
	case 32 : ret = " LastQty" ; break;
		
	case 1354 : ret = " ApplResponseError" ; break;
		
	case 1337 : ret = " UnderlyingLegSecurityType" ; break;
		
	case 1315 : ret = " DerivativePriceUnitOfMeasure" ; break;
		
	case 1109 : ret = " TriggerPriceDirection" ; break;
		
	case 1055 : ret = " PositionCurrency" ; break;
		
	case 1011 : ret = " MessageEventSource" ; break;
		
	case 909 : ret = " CollInquiryID" ; break;
		
	case 884 : ret = " UnderlyingStartValue" ; break;
		
	case 851 : ret = " LastLiquidityInd" ; break;
		
	case 48 : ret = " SecurityID" ; break;
		
	case 268 : ret = " NoMDEntries" ; break;
		
	case 1506 : ret = " NoPartyListResponseTypes" ; break;
		
	case 1478 : ret = " StrikePriceDeterminationMethod" ; break;
		
	case 917 : ret = " EndDate" ; break;
		
	case 901 : ret = " CashOutstanding" ; break;
		
	case 262 : ret = " MDReqID" ; break;
		
	case 26 : ret = " IOIRefID" ; break;
		
	case 1522 : ret = " NoContextPartyIDs" ; break;
		
	case 847 : ret = " TargetStrategy" ; break;
		
	case 772 : ret = " ConfirmRefID" ; break;
		
	case 287 : ret = " SellerDays" ; break;
		
	case 329 : ret = " DueToRelated" ; break;
		
	case 1240 : ret = " SecondaryTradingReferencePrice" ; break;
		
	case 1141 : ret = " NoMDFeedTypes" ; break;
		
	case 1064 : ret = " UnderlyingInstrumentPartySubIDType" ; break;
		
	case 344 : ret = " TradSesCloseTime" ; break;
		
	case 667 : ret = " ContractSettlMonth" ; break;
		
	case 1261 : ret = " DerivativeStrikePrice" ; break;
		
	case 1106 : ret = " TriggerSecurityDesc" ; break;
		
	case 974 : ret = " UnderlyingCashType" ; break;
		
	case 136 : ret = " NoMiscFees" ; break;
		
	case 582 : ret = " CustOrderCapacity" ; break;
		
	case 1616 : ret = " RiskSecurityExchange" ; break;
		
	case 1367 : ret = " LegAllocSettlCurrency" ; break;
		
	case 1044 : ret = " UnderlyingAdjustedQuantity" ; break;
		
	case 517 : ret = " OwnershipType" ; break;
		
	case 210 : ret = " MaxShow" ; break;
		
	case 602 : ret = " LegSecurityID" ; break;
		
	case 1360 : ret = " EncodedSymbol" ; break;
		
	case 1279 : ret = " DerivativeSecurityDesc" ; break;
		
	case 996 : ret = " UnitOfMeasure" ; break;
		
	case 653 : ret = " SecDefStatus" ; break;
		
	case 53 : ret = " Quantity" ; break;
		
	case 1472 : ret = " NewsID" ; break;
		
//	case 1064 : ret = " UndlyInstrumentPartySubIDType" ; break;
		
	case 966 : ret = " SettleOnOpenFlag" ; break;
		
	case 779 : ret = " LastUpdateTime" ; break;
		
	case 227 : ret = " RepurchaseRate" ; break;
		
	case 226 : ret = " RepurchaseTerm" ; break;
		
	case 538 : ret = " NestedPartyRole" ; break;
		
	case 527 : ret = " SecondaryExecID" ; break;
		
	case 691 : ret = " Pool" ; break;
		
	case 1205 : ret = " NoTickRules" ; break;
		
	case 1188 : ret = " Volatility" ; break;
		
	case 869 : ret = " PctAtRisk" ; break;
		
	case 308 : ret = " UnderlyingSecurityExchange" ; break;
		
	case 612 : ret = " LegStrikePrice" ; break;
		
	case 63 : ret = " SettlmntTyp" ; break;
		
	case 1390 : ret = " TradePublishIndicator" ; break;
		
	case 1348 : ret = " ApplResponseType" ; break;
		
	case 1173 : ret = " MDSubBookType" ; break;
		
	case 553 : ret = " Username" ; break;
		
	case 169 : ret = " StandInstDbType" ; break;
		
	case 1577 : ret = " RelatedContextPartyIDSource" ; break;
		
	case 1167 : ret = " QuoteEntryStatus" ; break;
		
	case 1107 : ret = " TriggerPriceType" ; break;
		
	case 1008 : ret = " SideTrdSubTyp" ; break;
		
	case 823 : ret = " UnderlyingTradingSessionSubID" ; break;
		
	case 792 : ret = " SettlInstReqRejCode" ; break;
		
	case 645 : ret = " MktBidPx" ; break;
		
	case 1330 : ret = " UnderlyingLegSymbol" ; break;
		
	case 968 : ret = " StrikeValue" ; break;
		
	case 61 : ret = " Urgency" ; break;
		
	case 70 : ret = " AllocID" ; break;
		
	case 1513 : ret = " NoPartyList" ; break;
		
	case 1037 : ret = " UnderlyingDeliveryAmount" ; break;
		
//	case 1009 : ret = " SideQty" ; break;
		
	case 903 : ret = " CollAsgnTransType" ; break;
		
	case 834 : ret = " ThresholdAmount" ; break;
		
	case 293 : ret = " DefBidSize" ; break;
		
	case 597 : ret = " LegStateOrProvinceOfIssue" ; break;
		
	case 492 : ret = " PaymentMethod" ; break;
		
	case 1555 : ret = " RiskCouponRate" ; break;
		
	case 1391 : ret = " UnderlyingLegOptAttribute" ; break;
		
	case 1379 : ret = " LegVolatility" ; break;
		
	case 1313 : ret = " DerivativeInstrAttribType" ; break;
		
	case 1270 : ret = " DerivativeUnitOfMeasureQty" ; break;
		
	case 1175 : ret = " NoStatsIndicators" ; break;
		
	case 1108 : ret = " TriggerPriceTypeScope" ; break;
		
	case 934 : ret = " LastNetworkResponseID" ; break;
		
	case 733 : ret = " UnderlyingSettlPriceType" ; break;
		
	case 1356 : ret = " ApplReportID" ; break;
		
	case 837 : ret = " PegLimitType" ; break;
		
	case 17 : ret = " ExecID" ; break;
		
	case 54 : ret = " Side" ; break;
		
	case 651 : ret = " UnderlyingLastPx" ; break;
		
	case 264 : ret = " MarketDepth" ; break;
		
	case 389 : ret = " DiscretionOffset" ; break;
		
	case 519 : ret = " ContAmtType" ; break;
		
	case 138 : ret = " MiscFeeCurr" ; break;
		
	case 1529 : ret = " NoRiskLimits" ; break;
		
	case 1457 : ret = " AttachmentPoint" ; break;
		
	case 1115 : ret = " OrderCategory" ; break;
		
	case 5 : ret = " AdvTransType" ; break;
		
	case 410 : ret = " WtAverageLiquidity" ; break;
		
	case 367 : ret = " QuoteSetValidUntilTime" ; break;
		
	case 1414 : ret = " NoNested4PartyIDs" ; break;
		
	case 1358 : ret = " LegPutOrCall" ; break;
		
	case 927 : ret = " UserStatusText" ; break;
		
	case 760 : ret = " Nested2PartySubID" ; break;
		
	case 405 : ret = " EFPTrackingError" ; break;
		
	case 1444 : ret = " SideLiquidityInd" ; break;
		
	case 1267 : ret = " DerivativeMinPriceIncrement" ; break;
		
	case 852 : ret = " PublishTrdIndicator" ; break;
		
	case 475 : ret = " InvestorCountryOfResidence" ; break;
		
	case 1007 : ret = " SideReasonCd" ; break;
		
	case 969 : ret = " MinPriceIncrement" ; break;
		
	case 180 : ret = " SecuritySettlAgentContactName" ; break;
		
	case 323 : ret = " SecurityResponseType" ; break;
		
	case 678 : ret = " LegBenchmarkCurvePoint" ; break;
		
	case 439 : ret = " ClearingFirm" ; break;
		
	case 1592 : ret = " RelationshipRiskSecurityIDSource" ; break;
		
	case 1409 : ret = " SessionStatus" ; break;
		
	case 1104 : ret = " TriggerSecurityID" ; break;
		
	case 892 : ret = " TotNoAllocs" ; break;
		
	case 816 : ret = " NoAltMDSource" ; break;
		
	case 798 : ret = " AllocAccountType" ; break;
		
	case 31 : ret = " LastPx" ; break;
		
	case 71 : ret = " AllocTransType" ; break;
		
//	case 304 : ret = " TotNoQuoteEntries" ; break;
		
	case 647 : ret = " MinBidSize" ; break;
		
	case 174 : ret = " SettlBrkrCode" ; break;
		
	case 488 : ret = " CardHolderName" ; break;
		
//	case 982 : ret = " ExpirationQtyType" ; break;
		
	case 364 : ret = " EncodedUnderlyingSecurityDescLen" ; break;
		
	case 131 : ret = " QuoteReqID" ; break;
		
	case 1572 : ret = " NoRelatedPartyAltSubIDs" ; break;
		
	case 1543 : ret = " RiskProduct" ; break;
		
	case 1542 : ret = " RiskSecurityAltIDSource" ; break;
		
	case 1191 : ret = " PriceUnitOfMeasure" ; break;
		
	case 1132 : ret = " TZTransactTime" ; break;
		
	case 209 : ret = " AllocHandlInst" ; break;
		
//	case 1060 : ret = " UnderlyingInstrumentPartyIDSource" ; break;
		
	case 1382 : ret = " CurrencyRatio" ; break;
		
	case 1088 : ret = " RefreshQty" ; break;
		
	case 750 : ret = " TradeRequestStatus" ; break;
		
	case 1389 : ret = " TrdRepIndicator" ; break;
		
	case 137 : ret = " MiscFeeAmt" ; break;
		
	case 342 : ret = " TradSesOpenTime" ; break;
		
	case 591 : ret = " PreallocMethod" ; break;
		
	case 495 : ret = " TaxAdvantageType" ; break;
		
	case 347 : ret = " MessageEncoding" ; break;
		
	case 1553 : ret = " RiskPutOrCall" ; break;
		
	case 1545 : ret = " RiskSecurityGroup" ; break;
		
	case 802 : ret = " NoPartySubIDs" ; break;
		
	case 791 : ret = " SettlInstReqID" ; break;
		
	case 250 : ret = " LegRepoCollateralSecurityType" ; break;
		
	case 536 : ret = " AffectedSecondaryOrderID" ; break;
		
	case 1536 : ret = " RiskSymbol" ; break;
		
	case 1253 : ret = " DerivativeMaturityTime" ; break;
		
	case 126 : ret = " ExpireTime" ; break;
		
	case 246 : ret = " UnderlyingFactor" ; break;
		
	case 586 : ret = " OrigOrdModTime" ; break;
		
	case 1387 : ret = " NoTrdRepIndicators" ; break;
		
	case 1252 : ret = " DerivativeMaturityDate" ; break;
		
	case 1248 : ret = " DerivativeCFICode" ; break;
		
	case 807 : ret = " Nested2PartySubIDType" ; break;
		
	case 682 : ret = " LegIOIQty" ; break;
		
	case 432 : ret = " ExpireDate" ; break;
		
	case 1547 : ret = " RiskSecurityType" ; break;
		
	case 1235 : ret = " NoMatchRules" ; break;
		
	case 1183 : ret = " ApplEndSeqNum" ; break;
		
	case 867 : ret = " EventPx" ; break;
		
	case 833 : ret = " AsgnRptID" ; break;
		
	case 59 : ret = " TimeInForce" ; break;
		
	case 333 : ret = " LowPx" ; break;
		
	case 104 : ret = " IOIQualifier" ; break;
		
	case 105 : ret = " WaveNo" ; break;
		
	case 1552 : ret = " RiskSeniority" ; break;
		
	case 1479 : ret = " StrikePriceBoundaryMethod" ; break;
		
	case 1276 : ret = " DerivativeIssueDate" ; break;
		
	case 139 : ret = " MiscFeeType" ; break;
		
	case 117 : ret = " QuoteID" ; break;
		
	case 1554 : ret = " RiskFlexibleIndicator" ; break;
		
	case 1294 : ret = " DerivativeInstrumentPartyIDSource" ; break;
		
	case 1161 : ret = " SettlObligID" ; break;
		
	case 872 : ret = " InstrAttribValue" ; break;
		
	case 404 : ret = " LiquidityValue" ; break;
		
//	case 22 : ret = " SecurityIDSource" ; break;
		
	case 1477 : ret = " NewsRefType" ; break;
		
	case 1342 : ret = " NoOfLegUnderlyings" ; break;
		
	case 1281 : ret = " DerivativeEncodedSecurityDesc" ; break;
		
	case 1111 : ret = " TriggerOrderType" ; break;
		
	case 882 : ret = " UnderlyingDirtyPrice" ; break;
		
	case 549 : ret = " CrossType" ; break;
		
	case 239 : ret = " RepoCollateralSecurityType" ; break;
		
	case 554 : ret = " Password" ; break;
		
//	case 286 : ret = " OpenCloseSettleFlag" ; break;
		
	case 147 : ret = " Subject" ; break;
		
	case 1433 : ret = " RefApplReqID" ; break;
		
	case 886 : ret = " UnderlyingEndValue" ; break;
		
	case 36 : ret = " NewSeqNo" ; break;
		
	case 1124 : ret = " OrigTradeHandlingInstr" ; break;
		
	case 1086 : ret = " DisplayHighQty" ; break;
		
	case 1021 : ret = " MDBookType" ; break;
		
	case 899 : ret = " MarginExcess" ; break;
		
	case 419 : ret = " BasisPxType" ; break;
		
	case 86 : ret = " DlvyInst" ; break;
		
	case 376 : ret = " ComplianceID" ; break;
		
	case 164 : ret = " EmailThreadID" ; break;
		
	case 521 : ret = " ContAmtCurr" ; break;
		
	case 1598 : ret = " RelationshipRiskSecurityGroup" ; break;
		
	case 1484 : ret = " ComplexEventType" ; break;
		
	case 1375 : ret = " MassActionResponse" ; break;
		
	case 242 : ret = " UnderlyingIssueDate" ; break;
		
	case 321 : ret = " SecurityRequestType" ; break;
		
	case 741 : ret = " AllocInterestAtMaturity" ; break;
		
	case 1386 : ret = " ListRejectReason" ; break;
		
	case 1033 : ret = " DeskType" ; break;
		
	case 818 : ret = " SecondaryTradeReportID" ; break;
		
//	case 63 : ret = " SettlType" ; break;
		
//	case 77 : ret = " OpenClose" ; break;
		
	case 1435 : ret = " ContractMultiplierUnit" ; break;
		
	case 1221 : ret = " SecondaryLowLimitPrice" ; break;
		
	case 983 : ret = " ExpQty" ; break;
		
	case 933 : ret = " NetworkRequestID" ; break;
		
	case 828 : ret = " TrdType" ; break;
		
	case 711 : ret = " NoUnderlyings" ; break;
		
	case 275 : ret = " MDMkt" ; break;
		
	case 30 : ret = " LastMkt" ; break;
		
	case 1449 : ret = " RestructuringType" ; break;
		
	case 1201 : ret = " NoStrikeRules" ; break;
		
	case 392 : ret = " ListName" ; break;
		
	case 414 : ret = " ProgRptReqs" ; break;
		
	case 336 : ret = " TradingSessionID" ; break;
		
	case 431 : ret = " ListOrderStatus" ; break;
		
	case 506 : ret = " RegistStatus" ; break;
		
	case 708 : ret = " PosAmt" ; break;
		
	case 1481 : ret = " UnderlyingPriceDeterminationMethod" ; break;
		
	case 887 : ret = " NoUnderlyingStips" ; break;
		
	case 343 : ret = " TradSesPreCloseTime" ; break;
		
	case 530 : ret = " MassCancelRequestType" ; break;
		
	case 1336 : ret = " UnderlyingLegSecurityAltIDSource" ; break;
		
	case 782 : ret = " SettlPartyID" ; break;
		
	case 534 : ret = " NoAffectedOrders" ; break;
		
	case 184 : ret = " CashSettlAgentAcctNum" ; break;
		
	case 1339 : ret = " UnderlyingLegMaturityMonthYear" ; break;
		
	case 1307 : ret = " DerivativeSecurityListRequestType" ; break;
		
	case 1234 : ret = " NoLotTypeRules" ; break;
		
	case 580 : ret = " NoDates" ; break;
		
	case 434 : ret = " CxlRejResponseTo" ; break;
		
	case 168 : ret = " EffectiveTime" ; break;
		
	case 381 : ret = " GrossTradeAmt" ; break;
		
	case 1523 : ret = " ContextPartyID" ; break;
		
	case 1467 : ret = " SecurityListDesc" ; break;
		
	case 1371 : ret = " NotAffectedOrderID" ; break;
		
	case 1264 : ret = " DerivativeStrikeValue" ; break;
		
	case 753 : ret = " NoPosAmt" ; break;
		
	case 257 : ret = " LegCreditRating" ; break;
		
	case 1611 : ret = " RelationshipRiskInstrumentSettlType" ; break;
		
	case 642 : ret = " BidForwardPoints2" ; break;
		
//	case 64 : ret = " SettlDate" ; break;
		
	case 109 : ret = " ClientID" ; break;
		
	case 298 : ret = " QuoteCancelType" ; break;
		
	case 233 : ret = " StipulationType" ; break;
		
	case 412 : ret = " OutMainCntryUIndex" ; break;
		
//	case 587 : ret = " LegSettlmntTyp" ; break;
		
	case 1587 : ret = " NoRelationshipRiskInstruments" ; break;
		
	case 1274 : ret = " DerivativeNTPositionLimit" ; break;
		
	case 1196 : ret = " PriceQuoteMethod" ; break;
		
	case 1148 : ret = " LowLimitPrice" ; break;
		
	case 999 : ret = " LegUnitOfMeasure" ; break;
		
	case 373 : ret = " SessionRejectReason" ; break;
		
	case 1510 : ret = " PartyDetailsListReportID" ; break;
		
	case 919 : ret = " DeliveryType" ; break;
		
	case 366 : ret = " AllocPrice" ; break;
		
	case 420 : ret = " NoBidComponents" ; break;
		
	case 695 : ret = " QuoteQualifier" ; break;
		
	case 546 : ret = " Scope" ; break;
		
	case 454 : ret = " NoSecurityAltID" ; break;
		
	case 1121 : ret = " RootPartySubID" ; break;
		
	case 145 : ret = " DeliverToLocationID" ; break;
		
	case 285 : ret = " DeleteReason" ; break;
		
	case 188 : ret = " BidSpotRate" ; break;
		
	case 1415 : ret = " Nested4PartyID" ; break;
		
	case 328 : ret = " InViewOfCommon" ; break;
		
	case 732 : ret = " UnderlyingSettlPrice" ; break;
		
	case 496 : ret = " RegistRejReasonText" ; break;
		
	case 552 : ret = " NoSides" ; break;
		
	case 671 : ret = " LegAllocAccount" ; break;
		
	case 1613 : ret = " NoRelationshipRiskWarningLevels" ; break;
		
	case 1596 : ret = " RelationshipRiskProduct" ; break;
		
	case 620 : ret = " LegSecurityDesc" ; break;
		
	case 583 : ret = " ClOrdLinkID" ; break;
		
	case 122 : ret = " OrigSendingTime" ; break;
		
	case 618 : ret = " EncodedLegIssuerLen" ; break;
		
	case 37 : ret = " OrderID" ; break;
		
	case 167 : ret = " SecurityType" ; break;
		
	case 468 : ret = " RoundingDirection" ; break;
		
	case 1363 : ret = " FillExecID" ; break;
		
	case 864 : ret = " NoEvents" ; break;
		
	case 561 : ret = " RoundLot" ; break;
		
	case 271 : ret = " MDEntrySize" ; break;
		
	case 348 : ret = " EncodedIssuerLen" ; break;
		
	case 1316 : ret = " DerivativePriceUnitOfMeasureQty" ; break;
		
	case 997 : ret = " TimeUnit" ; break;
		
//	case 68 : ret = " TotNoOrders" ; break;
		
	case 1517 : ret = " PartyAltID" ; break;
		
	case 690 : ret = " LegSwapType" ; break;
		
	case 28 : ret = " IOITransType" ; break;
		
	case 95 : ret = " RawDataLength" ; break;
		
	case 310 : ret = " UnderlyingSecurityType" ; break;
		
	case 1335 : ret = " UnderlyingLegSecurityAltID" ; break;
		
	case 964 : ret = " SecurityReportID" ; break;
		
	case 911 : ret = " TotNumReports" ; break;
		
	case 727 : ret = " TotalNumPosReports" ; break;
		
	case 320 : ret = " SecurityReqID" ; break;
		
	case 728 : ret = " PosReqResult" ; break;
		
	case 1068 : ret = " LegOfferForwardPoints" ; break;
		
	case 767 : ret = " AllowableOneSidednessCurr" ; break;
		
	case 535 : ret = " AffectedOrderID" ; break;
		
	case 592 : ret = " UnderlyingCountryOfIssue" ; break;
		
	case 245 : ret = " UnderlyingRepurchaseRate" ; break;
		
	case 369 : ret = " LastMsgSeqNumProcessed" ; break;
		
	case 463 : ret = " UnderlyingCFICode" ; break;
		
	case 1265 : ret = " DerivativeOptAttribute" ; break;
		
	case 1097 : ret = " PegSecurityID" ; break;
		
	case 961 : ret = " HostCrossID" ; break;
		
	case 1308 : ret = " ExecInstValue" ; break;
		
	case 1225 : ret = " DerivativeOptPayAmount" ; break;
		
	case 435 : ret = " UnderlyingCouponRate" ; break;
		
	case 160 : ret = " SettlInstMode" ; break;
		
	case 456 : ret = " SecurityAltIDSource" ; break;
		
	case 570 : ret = " PreviouslyReported" ; break;
		
	case 1524 : ret = " ContextPartyIDSource" ; break;
		
	case 1135 : ret = " RptSys" ; break;
		
	case 806 : ret = " NoNested2PartySubIDs" ; break;
		
	case 72 : ret = " RefAllocID" ; break;
		
	case 294 : ret = " DefOfferSize" ; break;
		
	case 1227 : ret = " ProductComplex" ; break;
		
	case 1031 : ret = " CustOrderHandlingInst" ; break;
		
	case 1023 : ret = " MDPriceLevel" ; break;
		
	case 1017 : ret = " LegOptionRatio" ; break;
		
	case 965 : ret = " SecurityStatus" ; break;
		
	case 654 : ret = " LegRefID" ; break;
		
	case 1380 : ret = " DividendYield" ; break;
		
	case 1298 : ret = " DerivativeInstrumentPartySubIDType" ; break;
		
	case 1203 : ret = " EndStrikePxRange" ; break;
		
	case 1138 : ret = " DisplayQty" ; break;
		
	case 764 : ret = " LegSecuritySubType" ; break;
		
	case 81 : ret = " ProcessCode" ; break;
		
	case 18 : ret = " ExecInst" ; break;
		
	case 345 : ret = " TradSesEndTime" ; break;
		
	case 42 : ret = " OrigTime" ; break;
		
	case 515 : ret = " ExecValuationPoint" ; break;
		
	case 150 : ret = " ExecType" ; break;
		
	case 1579 : ret = " NoRelatedContextPartySubIDs" ; break;
		
	case 1417 : ret = " Nested4PartyRole" ; break;
		
	case 1377 : ret = " MultilegModel" ; break;
		
	case 1151 : ret = " SecurityGroup" ; break;
		
	case 865 : ret = " EventType" ; break;
		
	case 826 : ret = " TradeAllocIndicator" ; break;
		
	case 701 : ret = " YieldCalcDate" ; break;
		
	case 408 : ret = " ValueOfFutures" ; break;
		
	case 624 : ret = " LegSide" ; break;
		
	case 926 : ret = " UserStatus" ; break;
		
	case 396 : ret = " SideValue1" ; break;
		
	case 84 : ret = " CxlQty" ; break;
		
	case 322 : ret = " SecurityResponseID" ; break;
		
	case 543 : ret = " InstrRegistry" ; break;
		
	case 1501 : ret = " StreamAsgnRptID" ; break;
		
	case 1429 : ret = " OrderDelayUnit" ; break;
		
	case 1383 : ret = " LegCurrencyRatio" ; break;
		
	case 1207 : ret = " EndTickPriceRange" ; break;
		
	case 894 : ret = " CollReqID" ; break;
		
	case 740 : ret = " LegPool" ; break;
		
	case 705 : ret = " ShortQty" ; break;
		
	case 397 : ret = " SideValue2" ; break;
		
	case 258 : ret = " TradedFlatSwitch" ; break;
		
	case 584 : ret = " MassStatusReqID" ; break;
		
	case 1493 : ret = " ComplexEventEndDate" ; break;
		
	case 1301 : ret = " MarketID" ; break;
		
	case 1125 : ret = " OrigTradeDate" ; break;
		
	case 1091 : ret = " PreTradeAnonymity" ; break;
		
	case 939 : ret = " TrdRptStatus" ; break;
		
	case 512 : ret = " DistribPercentage" ; break;
		
//	case 297 : ret = " QuoteStatus" ; break;
		
	case 440 : ret = " ClearingAccount" ; break;
		
	case 880 : ret = " TrdMatchID" ; break;
		
	case 821 : ret = " OrderInputDevice" ; break;
		
	case 377 : ret = " SolicitedFlag" ; break;
		
	case 60 : ret = " TransactTime" ; break;
		
	case 1530 : ret = " RiskLimitType" ; break;
		
	case 1441 : ret = " UnderlyingFlowScheduleType" ; break;
		
	case 889 : ret = " UnderlyingStipValue" ; break;
		
	case 789 : ret = " NextExpectedMsgSeqNum" ; break;
		
	case 220 : ret = " BenchmarkCurveCurrency" ; break;
		
	case 461 : ret = " CFICode" ; break;
		
	case 228 : ret = " Factor" ; break;
		
//	case 32 : ret = " LastShares" ; break;
		
	case 1509 : ret = " RequestedPartyRole" ; break;
		
	case 1145 : ret = " EventTime" ; break;
		
	case 1122 : ret = " RootPartySubIDType" ; break;
		
	case 853 : ret = " ShortSaleReason" ; break;
		
	case 213 : ret = " XmlData" ; break;
		
	case 1605 : ret = " RelationshipRiskSeniority" ; break;
		
	case 1461 : ret = " NoTargetPartyIDs" ; break;
		
	case 1116 : ret = " NoRootPartyIDs" ; break;
		
	case 866 : ret = " EventDate" ; break;
		
	case 838 : ret = " PegRoundDirection" ; break;
		
//	case 588 : ret = " LegSettlDate" ; break;
		
	case 1434 : ret = " ModelType" ; break;
		
	case 1410 : ret = " DefaultVerIndicator" ; break;
		
//	case 1197 : ret = " FuturesValuationMethod" ; break;
		
	case 1193 : ret = " SettlMethod" ; break;
		
	case 1045 : ret = " UnderlyingFXRate" ; break;
		
	case 665 : ret = " ConfirmStatus" ; break;
		
	case 114 : ret = " LocateReqd" ; break;
		
	case 721 : ret = " PosMaintRptID" ; break;
		
	case 334 : ret = " Adjustment" ; break;
		
	case 1617 : ret = " StreamAsgnType" ; break;
		
	case 912 : ret = " LastRptRequested" ; break;
		
	case 472 : ret = " LocaleOfIssue" ; break;
		
	case 50 : ret = " SenderSubID" ; break;
		
	case 332 : ret = " HighPx" ; break;
		
	case 737 : ret = " AllocSettlCurrAmt" ; break;
		
	case 1488 : ret = " ComplexEventPriceBoundaryPrecision" ; break;
		
	case 1051 : ret = " InstrumentPartyRole" ; break;
		
	case 697 : ret = " YieldRedemptionPrice" ; break;
		
	case 14 : ret = " CumQty" ; break;
		
	case 41 : ret = " OrigClOrdID" ; break;
		
	case 716 : ret = " SettlSessID" ; break;
		
	case 1325 : ret = " ParentMktSegmID" ; break;
		
	case 856 : ret = " TradeReportType" ; break;
		
	case 74 : ret = " AvgPrxPrecision" ; break;
		
	case 555 : ret = " NoLegs" ; break;
		
	case 311 : ret = " UnderlyingSymbol" ; break;
		
	case 1194 : ret = " ExerciseStyle" ; break;
		
//	case 327 : ret = " HaltReasonChar" ; break;
		
	case 100 : ret = " ExDestination" ; break;
		
	case 1514 : ret = " NoPartyRelationships" ; break;
		
	case 1255 : ret = " DerivativeInstrmtAssignmentMethod" ; break;
		
	case 305 : ret = " UnderlyingIDSource" ; break;
		
	case 2 : ret = " AdvId" ; break;
		
	case 483 : ret = " TransBkdTime" ; break;
		
	case 637 : ret = " LegLastPx" ; break;
		
	case 1559 : ret = " NoRiskWarningLevels" ; break;
		
	case 794 : ret = " AllocReportType" ; break;
		
	case 509 : ret = " RegistDtls" ; break;
		
	case 626 : ret = " AllocType" ; break;
		
	case 658 : ret = " QuoteRequestRejectReason" ; break;
		
	case 998 : ret = " UnderlyingUnitOfMeasure" ; break;
		
	case 467 : ret = " IndividualAllocID" ; break;
		
	case 684 : ret = " LegOfferPx" ; break;
		
	case 409 : ret = " LiquidityIndType" ; break;
		
	case 629 : ret = " HopSendingTime" ; break;
		
	case 1584 : ret = " RelationshipRiskLimitAmount" ; break;
		
	case 1352 : ret = " ApplResendFlag" ; break;
		
	case 1321 : ret = " DerivativeCapPrice" ; break;
		
	case 1538 : ret = " RiskSecurityID" ; break;
		
	case 1485 : ret = " ComplexOptPayoutAmount" ; break;
		
	case 1474 : ret = " LanguageCode" ; break;
		
	case 1163 : ret = " SettlObligRefID" ; break;
		
	case 1126 : ret = " OrigTradeID" ; break;
		
	case 986 : ret = " UnderlyingCollectAmount" ; break;
		
	case 928 : ret = " StatusValue" ; break;
		
	case 190 : ret = " OfferSpotRate" ; break;
		
	case 703 : ret = " PosType" ; break;
		
	case 247 : ret = " UnderlyingRedemptionDate" ; break;
		
	case 173 : ret = " SettlDepositoryCode" ; break;
		
	case 1503 : ret = " StreamAsgnAckType" ; break;
		
	case 1200 : ret = " FloorPrice" ; break;
		
	case 1550 : ret = " RiskMaturityTime" ; break;
		
	case 1425 : ret = " UnderlyingPriceUnitOfMeasureQty" ; break;
		
	case 1329 : ret = " FeeMultiplier" ; break;
		
	case 1213 : ret = " UnderlyingMaturityTime" ; break;
		
	case 1180 : ret = " ApplID" ; break;
		
	case 1075 : ret = " LegGrossTradeAmt" ; break;
		
	case 272 : ret = " MDEntryDate" ; break;
		
	case 676 : ret = " LegBenchmarkCurveCurrency" ; break;
		
	case 1535 : ret = " RiskInstrumentOperator" ; break;
		
	case 1195 : ret = " OptPayoutAmount" ; break;
		
	case 891 : ret = " MiscFeeBasis" ; break;
		
	case 62 : ret = " ValidUntilTime" ; break;
		
	case 40 : ret = " OrdType" ; break;
		
	case 3 : ret = " AdvRefID" ; break;
		
	case 628 : ret = " HopCompID" ; break;
		
	case 714 : ret = " PosMaintRptRefID" ; break;
		
	case 689 : ret = " LegStipulationValue" ; break;
		
	case 574 : ret = " MatchType" ; break;
		
	case 1482 : ret = " OptPayoutType" ; break;
		
	case 1424 : ret = " UnderlyingPriceUnitOfMeasure" ; break;
		
	case 1395 : ret = " MarketUpdateAction" ; break;
		
	case 906 : ret = " CollAsgnRejectReason" ; break;
		
	case 1095 : ret = " PeggedRefPrice" ; break;
		
	case 992 : ret = " IndividualAllocType" ; break;
		
	case 922 : ret = " EndCash" ; break;
		
	case 868 : ret = " EventText" ; break;
		
	case 230 : ret = " ExDate" ; break;
		
	case 525 : ret = " NestedPartyIDSource" ; break;
		
	case 427 : ret = " GTBookingInst" ; break;
		
//	case 1319 : ret = " DerivativeValuationMethod" ; break;
		
	case 346 : ret = " NumberOfOrders" ; break;
		
	case 1388 : ret = " TrdRepPartyRole" ; break;
		
	case 1102 : ret = " TriggerPrice" ; break;
		
	case 963 : ret = " MDReportID" ; break;
		
	case 793 : ret = " SecondaryAllocID" ; break;
		
	case 151 : ret = " LeavesQty" ; break;
		
	case 503 : ret = " CardStartDate" ; break;
		
	case 565 : ret = " LegCoveredOrUncovered" ; break;
		
	case 201 : ret = " PutOrCall" ; break;
		
	case 1142 : ret = " MatchAlgorithm" ; break;
		
	case 1056 : ret = " CalculatedCcyLastQty" ; break;
		
	case 497 : ret = " FundRenewWaiv" ; break;
		
	case 176 : ret = " SecuritySettlAgentName" ; break;
		
	case 400 : ret = " BidDescriptor" ; break;
		
	case 1595 : ret = " RelationshipRiskSecurityAltIDSource" ; break;
		
	case 1500 : ret = " MDStreamID" ; break;
		
	case 1499 : ret = " NoAsgnReqs" ; break;
		
	case 1451 : ret = " NotionalPercentageOutstanding" ; break;
		
	case 778 : ret = " NoSettlInst" ; break;
		
	case 777 : ret = " SettlInstMsgID" ; break;
		
	case 121 : ret = " ForexReq" ; break;
		
	case 335 : ret = " TradSesReqID" ; break;
		
	case 1340 : ret = " UnderlyingLegStrikePrice" ; break;
		
	case 1209 : ret = " TickRuleType" ; break;
		
	case 1049 : ret = " InstrmtAssignmentMethod" ; break;
		
	case 842 : ret = " DiscretionOffsetType" ; break;
		
	case 666 : ret = " ConfirmTransType" ; break;
		
	case 237 : ret = " TotalTakedown" ; break;
		
	case 726 : ret = " ResponseDestination" ; break;
		
	case 1178 : ret = " MDSecSizeType" ; break;
		
	case 1054 : ret = " InstrumentPartySubIDType" ; break;
		
	case 1001 : ret = " LegTimeUnit" ; break;
		
	case 830 : ret = " TransferReason" ; break;
		
	case 812 : ret = " ApplQueueMax" ; break;
		
//	case 389 : ret = " DiscretionOffsetValue" ; break;
		
	case 466 : ret = " BookingRefID" ; break;
		
	case 681 : ret = " LegBidPx" ; break;
		
	case 1525 : ret = " ContextPartyRole" ; break;
		
	case 1368 : ret = " TradSesEvent" ; break;
		
	case 1246 : ret = " DerivativeProduct" ; break;
		
	case 1119 : ret = " RootPartyRole" ; break;
		
	case 787 : ret = " DlvyInstType" ; break;
		
//	case 33 : ret = " NoLinesOfText" ; break;
		
	case 710 : ret = " PosReqID" ; break;
		
	case 606 : ret = " LegSecurityAltIDSource" ; break;
		
	case 357 : ret = " EncodedSubject" ; break;
		
	case 375 : ret = " ContraBroker" ; break;
		
	case 277 : ret = " TradeCondition" ; break;
		
	case 448 : ret = " PartyID" ; break;
		
	case 278 : ret = " MDEntryID" ; break;
		
	case 1341 : ret = " UnderlyingLegSecurityExchange" ; break;
		
	case 1306 : ret = " PriceLimitType" ; break;
		
	case 1105 : ret = " TriggerSecurityIDSource" ; break;
		
	case 1062 : ret = " NoUndlyInstrumentPartySubIDs" ; break;
		
	case 391 : ret = " ClientBidID" ; break;
		
	case 451 : ret = " NetChgPrevDay" ; break;
		
	case 1137 : ret = " DefaultApplVerID" ; break;
		
//	case 23 : ret = " IOIID" ; break;
		
//	case 218 : ret = " SpreadToBenchmark" ; break;
		
	case 13 : ret = " CommType" ; break;
		
	case 507 : ret = " RegistRejReasonCode" ; break;
		
	case 1619 : ret = " RelationshipRiskEncodedSecurityDesc" ; break;
		
	case 1604 : ret = " RelationshipRiskRestructuringType" ; break;
		
	case 962 : ret = " SideTimeInForce" ; break;
		
	case 769 : ret = " TrdRegTimestamp" ; break;
		
	case 291 : ret = " FinancialStatus" ; break;
		
	case 897 : ret = " NoTrades" ; break;
		
	case 893 : ret = " LastFragment" ; break;
		
	case 523 : ret = " PartySubID" ; break;
		
	case 80 : ret = " AllocQty" ; break;
		
	case 208 : ret = " NotifyBrokerOfCredit" ; break;
		
	case 1013 : ret = " SideTrdRegTimestampType" ; break;
		
	case 915 : ret = " AgreementDate" ; break;
		
	case 803 : ret = " PartySubIDType" ; break;
		
	case 900 : ret = " TotalNetValue" ; break;
		
	case 857 : ret = " AllocNoOrdersType" ; break;
		
	case 196 : ret = " AllocLinkID" ; break;
		
	case 469 : ret = " RoundingModulus" ; break;
		
	case 115 : ret = " OnBehalfOfCompID" ; break;
		
	case 309 : ret = " UnderlyingSecurityID" ; break;
		
	case 1160 : ret = " SettlObligMsgID" ; break;
		
	case 970 : ret = " PositionLimit" ; break;
		
	case 858 : ret = " SharedCommission" ; break;
		
	case 765 : ret = " AllowableOneSidednessPct" ; break;
		
	case 161 : ret = " AllocText" ; break;
		
	case 16 : ret = " EndSeqNo" ; break;
		
	case 453 : ret = " NoPartyIDs" ; break;
		
	case 382 : ret = " NoContraBrokers" ; break;
		
	case 197 : ret = " AllocLinkType" ; break;
		
	case 972 : ret = " UnderlyingAllocationPercent" ; break;
		
	case 742 : ret = " AllocAccruedInterestAmt" ; break;
		
	case 1548 : ret = " RiskSecuritySubType" ; break;
		
	case 1469 : ret = " EncodedSecurityListDesc" ; break;
		
	case 1401 : ret = " EncryptedPasswordLen" ; break;
		
	case 1381 : ret = " LegDividendYield" ; break;
		
	case 1187 : ret = " RefreshIndicator" ; break;
		
	case 1155 : ret = " SideSettlCurrency" ; break;
		
	case 975 : ret = " UnderlyingSettlementType" ; break;
		
	case 863 : ret = " OrderCapacityQty" ; break;
		
	case 704 : ret = " LongQty" ; break;
		
	case 1516 : ret = " NoPartyAltIDs" ; break;
		
	case 1317 : ret = " DerivativeSettlMethod" ; break;
		
	case 1113 : ret = " TriggerTradingSessionID" ; break;
		
	case 1084 : ret = " DisplayMethod" ; break;
		
	case 83 : ret = " RptSeq" ; break;
		
	case 282 : ret = " MDEntryOriginator" ; break;
		
	case 599 : ret = " LegInstrRegistry" ; break;
		
	case 1365 : ret = " FillQty" ; break;
		
	case 1096 : ret = " PegSecurityIDSource" ; break;
		
	case 1079 : ret = " MaturityTime" ; break;
		
	case 1022 : ret = " MDFeedType" ; break;
		
	case 910 : ret = " CollStatus" ; break;
		
	case 763 : ret = " UnderlyingSecuritySubType" ; break;
		
	case 1129 : ret = " CstmApplVerID" ; break;
		
	case 1407 : ret = " DefaultApplExtID" ; break;
		
	case 1218 : ret = " NoDerivativeSecurityAltID" ; break;
		
	case 401 : ret = " SideValueInd" ; break;
		
	case 355 : ret = " EncodedText" ; break;
		
	case 1 : ret = " Account" ; break;
		
	case 1110 : ret = " TriggerNewPrice" ; break;
		
	case 1061 : ret = " UndlyInstrumentPartyRole" ; break;
		
	case 385 : ret = " MsgDirection" ; break;
		
	case 611 : ret = " LegMaturityDate" ; break;
		
	case 1437 : ret = " UnderlyingContractMultiplierUnit" ; break;
		
	case 979 : ret = " InputSource" ; break;
		
	case 279 : ret = " MDUpdateAction" ; break;
		
	case 573 : ret = " MatchStatus" ; break;
		
	case 1446 : ret = " RateSource" ; break;
		
	case 1047 : ret = " AllocPositionEffect" ; break;
		
	case 447 : ret = " PartyIDSource" ; break;
		
	case 363 : ret = " EncodedUnderlyingIssuer" ; break;
		
	case 1508 : ret = " NoRequestedPartyRoles" ; break;
		
	case 1402 : ret = " EncryptedPassword" ; break;
		
	case 1112 : ret = " TriggerNewQty" ; break;
		
	case 1073 : ret = " LegLastForwardPoints" ; break;
		
	case 748 : ret = " TotNumTradeReports" ; break;
		
	case 1130 : ret = " RefApplVerID" ; break;
		
	case 194 : ret = " LastSpotRate" ; break;
		
	case 44 : ret = " Price" ; break;
		
//	case 305 : ret = " UnderlyingSecurityIDSource" ; break;
		
//	case 557 : ret = " TotNoSecurityTypes" ; break;
		
	case 1612 : ret = " RelationshipRiskInstrumentMultiplier" ; break;
		
	case 1534 : ret = " NoRiskInstruments" ; break;
		
	case 861 : ret = " ReportedPx" ; break;
		
	case 600 : ret = " LegSymbol" ; break;
		
	case 617 : ret = " LegIssuer" ; break;
		
//	case 509 : ret = " RegistDetls" ; break;
		
	case 1332 : ret = " UnderlyingLegSecurityID" ; break;
		
	case 1231 : ret = " MinLotSize" ; break;
		
	case 395 : ret = " NumTickets" ; break;
		
	case 598 : ret = " LegLocaleOfIssue" ; break;
		
	case 411 : ret = " ExchangeForPhysical" ; break;
		
	case 1174 : ret = " SecurityTradingEvent" ; break;
		
	case 1146 : ret = " MinPriceIncrementAmount" ; break;
		
	case 985 : ret = " UnderlyingPayAmount" ; break;
		
	case 785 : ret = " SettlPartySubID" ; break;
		
	case 154 : ret = " AllocNetMoney" ; break;
		
	case 314 : ret = " UnderlyingMaturityDay" ; break;
		
	case 932 : ret = " NetworkResponseID" ; break;
		
	case 417 : ret = " NumBidders" ; break;
		
	case 661 : ret = " AllocAcctIDSource" ; break;
		
	case 153 : ret = " AllocAvgPx" ; break;
		
	case 177 : ret = " SecuritySettlAgentCode" ; break;
		
	case 510 : ret = " NoDistribInsts" ; break;
		
	case 1029 : ret = " CustDirectedOrder" ; break;
		
	case 406 : ret = " FairValue" ; break;
		
	case 428 : ret = " NoStrikes" ; break;
		
	case 1468 : ret = " EncodedSecurityListDescLen" ; break;
		
	case 1420 : ret = " LegExerciseStyle" ; break;
		
	case 1215 : ret = " DerivativeSymbolSfx" ; break;
		
	case 1210 : ret = " NestedInstrAttribType" ; break;
		
	case 337 : ret = " ContraTrader" ; break;
		
	case 1557 : ret = " RiskInstrumentSettlType" ; break;
		
	case 1179 : ret = " MDSecSize" ; break;
		
	case 1177 : ret = " NoOfSecSizes" ; break;
		
	case 944 : ret = " CollAction" ; break;
		
	case 652 : ret = " UnderlyingLastQty" ; break;
		
	case 43 : ret = " PossDupFlag" ; break;
		
	case 429 : ret = " ListStatusType" ; break;
		
	case 1006 : ret = " SideFillStationCd" ; break;
		
	case 929 : ret = " StatusText" ; break;
		
	case 259 : ret = " BasisFeatureDate" ; break;
		
	case 212 : ret = " XmlDataLen" ; break;
		
	case 542 : ret = " UnderlyingMaturityDate" ; break;
		
	case 123 : ret = " GapFillFlag" ; break;
		
	case 1406 : ret = " RefApplExtID" ; break;
		
	case 1355 : ret = " RefApplID" ; break;
		
	case 1309 : ret = " NoTradingSessionRules" ; break;
		
	case 1069 : ret = " SwapPoints" ; break;
		
	case 848 : ret = " TargetStrategyParameters" ; break;
		
	case 195 : ret = " LastForwardPoints" ; break;
		
	case 696 : ret = " YieldRedemptionDate" ; break;
		
	case 1591 : ret = " RelationshipRiskSecurityID" ; break;
		
	case 1158 : ret = " NoSettlDetails" ; break;
		
	case 1123 : ret = " TradeHandlingInstr" ; break;
		
	case 183 : ret = " CashSettlAgentCode" ; break;
		
	case 686 : ret = " LegPriceType" ; break;
		
	case 352 : ret = " EncodedListExecInstLen" ; break;
		
	case 338 : ret = " TradSesMethod" ; break;
		
	case 1532 : ret = " RiskLimitCurrency" ; break;
		
	case 1505 : ret = " PartyDetailsListRequestID" ; break;
		
	case 914 : ret = " AgreementID" ; break;
		
	case 478 : ret = " CashDistribCurr" ; break;
		
	case 132 : ret = " BidPx" ; break;
		
//	case 418 : ret = " TradeType" ; break;
		
	case 350 : ret = " EncodedSecurityDescLen" ; break;
		
	case 1490 : ret = " ComplexEventCondition" ; break;
		
	case 1400 : ret = " EncryptedPasswordMethod" ; break;
		
	case 1219 : ret = " DerivativeSecurityAltID" ; break;
		
	case 1169 : ret = " TotNoAccQuotes" ; break;
		
	case 943 : ret = " TimeBracket" ; break;
		
	case 78 : ret = " NoAllocs" ; break;
		
	case 462 : ret = " UnderlyingProduct" ; break;
		
	case 221 : ret = " BenchmarkCurveName" ; break;
		
	case 312 : ret = " UnderlyingSymbolSfx" ; break;
		
	case 1480 : ret = " StrikePriceBoundaryPrecision" ; break;
		
	case 302 : ret = " QuoteSetID" ; break;
		
	case 544 : ret = " CashMargin" ; break;
		
	case 1162 : ret = " SettlObligTransType" ; break;
		
	case 1152 : ret = " LegNumber" ; break;
		
	case 1035 : ret = " DeskOrderHandlingInst" ; break;
		
	case 783 : ret = " SettlPartyIDSource" ; break;
		
	case 734 : ret = " PriorSettlPrice" ; break;
		
	case 1600 : ret = " RelationshipRiskSecurityType" ; break;
		
	case 1372 : ret = " NotAffOrigClOrdID" ; break;
		
	case 1326 : ret = " TradingSessionDesc" ; break;
		
	case 1322 : ret = " DerivativeFloorPrice" ; break;
		
	case 1214 : ret = " DerivativeSymbol" ; break;
		
	case 1190 : ret = " RiskFreeRate" ; break;
		
	case 709 : ret = " PosTransType" ; break;
		
	case 34 : ret = " MsgSeqNum" ; break;
		
	case 89 : ret = " Signature" ; break;
		
	case 1450 : ret = " Seniority" ; break;
		
	case 1445 : ret = " NoRateSources" ; break;
		
	case 1192 : ret = " PriceUnitOfMeasureQty" ; break;
		
	case 907 : ret = " CollAsgnRefID" ; break;
		
	case 330 : ret = " BuyVolume" ; break;
		
	case 156 : ret = " SettlCurrFxRateCalc" ; break;
		
	case 722 : ret = " PosMaintStatus" ; break;
		
	case 720 : ret = " PriorSpreadIndicator" ; break;
		
	case 219 : ret = " Benchmark" ; break;
		
	case 1303 : ret = " MaturityMonthYearFormat" ; break;
		
	case 822 : ret = " UnderlyingTradingSessionID" ; break;
		
//	case 393 : ret = " TotNoRelatedSym" ; break;
		
	case 471 : ret = " StateOrProvinceOfIssue" ; break;
		
	case 1573 : ret = " RelatedPartyAltSubID" ; break;
		
	case 1257 : ret = " DerivativeInstrRegistry" ; break;
		
	case 1067 : ret = " LegBidForwardPoints" ; break;
		
	case 1028 : ret = " ManualOrderIndicator" ; break;
		
	case 118 : ret = " NetMoney" ; break;
		
	case 650 : ret = " LegalConfirm" ; break;
		
	case 470 : ret = " CountryOfIssue" ; break;
		
	case 1426 : ret = " ApplReportType" ; break;
		
	case 1117 : ret = " RootPartyID" ; break;
		
	case 879 : ret = " UnderlyingQty" ; break;
		
	case 813 : ret = " ApplQueueDepth" ; break;
		
	case 99 : ret = " StopPx" ; break;
		
	case 113 : ret = " ReportToExch" ; break;
		
	case 719 : ret = " ContraryInstructionIndicator" ; break;
		
	case 445 : ret = " EncodedListStatusTextLen" ; break;
		
	case 1284 : ret = " DerivativeSecurityXMLSchema" ; break;
		
	case 146 : ret = " NoRelatedSym" ; break;
		
	case 88 : ret = " AllocRejCode" ; break;
		
	case 458 : ret = " UnderlyingSecurityAltID" ; break;
		
	case 1593 : ret = " NoRelationshipRiskSecurityAltID" ; break;
		
	case 1431 : ret = " RefOrdIDReason" ; break;
		
	case 1293 : ret = " DerivativeInstrumentPartyID" ; break;
		
	case 1186 : ret = " SecurityXMLSchema" ; break;
		
	case 1081 : ret = " RefOrderIDSource" ; break;
		
	case 971 : ret = " NTPositionLimit" ; break;
		
	case 920 : ret = " EndAccruedInterestAmt" ; break;
		
	case 158 : ret = " AccruedInterestRate" ; break;
		
	case 29 : ret = " LastCapacity" ; break;
		
	case 1585 : ret = " RelationshipRiskLimitCurrency" ; break;
		
	case 1063 : ret = " UnderlyingInstrumentPartySubID" ; break;
		
	case 1362 : ret = " NoFills" ; break;
		
	case 1237 : ret = " NoOrdTypeRules" ; break;
		
	case 1019 : ret = " InstrumentPartyID" ; break;
		
	case 898 : ret = " MarginRatio" ; break;
		
	case 371 : ret = " RefTagID" ; break;
		
	case 215 : ret = " NoRoutingIDs" ; break;
		
	case 223 : ret = " CouponRate" ; break;
		
	case 1351 : ret = " NoApplIDs" ; break;
		
	case 1285 : ret = " DerivativeContractSettlMonth" ; break;
		
	case 871 : ret = " InstrAttribType" ; break;
		
	case 460 : ret = " Product" ; break;
		
//	case 80 : ret = " AllocShares" ; break;
		
	case 295 : ret = " NoQuoteEntries" ; break;
		
	case 1615 : ret = " RelationshipRiskWarningLevelName" ; break;
		
	case 1408 : ret = " DefaultCstmApplVerID" ; break;
		
	case 1320 : ret = " DerivativeListMethod" ; break;
		
	case 1282 : ret = " DerivativeSecurityXMLLen" ; break;
		
	case 739 : ret = " LegDatedDate" ; break;
		
	case 758 : ret = " Nested2PartyIDSource" ; break;
		
	case 595 : ret = " UnderlyingInstrRegistry" ; break;
		
	case 225 : ret = " IssueDate" ; break;
		
	case 326 : ret = " SecurityTradingStatus" ; break;
		
	case 613 : ret = " LegOptAttribute" ; break;
		
	case 111 : ret = " MaxFloor" ; break;
		
	case 1260 : ret = " DerivativeLocaleOfIssue" ; break;
		
//	case 1195 : ret = " OptPayAmount" ; break;
		
	case 888 : ret = " UnderlyingStipType" ; break;
		
	case 47 : ret = " Rule80A" ; break;
		
	case 422 : ret = " TotNoStrikes" ; break;
		
	case 292 : ret = " CorporateAction" ; break;
		
	case 788 : ret = " TerminationType" ; break;
		
	case 615 : ret = " LegCouponRate" ; break;
		
	case 712 : ret = " PosMaintAction" ; break;
		
	case 558 : ret = " NoSecurityTypes" ; break;
		
	case 1489 : ret = " ComplexEventPriceTimeType" ; break;
		
	case 1071 : ret = " LastSwapPoints" ; break;
		
	case 1046 : ret = " UnderlyingFXRateCalc" ; break;
		
	case 444 : ret = " ListStatusText" ; break;
		
	case 575 : ret = " OddLot" ; break;
		
	case 590 : ret = " BookingUnit" ; break;
		
	case 674 : ret = " LegAllocAcctIDSource" ; break;
		
	case 370 : ret = " OnBehalfOfSendingTime" ; break;
		
	case 87 : ret = " AllocStatus" ; break;
		
	case 1448 : ret = " ReferencePage" ; break;
		
	case 1299 : ret = " DerivativeExerciseStyle" ; break;
		
	case 1182 : ret = " ApplBegSeqNum" ; break;
		
	case 908 : ret = " CollRptID" ; break;
		
	case 416 : ret = " IncTaxInd" ; break;
		
	case 398 : ret = " NoBidDescriptors" ; break;
		
	case 248 : ret = " LegCouponPaymentDate" ; break;
		
	case 1512 : ret = " TotNoPartyList" ; break;
		
	case 1507 : ret = " PartyListResponseType" ; break;
		
	case 1334 : ret = " NoUnderlyingLegSecurityAltID" ; break;
		
	case 700 : ret = " ReversalIndicator" ; break;
		
	case 10 : ret = " CheckSum" ; break;
		
	case 57 : ret = " TargetSubID" ; break;
		
	case 729 : ret = " PosReqStatus" ; break;
		
	case 638 : ret = " PriorityIndicator" ; break;
		
	case 1528 : ret = " ContextPartySubIDType" ; break;
		
	case 1344 : ret = " UnderlyingLegCFICode" ; break;
		
	case 1271 : ret = " DerivativeTimeUnit" ; break;
		
	case 948 : ret = " NoNested3PartyIDs" ; break;
		
	case 403 : ret = " LiquidityPctHigh" ; break;
		
	case 481 : ret = " MoneyLaunderingStatus" ; break;
		
	case 1412 : ret = " Nested4PartySubID" ; break;
		
	case 1272 : ret = " DerivativeSecurityExchange" ; break;
		
	case 1093 : ret = " LotType" ; break;
		
	case 977 : ret = " ContIntRptID" ; break;
		
	case 276 : ret = " QuoteCondition" ; break;
		
	case 1495 : ret = " ComplexEventStartTime" ; break;
		
	case 1483 : ret = " NoComplexEvents" ; break;
		
	case 1266 : ret = " DerivativeContractMultiplier" ; break;
		
	case 1256 : ret = " DerivativeSecurityStatus" ; break;
		
	case 1228 : ret = " DerivativeProductComplex" ; break;
		
	case 1103 : ret = " TriggerSymbol" ; break;
		
	case 594 : ret = " UnderlyingLocaleOfIssue" ; break;
		
	case 52 : ret = " SendingTime" ; break;
		
	case 1602 : ret = " RelationshipRiskMaturityMonthYear" ; break;
		
	case 1571 : ret = " RelatedPartyAltIDSource" ; break;
		
	case 1492 : ret = " ComplexEventStartDate" ; break;
		
	case 1453 : ret = " UnderlyingRestructuringType" ; break;
		
	case 1224 : ret = " LegUnitOfMeasureQty" ; break;
		
	case 768 : ret = " NoTrdRegTimestamps" ; break;
		
	case 51 : ret = " SendingDate" ; break;
		
	case 1515 : ret = " PartyRelationship" ; break;
		
	case 1189 : ret = " TimeToExpiration" ; break;
		
	case 673 : ret = " LegAllocQty" ; break;
		
	case 166 : ret = " SettlLocation" ; break;
		
	case 1419 : ret = " UnderlyingExerciseStyle" ; break;
		
	case 186 : ret = " CashSettlAgentContactName" ; break;
		
	case 252 : ret = " LegRepurchaseRate" ; break;
		
	case 1353 : ret = " ApplResponseID" ; break;
		
	case 1311 : ret = " NoDerivativeInstrAttrib" ; break;
		
	case 1263 : ret = " DerivativeStrikeMultiplier" ; break;
		
	case 942 : ret = " LegStrikeCurrency" ; break;
		
	case 324 : ret = " SecurityStatusReqID" ; break;
		
	case 90 : ret = " SecureDataLen" ; break;
		
	case 846 : ret = " DiscretionScope" ; break;
		
	case 522 : ret = " OwnerType" ; break;
		
//	case 53 : ret = " Shares" ; break;
		
	case 236 : ret = " Yield" ; break;
		
	case 693 : ret = " QuoteRespID" ; break;
		
	case 1549 : ret = " RiskMaturityMonthYear" ; break;
		
	case 953 : ret = " Nested3PartySubID" ; break;
		
	case 814 : ret = " ApplQueueResolution" ; break;
		
	case 771 : ret = " TrdRegTimestampOrigin" ; break;
		
	case 759 : ret = " Nested2PartyRole" ; break;
		
	case 757 : ret = " Nested2PartyID" ; break;
		
	case 134 : ret = " BidSize" ; break;
		
	case 601 : ret = " LegSymbolSfx" ; break;
		
	case 301 : ret = " QuoteResponseLevel" ; break;
		
	case 9 : ret = " BodyLength" ; break;
		
	case 69 : ret = " ListExecInst" ; break;
		
	case 1036 : ret = " ExecAckStatus" ; break;
		
//	case 193 : ret = " SettlDate2" ; break;
		
	case 430 : ret = " NetGrossInd" ; break;
		
	case 459 : ret = " UnderlyingSecurityAltIDSource" ; break;
		
	case 112 : ret = " TestReqID" ; break;
		
	case 125 : ret = " CxlType" ; break;
		
	case 256 : ret = " UnderlyingCreditRating" ; break;
		
//	case 74 : ret = " AvgPxPrecision" ; break;
		
	case 663 : ret = " BenchmarkPriceType" ; break;
		
	case 1034 : ret = " DeskTypeSource" ; break;
		
	case 844 : ret = " DiscretionRoundDirection" ; break;
		
	case 1127 : ret = " OrigSecondaryTradeID" ; break;
		
	case 1030 : ret = " ReceivedDeptID" ; break;
		
	case 890 : ret = " MaturityNetMoney" ; break;
		
	case 399 : ret = " BidDescriptorType" ; break;
		
	case 1620 : ret = " RiskEncodedSecurityDescLen" ; break;
		
	case 1589 : ret = " RelationshipRiskSymbol" ; break;
		
	case 1575 : ret = " NoRelatedContextPartyIDs" ; break;
		
	case 1297 : ret = " DerivativeInstrumentPartySubID" ; break;
		
	case 937 : ret = " NetworkStatusResponseType" ; break;
		
	case 486 : ret = " DateOfBirth" ; break;
		
	case 1581 : ret = " RelatedContextPartySubIDType" ; break;
		
	case 1202 : ret = " StartStrikePxRange" ; break;
		
//	case 1063 : ret = " UndlyInstrumentPartySubID" ; break;
		
	case 881 : ret = " SecondaryTradeReportRefID" ; break;
		
	case 878 : ret = " UnderlyingCPRegType" ; break;
		
	case 93 : ret = " SignatureLength" ; break;
		
	case 38 : ret = " OrderQty" ; break;
		
	case 1614 : ret = " RelationshipRiskWarningLevelPercent" ; break;
		
	case 1452 : ret = " OriginalNotionalPercentageOutstanding" ; break;
		
	case 1000 : ret = " UnderlyingTimeUnit" ; break;
		
	case 358 : ret = " EncodedHeadlineLen" ; break;
		
	case 473 : ret = " NoRegistDtls" ; break;
		
	case 960 : ret = " StrategyParameterValue" ; break;
		
	case 1556 : ret = " RiskSecurityDesc" ; break;
		
	case 1018 : ret = " NoInstrumentParties" ; break;
		
	case 537 : ret = " QuoteType" ; break;
		
	case 1540 : ret = " NoRiskSecurityAltID" ; break;
		
	case 957 : ret = " NoStrategyParameters" ; break;
		
	case 776 : ret = " IndividualAllocRejCode" ; break;
		
	case 388 : ret = " DiscretionInst" ; break;
		
	case 1541 : ret = " RiskSecurityAltID" ; break;
		
	case 1464 : ret = " TargetPartyRole" ; break;
		
	case 550 : ret = " CrossPrioritization" ; break;
		
	case 446 : ret = " EncodedListStatusText" ; break;
		
	case 24 : ret = " IOIOthSvc" ; break;
		
	case 249 : ret = " LegIssueDate" ; break;
		
	case 281 : ret = " MDReqRejReason" ; break;
		
	case 1606 : ret = " RelationshipRiskPutOrCall" ; break;
		
	case 1347 : ret = " ApplReqType" ; break;
		
	case 421 : ret = " Country" ; break;
		
	case 1333 : ret = " UnderlyingLegSecurityIDSource" ; break;
		
	case 1242 : ret = " FlexProductEligibilityIndicator" ; break;
		
	case 1057 : ret = " AggressorIndicator" ; break;
		
	case 485 : ret = " ExecPriceAdjustment" ; break;
		
	case 380 : ret = " BusinessRejectReason" ; break;
		
	case 75 : ret = " TradeDate" ; break;
		
	case 315 : ret = " UnderlyingPutOrCall" ; break;
		
	case 1590 : ret = " RelationshipRiskSymbolSfx" ; break;
		
//	case 1061 : ret = " UnderlyingInstrumentPartyRole" ; break;
		
	case 1273 : ret = " DerivativePositionLimit" ; break;
		
	case 994 : ret = " TierCode" ; break;
		
	case 775 : ret = " BookingType" ; break;
	case 234 : ret = " StipulationValue" ; break;
	case 656 : ret = " SettlCurrBidFxRate" ; break;
	default:
		break;
	}
	return ret;
}

