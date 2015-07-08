#pragma once

#include <list>
using namespace std;

class ImportExpression
{
public:
	ImportExpression(void);
	ImportExpression(string commodity, double refProfit, double a, double b, double c, double d, double e, double f, double g, double h);
	~ImportExpression(void);
	static list<ImportExpression *> listAll();
	static void add(string commodity, double refProfit, double a, double b, double c, double d, double e, double f, double g, double h);
	double calc(double overseaPrice, double domestic);
	double calc(double domestic1, double domestic2, double domestic);
	static ImportExpression * find(string commodity);
	static ImportExpression * find2(string commodity);
	string getCommodity();
private:
	static list<ImportExpression *> l;
	string commodity;
	double refProfit;
	double a;
	double b;
	double c;
	double d;
	double e;
	double f;
	double g;
	double h;
};

