// stdafx.cpp : source file that includes just the standard includes
// nerpa_sc.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

bool operator <(SYSTEMTIME const &t1, SYSTEMTIME const &t2)
{
	if( t1.wYear == t2.wYear )
		if( t1.wMonth == t2.wMonth )
			if( t1.wDay == t2.wDay )
				if( t1.wHour == t2.wHour )
					if( t1.wMinute == t2.wMinute ) return t1.wSecond < t2.wSecond;
					else return t1.wMinute < t2.wMinute;
				else return t1.wHour < t2.wHour;
			else return t1.wDay < t2.wDay;
		else return t1.wMonth < t2.wMonth;
	else return t1.wYear < t2.wYear; 
}

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
