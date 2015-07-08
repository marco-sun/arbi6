mkdir Release\pricestream
mkdir Release\dealstream
copy .\*.dll Release
copy .\*.cfg Release
copy .\*.ini Release
rem nmake -f "arbi.mak" CFG="arbi - Win32 Debug"