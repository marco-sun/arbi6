
#include "ImportExpression.h"
#include "Persistence.h"
#include <assert.h>


ImportExpression::ImportExpression(void)
{
}

ImportExpression::ImportExpression(string commodity, double refProfit, double a, double b, double c, double d, double e, double f, double g, double h)
{
	this->commodity = commodity;
	this->refProfit = refProfit;
	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;
	this->e = e;
	this->f = f;
	this->g = g;
	this->h = h;
}

ImportExpression::~ImportExpression(void)
{
}

list<ImportExpression *> ImportExpression::l;

list<ImportExpression *> ImportExpression::listAll()
{
	Persistence::loadAsNecessary();
	return l;
}

bool startWith(const char * s1, const char * s2);

ImportExpression * ImportExpression::find(string commodity)
{
	ImportExpression * ip = NULL;
    list<ImportExpression *>::iterator the_iterator = l.begin();

    while( the_iterator != l.end() ) {
        if(startWith(commodity.c_str(), (*the_iterator)->commodity.c_str())) ip = (*the_iterator);
        the_iterator++;
	}
	return ip;
}
ImportExpression * ImportExpression::find2(string commodity)
{
	ImportExpression * ip = NULL;
    list<ImportExpression *>::iterator the_iterator = l.begin();

    while( the_iterator != l.end() ) {
        if(startWith((*the_iterator)->commodity.c_str(), commodity.c_str())) ip = (*the_iterator);
        the_iterator++;
	}
	return ip;
}

double ImportExpression::calc(double overseaPrice, double domestic)
{
	if (startWith(commodity.c_str(),"CT"))
		return domestic - (((((overseaPrice + c) * b) + d) * a + e) * (1 + f) * (1 + g) + h);
	return domestic - ((((overseaPrice + c) * b) + d) * a * (1 + e) * (1 + f) * (1 + g) + h);
}

double ImportExpression::calc(double domestic1, double domestic2, double domestic3)
{
	return domestic1 * a + domestic2 * b - domestic3 * c - d;
}

void ImportExpression::add(string commodity, double refProfit, double a, double b, double c, double d, double e, double f, double g, double h)
{
	ImportExpression * p = new ImportExpression(commodity, refProfit, a, b, c, d, e, f, g, h);
	l.push_back(p);
}

string ImportExpression::getCommodity()
{
	return commodity;
}