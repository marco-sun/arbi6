// Utils.h: interface for the Leg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(UTILS_H)
#define UTILS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
using namespace std;

class Utils
{
public:
	static bool doubleEqual(double a, double b);
	static string intToString(int i);
	static string doubleToString(double i);
};

#endif // !defined(UTILS_H)
