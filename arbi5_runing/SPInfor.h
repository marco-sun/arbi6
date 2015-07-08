#pragma once
#include <time.h>
#pragma warning(disable : 4786)
#include <string>
#include <vector>


using namespace std;

typedef vector<string> strings;

enum SP_MSG_TYPE
{
    MSGID_USER_LOGIN        = 3101,
	MSGID_USER_LOGOUT		= 3102,
	MSGID_ACCOUNT_LOGIN		= 3121,
	MSGID_ACCOUNT_LOGOUT	= 3122,
	MSGID_ORDER_REQ			= 3103,
	MSGID_CLEAR_TRADE		= 3109,
	MSGID_LOAD_ORDER_REQ	= 3186,
	MSGID_UPDATE_ORDER		= 3119,
	MSGID_LOAD_TRADE_REQ	= 3181,
	MSGID_CONNECTION_STATUS = 9000,
	MSGID_CONNECTION_REQ	= 9001,
	MSGID_TICKER_REQ		= 5107,
//	MSGID_UPDATED_PRICE		= 5102,
	MSGID_PRC_SNAP_REQ		= 4106,
	MSGID_PRC_UPD_REQ		= 4107,
	MSGID_PRC_UPD_REL		= 4108,
	MSGID_UPDATED_PRICE		= 4102
};

enum SP_ORDER_STATUS
{
	SP_WORKING      = 1, 
	SP_INACTIVE		= 2,
	SP_PENDING		= 3,
	SP_ADDING		= 4,
	SP_CHANGING		= 5, 
	SP_DELETING		= 6,
	SP_DEACTIVATING	= 7,
	SP_PARTIAL_TRADED	= 8,
	SP_ALL_TRADED	= 9, 
	SP_DELETED		= 10
};

class CSPPublicFun
{
public:
	string& trim_string(string &s, const char *chars = NULL);
	void divide_string(const char *str, strings &parts, const char *chars = NULL, bool allow_empty_part = false);

};


