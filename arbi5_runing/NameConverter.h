#if !defined(NAMECONVERTER_H)
#define NAMECONVERTER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)
#include <string>
#include <map>
#include <vector>
#include <windows.h>
using namespace std;

typedef vector<string> strings;

class NameConverter
{
public:
	static NameConverter* getInstance();
	static string& trim_string(string &s, const char *chars = NULL);
	static void divide_string(const char *str, strings &parts, const char *chars = NULL, bool allow_empty_part = false);
	NameConverter(void);
	virtual ~NameConverter(void);
	void init(void);
	string base2RohonName(string baseName);
	string rohon2BaseName(string rohonName);

private:
	static NameConverter* m_pNC;
	map<string,string> rohonMonthMap;
	map<string,string> rohonRevertMonthMap;
	CRITICAL_SECTION cs;
	map<string, string> rohonMap;
	map<string, string> rohonRevertMap;
};

#endif
