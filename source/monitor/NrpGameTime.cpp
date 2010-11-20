#include "StdAfx.h"
#include "NrpGameTime.h"
#include "NrpApplication.h"
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

	(*parent)[ CURRENTTIME ] = time;
}

CNrpGameTime::~CNrpGameTime(void)
{
}

bool CNrpGameTime::Update()
{
	CNrpApplication& app = CNrpApplication::Instance();
	SYSTEMTIME& time = app[ CURRENTTIME ];
	time.wDayOfWeek = time.wMilliseconds = time.wSecond = 0;
	if( GetTickCount() - lastTimeUpdate_ > (int)app[ PAUSEBTWSTEP ] )
	{
		lastTimeUpdate_ = GetTickCount();
		SPEED spd = speed_;
		if( spd == SPD_MINUTE )
		{
			time.wMinute += 10;
			if( time.wMinute >= 60  )
			{
				time.wMinute = 0;
				spd = SPD_HOUR;
				app._BeginNewHour();
			}
		}

		if( spd == SPD_HOUR )
		{
			time.wHour++;
			if( time.wHour > 18 )
			{
				time.wHour = 9;
				spd = SPD_DAY;
				app._BeginNewDay();
				app.DoLuaFunctionsByType( APP_DAY_CHANGE, this );
			}
		}

		if( spd == SPD_DAY )
		{
			time.wDay++;
			int monthL = monthLen[ time.wMonth ] + (( time.wMonth == 1 && (time.wYear % 4 == 0)) ? 1 : 0 );
			if( time.wDay > monthL )
			{
				time.wDay = 1;
				spd = SPD_MONTH;
				app._BeginNewMonth();
				app.DoLuaFunctionsByType( APP_MONTH_CHANGE, this );
			}
		}

		if( spd == SPD_MONTH )
		{
			time.wMonth++;
			if( time.wMonth > 12 )
			{
				time.wMonth = 1;
				time.wYear++;
				//app.BeginNewYear_();
				app.DoLuaFunctionsByType( APP_YEAR_CHANGE, this );
			}
		}

		return true;
	}

	return false;
}

}//end namespace nrp