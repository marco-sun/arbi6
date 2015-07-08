#include "Dealer.h"
#include <windows.h>
#include <map>
#include ".\hiredis\hiredis.h"
#include "SPInfor.h"
using namespace std;

class otherDealerInventory
{
public:
	otherDealerInventory(Order *pOrder);
	virtual ~otherDealerInventory();

	Order *order;
	char sysID[64];
	int localRef;
	int remoteRef; //nIntOrderNo
	int placeStatus; // 0 -- new order; 1 -- received by remote; 2 -- partial dealed; 3 -- all dealed; 4 -- canceled; 5 -- aborted.
	int dealedLot;
};

class otherDealer : public Dealer
{
public:
	otherDealer();
	~otherDealer();
	void start();
	void stop();
	virtual void placeOrder(Order *order);
	virtual void cancelOrder(Order *order);
	void ReadIniConfig();
	bool startRedis();
	static void receiveOrderReport(void* pParams); 
	void printReply(redisReply *reply);
	void compute(char *reply);
	int getCurOrderRef();

	void redisStart();
	void redisPause();
	void redisStop();
private:
	int  startTcp();
	int  closeTcp();
	HANDLE hLogin;
	redisContext *c;
	redisContext *w;
	redisReply *reply;
	bool started;
	CRITICAL_SECTION cs;
	map<string, otherDealerInventory*> dealerInventoryMap;
	int iNextOrderRef; 
	CSPPublicFun m_sPPublic;

	otherDealerInventory * addInventory(Order *order);
	bool delInventory(otherDealerInventory * pDealerInventory);
	otherDealerInventory * getInventory(string key);
	otherDealerInventory * findInventorybyLocal(int localRef);
	otherDealerInventory * findInventorybyRemote(int remoteRef);
	otherDealerInventory * findInventorybySysID(const char * ID);
};