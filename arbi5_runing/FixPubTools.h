#pragma once
#include <time.h>
#pragma warning(disable : 4786)
#include <string>
#include <vector>
#include <list>
using namespace std;
typedef vector<string> strings;
class FixPubTools
{
public:
	static string& trim_string(string &s, const char *chars = NULL);
	static	void divide_string(const char *str, strings &parts, const char *chars = NULL, bool allow_empty_part = false);

};



