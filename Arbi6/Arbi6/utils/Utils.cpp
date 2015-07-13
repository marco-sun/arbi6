// Utils.cpp: implementation of the Utils class.
//
//////////////////////////////////////////////////////////////////////

#include "Utils.h"

#define EPSON 0.00001


bool Utils::doubleEqual(double a, double b)
{
	return a - b > -EPSON && a - b < EPSON;
}

string Utils::intToString(int i)
{
    char s1[25];
    _itoa_s(i, s1, 25, 10);
	string s = "";
	s.append(s1);
    return s;
}

string Utils::doubleToString(double i)
{
	char temp[1000];
	sprintf_s(temp, 1000, "%.2f", i );
	return temp;
	
}
