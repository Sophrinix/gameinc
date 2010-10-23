#pragma once

#include <OleAuto.h>

namespace nrp
{

class TimeHelper
{
public:
	static int GetDaysBetweenDate( SYSTEMTIME& begin, SYSTEMTIME& end )
	{
		double fBegin, fEnd;
		SystemTimeToVariantTime( &begin, &fBegin );
		SystemTimeToVariantTime( &end, &fEnd );
		//������ ������� � ����
		return abs( static_cast< int >( fBegin - fEnd ) );
	}

	static SYSTEMTIME GetDateWithDay( SYSTEMTIME& start, int dayNumber )
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