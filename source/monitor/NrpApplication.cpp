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

	CreateValue<PNrpBank>( BANK, NULL );
	CreateValue<int>( TECHNUMBER, 0 );
	CreateValue<int>( USERNUMBER, 0 );
	CreateValue<int>( COMPANIESNUMBER, 0 );
	std::string profileName = IniFile::Read( "options", "currentProfile", std::string( "dalerank" ), "config/system.ini" );
	std::string profileCompany = IniFile::Read( "options", "currentCompany", std::string( "daleteam"), "config/system.ini" );
	CreateValue<std::string>( PROFILENAME, profileName );
	CreateValue<std::string>( PROFILECOMPANY, profileCompany );
	LoadProfile( profileName );
}

CNrpApplication::~CNrpApplication(void)
{
}

CNrpCompany* CNrpApplication::GetCompany( std::string companyName ) const
{
	COMPANIES_LIST::const_iterator pIter = companies_.begin();

	for( ; pIter != companies_.end(); ++pIter)
		if( (*pIter)->GetValue<std::string>( NAME ) == companyName )
			return (*pIter);

	return NULL;
}

CNrpApplication::COMPANIES_LIST& CNrpApplication::GetCompanies()
{
	return companies_;
}

int CNrpApplication::AddCompany( CNrpCompany* company )
{
	if( company )
	{
		companies_.push_back( company );
	}

	SetValue<int>( COMPANIESNUMBER, companies_.size() );

	return 1;
}

int CNrpApplication::AddUser( bool player, IUser* user )
{
	if( user )
		users_.push_back( user );
	SetValue<int>( USERNUMBER, users_.size() );
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
	COMPANIES_LIST::iterator pIter = companies_.begin();

	for( ; pIter != companies_.end(); ++pIter )
	{	
		PUser user = (*pIter)->GetValue<PUser>( CEO );
		
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
	SetValue<int>( TECHNUMBER, technologies_.size() );
}

IUser* CNrpApplication::GetUser( int index )
{
	return index < (int)users_.size() ? users_[ index ] : NULL;
}

int CNrpApplication::RemoveUser( IUser* user )
{
	USER_LIST::iterator pIter = users_.begin();
	for( ; pIter != users_.end(); ++pIter )
		if( (*pIter) == user )
		{
			users_.erase( pIter );
			return 0;
		}

	return 1;
}

void CNrpApplication::SaveProfile()
{
	std::string saveFolder = "save/" + GetValue<std::string>( PROFILENAME ) + "/";
	std::string profileIni = saveFolder + "profile.ini";

	DeleteFile( profileIni.c_str() );

	std::string config = "config/system.ini";
	IniFile::Write( "options", "currentProfile", GetValue<std::string>( PROFILENAME ), config );
	IniFile::Write( "options", "currentCompany", GetValue<std::string>( PROFILECOMPANY ), config );
	
	std::string saveFolderCompanies = saveFolder + "companies/";
	COMPANIES_LIST::iterator cIter = companies_.begin();
	for( int i=0; cIter != companies_.end(); ++cIter, ++i )
	{
		(*cIter)->Save( saveFolderCompanies );
		IniFile::Write( "companies", "company_" + IntToStr(i), (*cIter)->GetValue<std::string>( NAME ), profileIni );
	}
	IniFile::Write( "options", "companyNumber", companies_.size(), profileIni );

	std::string saveFolderUsers = saveFolder + "freeUsers/";
	USER_LIST::iterator uIter = users_.begin();
	for( int i=0; uIter != users_.end(); ++uIter, ++i )
	{
		(*uIter)->Save( saveFolderUsers );
		IniFile::Write( "users", "user_" + IntToStr(i), (*uIter)->GetValue<std::string>( NAME ), profileIni );
	}
	IniFile::Write( "options", "userNumber", users_.size(), profileIni );

	std::string saveFolderTech = saveFolder + "freeTech/";
	TECH_LIST::iterator tIter = technologies_.begin();
	for( int i=0; tIter != technologies_.end(); ++tIter, ++i )
	{
		(*tIter)->Save( saveFolderTech );
		IniFile::Write( "technologies", "technology_" + IntToStr(i), (*tIter)->GetValue<std::string>( NAME ), profileIni );
	}
	IniFile::Write( "options", "techNumber", technologies_.size(), profileIni );
}

void CNrpApplication::LoadProfile( std::string profileName )
{
	std::string saveFolder = "save/" + profileName + "/";
	std::string profileIni = saveFolder + "profile.ini";
	INrpConfig::Load( "options", profileIni );

	std::string saveFolderCompanies = saveFolder + "companies/";
	for( int i=0; i < GetValue<int>( COMPANIESNUMBER ); ++i )
	{
		std::string name = IniFile::Read( "companies", "company_" + IntToStr(i), std::string(""), profileIni );
		CNrpCompany* cmp = new CNrpCompany( name.c_str() );
		cmp->Load( saveFolderCompanies + name + "/" );
		companies_.push_back( cmp );
	}

	std::string saveFolderUsers = saveFolder + "freeUsers/";
	for( int i=0; i < GetValue<int>( USERNUMBER ); ++i )
	{
		std::string name = IniFile::Read( "users", "user_" + IntToStr(i), std::string(""), profileIni );
		std::string className = name.substr( 0, name.find( ':' ) );
		name = name.substr( name.find(':') + 1, 0xff );
		IUser* user = new IUser( className.c_str(), "" );
		user->Load( saveFolderUsers + name + ".ini" );
		users_.push_back( user );
	}

	std::string saveFolderTech = saveFolder + "freeTech/";
	for( int i=0; i < GetValue<int>( TECHNUMBER ); ++i )
	{
		std::string name = IniFile::Read( "technologies", "technology_" + IntToStr(i), std::string(""), profileIni );
		CNrpTechnology* tech = new CNrpTechnology( PROJECT_TYPE(0) );
		tech->Load( saveFolder + name + ".ini" );
		technologies_.push_back( tech );
	}
}

void CNrpApplication::CreateProfile( std::string profileName, std::string companyName )
{
	SetValue<std::string>( PROFILENAME, profileName );
	SetValue<std::string>( PROFILECOMPANY, companyName );
}

void CNrpApplication::ResetData()
{
	COMPANIES_LIST::iterator cIter = companies_.begin();
	for( ; cIter != companies_.end(); ++cIter )
		delete *cIter;
	companies_.clear();

	USER_LIST::iterator uIter = users_.begin();
	for( ; uIter != users_.end(); ++uIter )
		delete *uIter;
	users_.clear();

	TECH_LIST::iterator tIter = technologies_.begin();
	for( ; tIter != technologies_.end(); ++tIter )
		delete *tIter;	
	technologies_.clear();
}

CNrpApplication& nrp::CNrpApplication::Instance()
{
	if( !globalApplication )
		globalApplication = new CNrpApplication();

	return *globalApplication;
}

}//namespace nrp