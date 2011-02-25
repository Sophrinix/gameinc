#pragma once

#include <OleAuto.h>
#include <assert.h>

namespace nrp
{

class NrpTime
{
public:
	WORD& RHour() { return _date.wHour; }
	WORD& RMonth() { return _date.wMonth; }
	WORD& RYear() { return _date.wYear; }
	WORD& RMinute() { return _date.wMinute; }
	WORD& RDay() { return _date.wDay; }
	WORD& RSecond() { return _date.wSecond; }

	const WORD& RHour() const { return _date.wHour; }
	const WORD& RMonth() const { return _date.wMonth; }
	const WORD& RYear() const { return _date.wYear; }
	const WORD& RMinute() const { return _date.wMinute; }
	const WORD& RDay() const { return _date.wDay; }
	const WORD& RSecond() const { return _date.wSecond; }

	double ToDouble()
	{
		double fDate;
		SystemTimeToVariantTime( &_date, &fDate );

		return fDate;
	}

	NrpTime( const SYSTEMTIME& time )
	{
		_date = time;
		_date.wMilliseconds = 0;
	}

	NrpTime( double val )
	{
		VariantTimeToSystemTime( val, &_date );
	}

	NrpTime GetDate()
	{
		NrpTime ret( *this );
		ret._date.wHour = ret._date.wMinute = ret._date.wSecond = ret._date.wMilliseconds = 0;
		return ret;
	}

	int GetDaysToDate( const NrpTime& future )
	{
		return GetDaysToDate( future._date );
	}

	int Equale( const NrpTime& b )
	{
		return Equale( b._date );
	}

	int GetMonthToDate( const NrpTime& end )
	{
		return GetDaysToDate( end._date ) / 30;
	}

	NrpTime& AppendMonth( int month=1 )
	{
		return AppendDay( month * 30 );
	}

	NrpTime& AppendDay( int dayNumber=1 )
	{
		double time;
		int errCurrTime = SystemTimeToVariantTime( &_date, &time );
		assert( errCurrTime > 0 );

		time += dayNumber;
		VariantTimeToSystemTime( time, &_date );//прогноз завершения работ
		return *this;
	}

	NrpTime& AppendMinute( int minute )
	{
		double time;
		int errCurrTime = SystemTimeToVariantTime( &_date, &time );
		assert( errCurrTime > 0 );

		time += minute / ( 24 * 60 );
		VariantTimeToSystemTime( time, &_date );//прогноз завершения работ
		return *this;
	}

	NrpTime& AppendHour( int hour )
	{
		return AppendMinute( 60 * hour );
	}

	bool IsValid() 
	{
		return _date.wYear > 0 && _date.wMonth > 0 && _date.wMonth <= 12 && _date.wDay > 0 && _date.wDay <= 31;
	}

	static NrpTime FromLocalTime()
	{
		SYSTEMTIME time;
		GetLocalTime( &time );

		return NrpTime( time );
	}
private:
	SYSTEMTIME _date;

	int GetMonthToDate( const SYSTEMTIME& end )
	{
		return GetDaysToDate( end ) / 30;
	}

	int Equale( const SYSTEMTIME& b )
	{
		double fBegin, fEnd;
		SystemTimeToVariantTime( &_date, &fBegin );
		SystemTimeToVariantTime( &const_cast< SYSTEMTIME& >( b ), &fEnd );
		//вернем разницу в днях
		return fBegin == fEnd ? 0 : (fBegin < fEnd ? -1 : 1) ;
	}

	int GetDaysToDate( const SYSTEMTIME& future )
	{
		double fBegin, fEnd;
		SystemTimeToVariantTime( &_date, &fBegin );
		SystemTimeToVariantTime( &const_cast< SYSTEMTIME& >( future ), &fEnd );
		//вернем разницу в днях
		return abs( static_cast< int >( fBegin - fEnd ) );
	}
};

}//end namespace  nrp