#include "stdafx.h"
#include "otherPriceSource.h"
#include "LogStore.h"
#include <winsock2.h>
#include <process.h>
#include "Contract.h"
#include <cstdlib>

otherPriceSource::otherPriceSource()
{
	m_contracts.clear();
	started = false;
	ReadIniConfig();
	InitializeCriticalSection(&csPrice);
}

otherPriceSource::~otherPriceSource()
{
	DeleteCriticalSection(&csPrice);
}

void otherPriceSource::start()
{
	TRACE_LOG("启动 otherPriceSource.....");
    if(started) {
	    TRACE_LOG("otherPriceSource already started.");
	    return;
	}
	if (startRedis())
	{
		TRACE_LOG("Redis服务器连接成功");
	}
	else
	{
		TRACE_LOG("Redis服务器连接失败");
		exit(1);
	}
	//_beginthread( receive_tcp, 0, (void *)this);
	//_beginthread( price_query, 0, (void *)this);
    started = true;	
	redisAppendCommand(c,"SUBSCRIBE Price-BPTA Price-BCK Price-BRURV Price-BSC Price-BCUSH Price-BRBWX Price-BAG");
	_beginthread(receivePrice, 0, (void *)this);
	TRACE_LOG("otherPriceSource started _beginthread receivePrice");
	return;
}
void otherPriceSource::ReadIniConfig()
{

}
bool otherPriceSource::startRedis()
{
    unsigned int j;
    struct timeval timeout = { 1, 500000 }; // 1.5 seconds

	WSADATA wsaData;
	WSAStartup(MAKEWORD( 2, 2 ), &wsaData);

	c = redisConnect((char*)"192.168.11.105", 6379);
    if (c->err) {
        printf("Connection error: %s\n", c->errstr);
        return false;
    }
	return true;
}

void otherPriceSource::compute(redisReply *reply)
{
	TRACE_LOG("otherPriceSource receive: %s",reply->str);
	strings pars;	
	strings::iterator it;		
	
	Pricing tmpPrice("N/A",0.0,0.0);

	int npar = 0;
	int code = 0;
	time_t now;
	time( &now );
	//TRACE_LOG("PRICE ,%s",pmsg);

	m_sPPublic.divide_string(reply->str , pars ,"," , TRUE);
	char  szProductId[32];
	char *e;
	for(npar = 0,it = pars.begin(); it != pars.end();it++,npar++)
	{
		switch(npar) {
		case 1:
			memset(szProductId, 0, 32);
			strcpy_s(szProductId, 32, (* it).c_str());
			break;
		case 3:
			tmpPrice.bid = strtod((* it).c_str(), &e);
			if(*e != 0) return;
			break;
		case 4:
			tmpPrice.ask = strtod((* it).c_str(), &e);
			if(*e != 0) return;
			break;
		case 2:
			tmpPrice.lastprice = strtod((* it).c_str(), &e);
			if(*e != 0) return;
			break;
		default:
			break;
		}
	}
	tmpPrice.contract = string(szProductId);
	//TRACE_LOG("Price for ,%s",tmpPrice.contract.c_str());
	//inform(tmpPrice);
	Contract::get(tmpPrice.contract)->setPrice(0,tmpPrice.ask,tmpPrice.bid,tmpPrice.lastprice);
}

void otherPriceSource::printReply(redisReply *reply)
{
	int i;
	if(reply == NULL) return;
	if(reply->type == REDIS_REPLY_ARRAY)
	{
		redisReply * r = *(reply->element + 2);
		if(r->type == REDIS_REPLY_STRING) compute(r);
	} 
	else 
	{
		TRACE_LOG("Redis reveive other type\n");
	}
}

void otherPriceSource::receivePrice(void* pParams) 
{
	otherPriceSource * ps = (otherPriceSource*)pParams;
	while(1)
	{
		ps->reply = NULL;
		redisGetReply(ps->c,(void **)&(ps->reply));
		ps->printReply(ps->reply);
		if(ps->reply!=NULL)	freeReplyObject(ps->reply);	
	}
}

void otherPriceSource::add(string ID)
{
	/*
	EnterCriticalSection(&csPrice);
	TRACE_LOG("otherPriceSource Add Id %s.", ID.c_str());
	m_contracts.insert(ID);
	redisAppendCommand(c,"SUBSCRIBE Price-%s",ID.c_str());
	LeaveCriticalSection(&csPrice);
	*/
}
