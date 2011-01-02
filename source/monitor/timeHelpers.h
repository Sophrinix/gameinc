#pragma once

#include <OleAuto.h>

namespace nrp
{

class TimeHelper
{
public:
	static int DateToInt( SYSTEMTIME& date )
	{
		double fDate;
		SystemTimeToVariantTime( &date, &fDate );

		return static_cast< int >( floor( fDate ) );
	}

	static int GetDaysBetweenDate( const SYSTEMTIME& begin, const SYSTEMTIME& end )
	{
		double fBegin, fEnd;
		SystemTimeToVariantTime( &const_cast< SYSTEMTIME& >( begin ), &fBegin );
		SystemTimeToVariantTime( &const_cast< SYSTEMTIME& >( end ), &fEnd );
		//������ ������� � ����
		return abs( static_cast< int >( fBegin - fEnd ) );
	}

	static int TimeCmp( const SYSTEMTIME& a, const SYSTEMTIME& b )
	{
		double fBegin, fEnd;
		SystemTimeToVariantTime( &const_cast< SYSTEMTIME& >( a ), &fBegin );
		SystemTimeToVariantTime( &const_cast< SYSTEMTIME& >( b ), &fEnd );
		//������ ������� � ����
		return fBegin == fEnd ? 0 : (fBegin < fEnd ? -1 : 1) ;
	}

	static int GetMonthBetweenDate( SYSTEMTIME& begin, SYSTEMTIME& end )
	{
		return GetDaysBetweenDate( begin, end ) / 30;
	}

	static SYSTEMTIME DatePlusDay( SYSTEMTIME& start, int dayNumber )
	{
		double time;
		int errCurrTime = SystemTimeToVariantTime( &start, &time );
		assert( errCurrTime > 0 );

		time += dayNumber;
		SYSTEMTIME rTime;
		VariantTimeToSystemTime( time, &rTime );//������� ���������� �����
		return rTime;
	}
};

}//end namespace  nrp