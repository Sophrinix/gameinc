// stdafx.cpp : source file that includes just the standard includes
// nerpa_sc.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

bool operator <(SYSTEMTIME const &t1, SYSTEMTIME const &t2)
{
	FILETIME ft1, ft2;
	if (!SystemTimeToFileTime(&t1, &ft1)) return true;
	if (!SystemTimeToFileTime(&t2, &ft2)) return true;
	return *((ULONGLONG*)&ft1) < *((ULONGLONG*)&ft2);
}

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
