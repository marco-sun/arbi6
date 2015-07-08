#include "NameConverter.h"

NameConverter* NameConverter::m_pNC=NULL;

NameConverter* NameConverter::getInstance()
{
	if(m_pNC==NULL) 
	{
		m_pNC = new NameConverter();
		m_pNC->init();
	}
	return m_pNC;

}

string& NameConverter::trim_string(string &s, const char *chars/*=NULL*/)
{
	if (s.length() > 0)
	{
		if (chars == NULL)
			chars = " \t\r\n";
		while ((::strchr(chars, *s.begin())) && (s.length() > 0))
			s.erase(s.begin());
		while ((::strchr(chars, *s.rbegin())) && (s.length() > 0))
			s.erase((s.end())-1);
	}
	return s;
}

void NameConverter::divide_string(const char *str, strings &parts, const char *chars/*=NULL*/, bool allow_empty_part/*=FALSE*/)
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

NameConverter::NameConverter(void)
{
	InitializeCriticalSection(&cs);
	rohonMap.clear();
	rohonRevertMap.clear();
	rohonMonthMap.clear();
	rohonRevertMonthMap.clear();
}


NameConverter::~NameConverter(void)
{
	DeleteCriticalSection(&cs);
}

void NameConverter::init(void)
{
	rohonMonthMap["01"] = "JAN";
	rohonMonthMap["02"] = "FEB";
	rohonMonthMap["03"] = "MAR";
	rohonMonthMap["04"] = "APR";
	rohonMonthMap["05"] = "MAY";
	rohonMonthMap["06"] = "JUN";
	rohonMonthMap["07"] = "JUL";
	rohonMonthMap["08"] = "AUG";
	rohonMonthMap["09"] = "SEP";
	rohonMonthMap["10"] = "OCT";
	rohonMonthMap["11"] = "NOV";
	rohonMonthMap["12"] = "DEC";

	rohonRevertMonthMap["JAN"]="01";
	rohonRevertMonthMap["FEB"]="02";
	rohonRevertMonthMap["MAR"]="03";
	rohonRevertMonthMap["APR"]="04";
	rohonRevertMonthMap["MAY"]="05";
	rohonRevertMonthMap["JUN"]="06";
	rohonRevertMonthMap["JUL"]="07";
	rohonRevertMonthMap["AUG"]="08";
	rohonRevertMonthMap["SEP"]="09";
	rohonRevertMonthMap["OCT"]="10";
	rohonRevertMonthMap["NOV"]="11";
	rohonRevertMonthMap["DEC"]="12";
	// The following will be changed to read info from file later.
	EnterCriticalSection(&cs);
	rohonMap["ZS@CBOT"]="CME_CBT SOYBEAN";
	rohonMap["ZC@CBOT"]="CME_CBT CORN";
	rohonMap["ZM@CBOT"]="CME_CBT SOYMEAL";
	rohonMap["ZL@CBOT"]="CME_CBT SOYOIL";
	rohonMap["ZW@CBOT"]="CME_CBT WHEAT";
	rohonMap["CT@NBOT"]="NYBOT NB COTT";
	rohonMap["SB@NBOT"]="NYBOT NB SU11";
	rohonMap["HG@NYMEX"]="CME CMX COP";
	rohonMap["GC@NYMEX"]="CME CMX GLD";
	rohonMap["CL@NYMEX"]="CME CRUDE";
	rohonMap["PL@NYMEX"]="CME NYM PAL";
	rohonMap["SI@NYMEX"]="CME CMX SIL";
	rohonMap["LCA@LME"]="LME CA";
	rohonMap["LZS@LME"]="LME ZS";
	rohonMap["LAH@LME"]="LME AH";
	rohonMap["SN@LME"]="LME SN";

	rohonRevertMap["CME_CBT SOYBEAN"]="ZS@CBOT";
	rohonRevertMap["CME_CBT CORN"]="ZC@CBOT";
	rohonRevertMap["CME_CBT SOYMEAL"]="ZM@CBOT";
	rohonRevertMap["CME_CBT SOYOIL"]="ZL@CBOT";
	rohonRevertMap["CME_CBT WHEAT"]="ZW@CBOT";
	rohonRevertMap["NYBOT NB COTT"]="CT@NBOT";
	rohonRevertMap["NYBOT NB SU11"]="SB@NBOT";
	rohonRevertMap["CME CMX COP"]="HG@NYMEX";
	rohonRevertMap["CME CMX GLD"]="GC@NYMEX";
	rohonRevertMap["CME CRUDE"]="CL@NYMEX";
	rohonRevertMap["CME NYM PAL"]="PL@NYMEX";
	rohonRevertMap["CME CMX SIL"]="SI@NYMEX";
	rohonRevertMap["LME CA"]="LCA@LME";
	rohonRevertMap["LME ZS"]="LZS@LME";
	rohonRevertMap["LME AH"]="LAH@LME";
	rohonRevertMap["LME SN"]="SN@LME";
	LeaveCriticalSection(&cs);
}


string NameConverter::base2RohonName(string baseName)
{
	string ret = "";
	string comodity="";
	string exchange="";
	string month="";
	strings pars;
	divide_string(baseName.c_str(),pars,"@");
	if(pars.size()!=2) return ret;
	
	exchange=pars[1];
	int size1 = pars[0].length();
	if(pars[1]=="LME"&&pars[0].length()>2)
	{
		comodity = pars[0].substr(0,size1-2);
		month = pars[0].substr(size1-2);
	}
	else if(pars[0].length()>6)
	{
		comodity = pars[0].substr(0,size1-6);
		month = pars[0].substr(size1-6);
	}
	else return ret;

	map<string, string>::iterator iter = rohonMap.find(comodity+"@"+exchange);
	if(iter == rohonMap.end()) return ret;
	string rohonExchangeComodity = iter->second;
	string rohonMonth = "";
	if(exchange=="LME")
	{
		rohonMonth = month;
	}
	else
	{
		map<string, string>::iterator monthIter = rohonMonthMap.find(month.substr(4));
		if(monthIter == rohonMonthMap.end()) return ret;
		rohonMonth = monthIter->second + month.substr(2,2);
	}

	return rohonExchangeComodity + " " + rohonMonth;
}

string NameConverter::rohon2BaseName(string rohonName)
{
	string ret = "";
	strings pars;
	divide_string(rohonName.c_str(),pars," ");
	if(pars.size()<3) return ret;

	string rohonMonth=pars[pars.size()-1];
	string baseMonth="";
	if(pars[0]=="LME") 
	{
		baseMonth=rohonMonth;
	}
	else
	{
		map<string, string>::iterator revertMonthIter = rohonRevertMonthMap.find(rohonMonth.substr(0,3));
		if(revertMonthIter == rohonRevertMonthMap.end()) return ret;
		baseMonth = "20"+ rohonMonth.substr(3) + revertMonthIter->second;
	}

	string rohonExchangeComodity = rohonName.substr(0,rohonName.find_last_of(" "));
	map<string, string>::iterator revertIter = rohonRevertMap.find(rohonExchangeComodity);
	if(revertIter == rohonRevertMap.end()) return ret;
	string baseExchangeComodity = revertIter->second;

	strings basePars;
	divide_string(baseExchangeComodity.c_str(),basePars,"@");
	if(basePars.size()!=2) return ret;

	return basePars[0]+"-" + baseMonth + "@"+basePars[1];
}