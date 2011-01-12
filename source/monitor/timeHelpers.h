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
		//вернем разницу в днях
		return abs( static_cast< int >( fBegin - fEnd ) );
	}

	static int TimeCmp( const SYSTEMTIME& a, const SYSTEMTIME& b )
	{
		double fBegin, fEnd;
		SystemTimeToVariantTime( &const_cast< SYSTEMTIME& >( a ), &fBegin );
		SystemTimeToVariantTime( &const_cast< SYSTEMTIME& >( b ), &fEnd );
		//вернем разницу в днях
		return fBegin == fEnd ? 0 : (fBegin < fEnd ? -1 : 1) ;
	}

	static int GetMonthBetweenDate( const SYSTEMTIME& begin, const SYSTEMTIME& end )
	{
		return GetDaysBetweenDate( begin, end ) / 30;
	}

	static SYSTEMTIME DatePlusDay( const SYSTEMTIME& start, int dayNumber )
	{
		double time;
		int errCurrTime = SystemTimeToVariantTime( &const_cast< SYSTEMTIME& >( start ), &time );
		assert( errCurrTime > 0 );

		time += dayNumber;
		SYSTEMTIME rTime;
		VariantTimeToSystemTime( time, &rTime );//прогноз завершения работ
		return rTime;
	}
};

}//end namespace  nrp