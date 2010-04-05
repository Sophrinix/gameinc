#include "StdAfx.h"
#include "NrpApplication.h"
#include "NrpCompany.h"
#include "IUser.h"
#include "NrpGameProject.h"
#include "NrpGameEngine.h"
#include "NrpTechnology.h"
#include "NrpBank.h"

static nrp::CNrpApplication* globalApplication = NULL;
					 //янв фев мрт апр май июн июл авг снт окт ноя дек	
int monthLen[ 12 ] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

namespace nrp
{

CNrpApplication::CNrpApplication(void) : INrpConfig( "CNrpApplication", "Appication")
{
	speed_ = SPD_MINUTE;
	time_.wYear = 1983;
	time_.wMonth = 0;
	time_.wDay = 0;
	time_.wHour = 0;
	time_.wMinute = 0;
	lastTimeUpdate_ = 0;

	options_[ BANK ] = new PNrpBank( NULL );
	options_[ TECHNUMBER ] = new int( 0 );
	options_[ EMPLOYERNUMBER ] = new int( 0 );
}

CNrpApplication::~CNrpApplication(void)
{
}

CNrpCompany* CNrpApplication::GetCompany( std::string companyName ) const
{
	COMPANY_LIST::const_iterator pIter = companies_.begin();

	for( ; pIter != companies_.end(); ++pIter)
		if( (*pIter)->GetOption<std::string>( NAME ) == companyName )
			return (*pIter);

	return NULL;
}

CNrpApplication::COMPANY_LIST& CNrpApplication::GetCompanies()
{
	return companies_;
}

int CNrpApplication::AddCompany( CNrpCompany* company )
{
	if( company )
	{
		companies_.push_back( company );
	}

	return 1;
}

int CNrpApplication::AddUser( bool player, IUser* user )
{
	if( user )
		( player ? ceo_ : employers_ ).push_back( user );
	return 1;
}

bool CNrpApplication::UpdateTime()
{
	if( GetTickCount() - lastTimeUpdate_ > 100 )
	{
		lastTimeUpdate_ = GetTickCount();
		SPEED spd = speed_;
		if( spd == SPD_MINUTE )
		{
			time_.wMinute += 10;
			if( time_.wMinute >= 60  )
			{
				 time_.wMinute = 0;
				 spd = SPD_HOUR;
			}
		}

		if( spd == SPD_HOUR )
		{
			time_.wHour++;
			if( time_.wHour > 18 )
			{
				time_.wHour = 9;
				spd = SPD_DAY;
				DoLuaFunctionsByType( APP_DAY_CHANGE, (void*)this );
			}
		}

		if( spd == SPD_DAY )
		{
			time_.wDay++;
			int monthL = monthLen[ time_.wMonth ] + (( time_.wMonth == 1 && (time_.wYear % 4 == 0)) ? 1 : 0 );
			if( time_.wDay > monthL )
			{
				time_.wDay = 0;
			    spd = SPD_MONTH;
				DoLuaFunctionsByType( APP_MONTH_CHANGE, (void*)this );
			}
		}

		if( spd == SPD_MONTH )
		{
			time_.wMonth++;
			if( time_.wMonth > 11 )
			{
				time_.wMonth = 0;
				time_.wYear++;
				DoLuaFunctionsByType( APP_YEAR_CHANGE, (void*)this );
			}
		}

		return true;
	}

	return false;
}

CNrpCompany* CNrpApplication::GetPlayerCompany()
{
	COMPANY_LIST::iterator pIter = companies_.begin();

	for( ; pIter != companies_.end(); ++pIter )
	{	
		PUser user = (*pIter)->GetOption<PUser>( CEO );
		
		if( user && user->ClassName() == "RealPlayer" )
			return (*pIter);
	}

	return NULL;
}

INrpProject* CNrpApplication::CreateGameProject( std::string name )
{
	return new CNrpGameProject( name );
}

nrp::CNrpGameEngine* CNrpApplication::CreateGameEngine( std::string name )
{
	return new CNrpGameEngine( name );
}

CNrpTechnology* CNrpApplication::CreateTechnology( int typeTech )
{
	return new CNrpTechnology( PROJECT_TYPE( typeTech ) );
}

CNrpTechnology* CNrpApplication::GetTechnology( int index ) const
{
	return index < (int)technologies_.size() ?  technologies_[ index ] : NULL;
}

void CNrpApplication::AddTechnology( CNrpTechnology* ptrTech )
{
	technologies_.push_back( ptrTech );
	SetOption<int>( TECHNUMBER, technologies_.size() );
}

CNrpApplication& nrp::CNrpApplication::Instance()
{
	if( !globalApplication )
		globalApplication = new CNrpApplication();

	return *globalApplication;
}

}//namespace nrp