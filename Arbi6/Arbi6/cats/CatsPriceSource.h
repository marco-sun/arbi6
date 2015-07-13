#pragma once

#include "../arbi/PriceSource.h"

class CatsPriceSource : public PriceSource  
{
public:
	CatsPriceSource(void);
	virtual ~CatsPriceSource(void);
	virtual	void start(void);
	virtual void add(string contract);

private:
	bool started;

};

