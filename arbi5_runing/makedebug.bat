mkdir Debug\pricestream
mkdir Debug\dealstream
copy sfit\*.dll Debug
copy esunny\*.dll Debug
copy rohon\*.dll Debug
copy .\*.cfg Debug
copy .\*.ini Debug
copy .\*.txt Debug
rem nmake -f "arbi.mak" CFG="arbi - Win32 Debug"
