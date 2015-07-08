#pragma warning(disable : 4786)
#include "PriceSource.h"
#include ".\hiredis\hiredis.h"
#include <windows.h>
#include <set>
#include "SPInfor.h"
using namespace std;


class otherPriceSource : public PriceSource    
{
public:
	otherPriceSource();
	~otherPriceSource();  
	virtual void start();
	void ReadIniConfig();
	virtual void add(string ID);
	bool startRedis();
	static void receivePrice(void* pParams); 
	void printReply(redisReply *reply);
	void compute(redisReply *reply);

	CSPPublicFun m_sPPublic;
	CRITICAL_SECTION csPrice;
private:
	bool started;
	redisContext *c;
	redisReply *reply;
	set<string> m_contracts;
};
