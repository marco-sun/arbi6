#pragma   warning   (disable:   4786) 

#include "SPInfor.h"

string& CSPPublicFun::trim_string(string &s, const char *chars/*=NULL*/)
{

	if (chars == NULL)
		chars = " \t\r\n";
	while ((s.length() > 0) && (::strchr(chars, *s.begin())))
		s.erase(s.begin());
	while ((s.length() > 0) && (::strchr(chars, *s.rbegin())))
		s.erase((s.end())-1);

	return s;
}

void CSPPublicFun::divide_string(const char *str, strings &parts, const char *chars/*=NULL*/, bool allow_empty_part/*=FALSE*/)
{
	if (! chars)
		chars = " \t";

	string s = str;
	parts.clear();

	while (s.length())
	{
		int i;
		string v;

		i = s.find_first_of(chars);
		if (i != string::npos)
		{
			v.assign(s, 0, i);
			s.erase(0, i+1);
		}
		else
		{
			v = s;
			s.erase();
		}

		trim_string(v);
		if ((v.length() > 0) || allow_empty_part)
			parts.push_back(v);
	}
}
