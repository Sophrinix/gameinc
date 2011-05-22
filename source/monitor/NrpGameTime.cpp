#include "StdAfx.h"
#include "NrpGameTime.h"
#include "NrpApplication.h"
#include "NrpTime.h"
#include "irrlicht.h"

//янв фев мрт апр май июн июл авг снт окт ноя дек	
static int monthLen[ 13 ] = { 0, 31,  28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

namespace nrp
{

CNrpGameTime::CNrpGameTime( CNrpApplication* parent )
{
	speed_ = SPD_MINUTE;
	lastTimeUpdate_ = 0;

	SYSTEMTIME time;
	time.wYear = 1983;
	time.wMonth = 1;
	time.wDay = 1;
	time.wHour = 0;
	time.wMinute = 0;
	time.wSecond = 0;

	(*parent)[ CURRENTTIME ] = NrpTime( time );
}

CNrpGameTime::~CNrpGameTime(void)
{
}

bool CNrpGameTime::Update()
{
	NrpTime& time = _nrpApp[ CURRENTTIME ].As<NrpTime>();
	NrpTime oldTime( time );

	if( GetTickCount() - lastTimeUpdate_ > (int)_nrpApp[ PAUSEBTWSTEP ] )
	{
		lastTimeUpdate_ = GetTickCount();

		time.AppendMinute( 10 );

		if( oldTime.RHour() != time.RHour() )
			_nrpApp._BeginNewHour();

		if( time.RHour() > 17 )
		{
			time.AppendDay();
			time.RHour() = 8;
			_nrpApp._BeginNewDay();
			_nrpApp.PCall( APP_DAY_CHANGE, this );
		}

		if( time.RMonth() != oldTime.RMonth() )
		{
			_nrpApp._BeginNewMonth();
			_nrpApp.PCall( APP_MONTH_CHANGE, this );
		}

		if( time.RYear() != oldTime.RMonth() )
		{
			//app.BeginNewYear_();
			_nrpApp.PCall( APP_YEAR_CHANGE, this );
		}

		return true;
	}

	return false;
}

}//end namespace nrp