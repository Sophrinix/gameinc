#include "StdAfx.h"
#include "NrpApplication.h"
#include "NrpCompany.h"
#include "IUser.h"
#include "NrpGameProject.h"
#include "NrpGameEngine.h"
#include "NrpTechnology.h"
#include "NrpBank.h"
#include "NrpAiUser.h"
#include "NrpPlayer.h"
#include "OpFileSystem.h"
#include "nrpEngine.h"
#include "PeopleName.h"
#include "NrpGame.h"

#include <io.h>
#include <errno.h>

static nrp::CNrpApplication* globalApplication = NULL;
					 //янв фев мрт апр май июн июл авг снт окт ноя дек	
int monthLen[ 12 ] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

namespace nrp
{

CNrpApplication::CNrpApplication(void) : INrpConfig( "CNrpApplication", "Appication")
{
	speed_ = SPD_MINUTE;
	SYSTEMTIME time;
	time.wYear = 1983;
	time.wMonth = 0;
	time.wDay = 0;
	time.wHour = 0;
	time.wMinute = 0;
	lastTimeUpdate_ = 0;

	CreateValue<PNrpBank>( BANK, NULL );
	CreateValue<int>( TECHNUMBER, 0 );
	CreateValue<int>( USERNUMBER, 0 );
	CreateValue<int>( COMPANIESNUMBER, 0 );
	CreateValue<std::string>( FULLPATH, "" );
	std::string profileName = IniFile::Read( "options", "currentProfile", std::string( "dalerank" ), "config/system.ini" );
	std::string profileCompany = IniFile::Read( "options", "currentCompany", std::string( "daleteam" ), "config/system.ini" );
	CreateValue<std::string>( PROFILENAME, profileName );
	CreateValue<std::string>( PROFILECOMPANY, profileCompany );
	CreateValue<SYSTEMTIME>( CURRENTTIME, time );

	srand( GetTickCount() );
}

CNrpApplication::~CNrpApplication(void)
{
}

CNrpCompany* CNrpApplication::GetCompany( std::string companyName ) const
{
	COMPANIES_LIST::const_iterator pIter = companies_.begin();

	for( ; pIter != companies_.end(); pIter++)
		if( (*pIter)->GetValue<std::string>( NAME ) == companyName )
			return (*pIter);

	return NULL;
}

CNrpCompany* CNrpApplication::GetCompany( int index ) const
{
	return index < companies_.size() ? companies_[ index ] : NULL;
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
	SYSTEMTIME& time = GetValue<SYSTEMTIME>( CURRENTTIME );
	if( GetTickCount() - lastTimeUpdate_ > 100 )
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
				 BeginNewHour_();
			}
		}

		if( spd == SPD_HOUR )
		{
			time.wHour++;
			if( time.wHour > 18 )
			{
				time.wHour = 9;
				spd = SPD_DAY;
				DoLuaFunctionsByType( APP_DAY_CHANGE, (void*)this );
				BeginNewDay_();
			}
		}

		if( spd == SPD_DAY )
		{
			time.wDay++;
			int monthL = monthLen[ time.wMonth ] + (( time.wMonth == 1 && (time.wYear % 4 == 0)) ? 1 : 0 );
			if( time.wDay > monthL )
			{
				time.wDay = 0;
			    spd = SPD_MONTH;
				DoLuaFunctionsByType( APP_MONTH_CHANGE, (void*)this );
				BeginNewMonth_();
			}
		}

		if( spd == SPD_MONTH )
		{
			time.wMonth++;
			if( time.wMonth > 11 )
			{
				time.wMonth = 0;
				time.wYear++;
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

	for( ; pIter != companies_.end(); pIter++ )
	{	
		PUser user = (*pIter)->GetValue<PUser>( CEO );
		
		if( user && user->ClassName() == "RealPlayer" )
			return (*pIter);
	}

	return NULL;
}

INrpProject* CNrpApplication::CreateGameProject( std::string name )
{
	return new CNrpGameProject( name, NULL );
}

nrp::CNrpGameEngine* CNrpApplication::CreateGameEngine( std::string name )
{
	return new CNrpGameEngine( name );
}

CNrpTechnology* CNrpApplication::CreateTechnology( int typeTech )
{
	return new CNrpTechnology( PROJECT_TYPE( typeTech ), NULL );
}

CNrpTechnology* CNrpApplication::GetTechnology( int index ) const
{
	return index < (int)technologies_.size() ?  technologies_[ index ] : NULL;
}

CNrpTechnology* CNrpApplication::GetTechnology( const std::string& name ) const
{
	TECH_LIST::const_iterator pIter = technologies_.begin();
	for( ; pIter != technologies_.end(); pIter++ )
		if( (*pIter)->GetValue<std::string>( NAME ) == name )
			return (*pIter);

	return NULL;
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

IUser* CNrpApplication::GetUser( std::string name )
{
	USER_LIST::iterator pIter = users_.begin();
	for( ; pIter != users_.end(); pIter++ )
		if( (*pIter)->GetValueA<std::string>( NAME ) == name )
			return (*pIter);

	return NULL;	
}

IUser* CNrpApplication::GetUser( std::string company, std::string name )
{
	if( company.empty() )
	{
		COMPANIES_LIST::const_iterator pIter = companies_.begin();
		for( ; pIter != companies_.end(); pIter++)
		{
			IUser* ptrUser = (*pIter)->GetUser(name);
			if( ptrUser != NULL )
				return ptrUser;
		}

		return NULL;
	}
	else
	{
		PNrpCompany ptrCmp = GetCompany( company );
		return ptrCmp != NULL ? ptrCmp->GetUser( name ) : NULL;
	}
}

int CNrpApplication::RemoveUser( IUser* user )
{
	USER_LIST::iterator pIter = users_.begin();
	for( ; pIter != users_.end(); pIter++ )
		if( (*pIter) == user )
		{
			users_.erase( pIter );
			SetValue<int>( USERNUMBER, users_.size() );
			return 0;
		}

	return 1;
}

void CNrpApplication::SaveProfile()
{
	std::string saveFolder = "save/" + GetValue<std::string>( PROFILENAME ) + "/";
	std::string prevSaveFolder = "save/" + GetValue<std::string>( PROFILENAME ) + "Old/";

	OpFileSystem::MoveDirectory( CNrpEngine::Instance().GetWindowHandle(), saveFolder, prevSaveFolder );
	if( _access( saveFolder.c_str(), 0 ) == -1 )
		CreateDirectory( saveFolder.c_str(), NULL );

	std::string profileIni = saveFolder + "profile.ini";
	DeleteFile( profileIni.c_str() );
	INrpConfig::Save( PROPERTIES, profileIni );

	std::string config = "config/system.ini";
	IniFile::Write( "options", "currentProfile", GetValue<std::string>( PROFILENAME ), config );
	IniFile::Write( "options", "currentCompany", GetValue<std::string>( PROFILECOMPANY ), config );
	
	std::string saveFolderCompanies = saveFolder + "companies/";
	COMPANIES_LIST::iterator cIter = companies_.begin();
	for( int i=0; cIter != companies_.end(); cIter++, i++ )
	{
		(*cIter)->Save( saveFolderCompanies );
		IniFile::Write( "companies", "company_" + IntToStr(i), (*cIter)->GetValue<std::string>( NAME ), profileIni );
	}

	std::string saveFolderUsers = saveFolder + "freeUsers/";
	USER_LIST::iterator uIter = users_.begin();
	for( int i=0; uIter != users_.end(); uIter++, i++ )
	{
		(*uIter)->Save( saveFolderUsers );
		std::string text = (*uIter)->ClassName() + ":" + (*uIter)->GetValueA<std::string>( NAME );
		IniFile::Write( "users", "user_" + IntToStr(i), text, profileIni );
	}

	std::string saveFolderTech = saveFolder + "freeTech/";
	TECH_LIST::iterator tIter = technologies_.begin();
	for( int i=0; tIter != technologies_.end(); tIter++, i++ )
	{
		(*tIter)->Save( saveFolderTech );
		IniFile::Write( "technologies", "technology_" + IntToStr(i), (*tIter)->GetValue<std::string>( NAME ), profileIni );
	}
}

void CNrpApplication::LoadProfile( std::string profileName, std::string companyName )
{
	std::string saveFolder = "save/" + profileName + "/";
	std::string profileIni = saveFolder + "profile.ini";
	INrpConfig::Load( PROPERTIES, profileIni );

	std::string saveFolderUsers = saveFolder + "freeUsers/";
	for( int i=0; i < GetValue<int>( USERNUMBER ); i++ )
	{
		std::string name = IniFile::Read( "users", "user_" + IntToStr(i), std::string(""), profileIni );
		std::string className = name.substr( 0, name.find( ':' ) );
		name = name.substr( name.find(':') + 1, 0xff );
		IUser* usert = NULL;
		if( className == "RealPlayer" ) 
			usert = new CNrpPlayer( name, NULL );
		else
		if( className == "AIPlayer" )
			usert = new CNrpAiUser( name, NULL );
		else
			usert = new IUser( className.c_str(), "", NULL );

		users_.push_back( usert );
		usert->Load( saveFolderUsers + name + ".ini" );
	}

	std::string saveFolderTech = saveFolder + "freeTech/";
	for( int i=0; i < GetValue<int>( TECHNUMBER ); i++ )
	{
		std::string name = IniFile::Read( "technologies", "technology_" + IntToStr(i), std::string(""), profileIni );
		CNrpTechnology* tech = new CNrpTechnology( PROJECT_TYPE(0), NULL );
		technologies_.push_back( tech );
		tech->Load( saveFolderTech + name + ".ini" );
	}

	std::string saveFolderCompanies = saveFolder + "companies/";
	for( int i=0; i < GetValue<int>( COMPANIESNUMBER ); i++ )
	{
		std::string name = IniFile::Read( "companies", "company_" + IntToStr(i), std::string(""), profileIni );
		CNrpCompany* cmp = new CNrpCompany( name.c_str() );
		companies_.push_back( cmp );
		cmp->Load( saveFolderCompanies + name + "/" );
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
	for( ; cIter != companies_.end(); cIter++ )
		delete *cIter;
	companies_.clear();

	USER_LIST::iterator uIter = users_.begin();
	for( ; uIter != users_.end(); uIter++ )
		delete *uIter;
	users_.clear();

	TECH_LIST::iterator tIter = technologies_.begin();
	for( ; tIter != technologies_.end(); tIter++ )
		delete *tIter;	
	technologies_.clear();
}

CNrpGameEngine* CNrpApplication::GetGameEngine( std::string name )
{
	CNrpGameEngine* resultt= NULL;
	COMPANIES_LIST::const_iterator pIter = companies_.begin();

	for( ; pIter != companies_.end(); pIter++)
	{
		resultt = (*pIter)->GetGameEngine( name );
		if( resultt )
			return resultt;
	}

	return NULL;
}

void CNrpApplication::BeginNewDay_()
{
	COMPANIES_LIST::iterator cIter = companies_.begin();
	for( ; cIter != companies_.end(); cIter++)
		 (*cIter)->BeginNewDay( GetValue<SYSTEMTIME>( CURRENTTIME ));
}

void CNrpApplication::CreateNewFreeUsers()
{
	USER_LIST::iterator pIter = users_.begin();

	USER_LIST coders, designer, composer, tester, others;
	std::map< std::string, USER_LIST* > group;
	group[ "coder" ] = &coders;
	group[ "designer" ] = &designer;
	group[ "composer" ] = &composer;
	group[ "tester" ] = &tester;
	group[ "other" ] = &others;
	
	for( ; pIter != users_.end(); pIter++ )
	{
		std::string typeName = (*pIter)->GetType();
		if( group.find( typeName ) != group.end() )
			group[ typeName ]->push_back( *pIter );
		else
			group[ "other" ]->push_back( *pIter );
	}
	users_.clear();
	size_t USER_GROUP_COUNT = 6;

	std::map< std::string, USER_LIST* >::iterator gIter = group.begin();
	for( ; gIter != group.end(); gIter++ )
	{
		USER_LIST& tmpList = *(gIter->second);
		if( tmpList.size() > USER_GROUP_COUNT )
		{
			for( size_t cnt=USER_GROUP_COUNT; cnt < tmpList.size(); cnt++ )
				delete tmpList[ cnt ];
			tmpList.erase( tmpList.begin() + USER_GROUP_COUNT, tmpList.end() );
		}
		else 
		{
			for( size_t cnt=tmpList.size(); cnt < USER_GROUP_COUNT; cnt++ )
				tmpList.push_back( CreateRandomUser_( gIter->first ) );
		}
	}

	gIter = group.begin();
	for( ; gIter != group.end(); gIter++ )
		for(  size_t cnt=0; cnt < gIter->second->size(); cnt++ )
			users_.push_back( gIter->second->at( cnt ) );
	SetValue<int>( USERNUMBER, users_.size() );
}

CNrpApplication& nrp::CNrpApplication::Instance()
{
	if( !globalApplication )
		globalApplication = new CNrpApplication();

	return *globalApplication;
}

CNrpTechnology* CNrpApplication::GetTechnology( CNrpCompany* ptrCompany, const std::string& name )
{
	CNrpTechnology* tech = GetTechnology( name );
	if( !tech )
		tech = ptrCompany->GetTechnology( name );

	return tech;
}

template< class B > int GetQuality_( B* ptrObject )
{
	if( ptrObject )
		return ptrObject->GetValue<int>( QUALITY );
	return 0;
}

int CNrpApplication::GetGameRating_( CNrpGame* ptrGame, GAME_RATING_TYPE typeRating )
{
	CNrpCompany* cmp = GetCompany( ptrGame->GetValue<std::string>( COMPANYNAME ) );
	int rating = 0;
	int number = 0;

	switch( typeRating ) {
	case GRT_GENERAL:
		rating = GetQuality_( cmp->GetGameEngine( ptrGame->GetValue<std::string>( GAME_ENGINE ) ) );
		rating += GetQuality_( GetTechnology( cmp, ptrGame->GetValue<std::string>( LOCALIZATION ) ) );
		rating /= 2;
		rating += GetQuality_( GetTechnology( cmp, ptrGame->GetValue<std::string>( CROSSPLATFORMCODE ) ) );
		rating /= 2;
		rating += GetGameRating_( ptrGame, GRT_VIDEO );
		rating /= 2;
		rating += GetGameRating_( ptrGame, GRT_SOUND );
		rating /= 2;
		rating += GetGameRating_( ptrGame, GRT_ADVFUNC );
		rating /= 2;
		rating += GetGameRating_( ptrGame, GRT_GENRE );
		rating /= 2;
	break;

	case GRT_VIDEO:
		number = ptrGame->GetValue<int>( VIDEOTECHNUMBER );
		for( int cnt=0; cnt < number; cnt++ )
		{
			rating += GetQuality_( GetTechnology( cmp, ptrGame->GetVideoTech( cnt ) ) );
			rating /= 2;
		}
	break;

	case GRT_SOUND:
		number = ptrGame->GetValue<int>( SOUNDTECHNUMBER );
		for( int cnt=0; cnt < number; cnt++ )
		{
			rating += GetQuality_( GetTechnology( cmp, ptrGame->GetSoundTech( cnt ) ) );
			rating /= 2;
		}
	break;

	case GRT_ADVFUNC:
		number = ptrGame->GetValue<int>( ADVTECHNUMBER );
		for( int cnt=0; cnt < number; cnt++ )
		{
			rating += GetQuality_( GetTechnology( cmp, ptrGame->GetAdvTech( cnt ) ) );
			rating /= 2;
		}
	break;

	case GRT_GENRE:
		number = ptrGame->GetValue<int>( GENRE_MODULE_NUMBER );
		for( int cnt=0; cnt < number; cnt++ )
		{
			rating += GetQuality_( GetTechnology( cmp, ptrGame->GetGenreTech( cnt ) ) );
			rating /= 2;
		}
	break;
	}

	return rating;
}

void CNrpApplication::UpdateGameRatings( CNrpGame* ptrGame, bool firstTime )
{
	if( firstTime )
	{
		ptrGame->SetValue<int>( STARTGAMERATING, GetGameRating_( ptrGame, GRT_GENERAL ) );
		ptrGame->SetValue<int>( STARTGRAPHICRATING, GetGameRating_( ptrGame, GRT_VIDEO ) );
		ptrGame->SetValue<int>( STARTGENRERATING, GetGameRating_( ptrGame, GRT_GENRE ) );
		ptrGame->SetValue<int>( STARTSOUNDRATING, GetGameRating_( ptrGame, GRT_SOUND ) );
		ptrGame->SetValue<int>( STARTADVFUNC, GetGameRating_( ptrGame, GRT_ADVFUNC ) );
	}

	ptrGame->SetValue<int>( CURRENTGAMERATING, GetGameRating_( ptrGame, GRT_GENERAL ) );
	ptrGame->SetValue<int>( CURRENTGRAPHICRATING, GetGameRating_( ptrGame, GRT_VIDEO ) );
	ptrGame->SetValue<int>( CURRENTGENRERATING, GetGameRating_( ptrGame, GRT_GENRE ) );
	ptrGame->SetValue<int>( CURRENTSOUNDRATING, GetGameRating_( ptrGame, GRT_SOUND ) );
}

IUser* CNrpApplication::CreateRandomUser_( std::string userType )
{
	size_t randomParams = 1 + rand() % (GT_COUNT % 100);//сколько параметров будем создавать
	size_t maxParamValue = 1 + rand() % 100;//максимальное значение параметров

	std::map< std::string, int > skillMap;
	skillMap[ "coder" ] = SKL_CODING;
	skillMap[ "designer" ] = SKL_DRAWING;
	skillMap[ "composer" ] = SKL_SOUND;
	skillMap[ "tester" ] = SKL_TESTING;

	std::string userName;

	IUser* ptrUser = NULL;
	do 
	{
		userName = GlobalPeopleName[ rand() % PEOPLE_NAME_COUNT ] + " " + GlobalPeopleSurname[ rand() % PEOPLE_SURNAME_COUNT ];
		ptrUser = GetUser( "", userName );
	} while ( ptrUser != NULL );

	ptrUser = new IUser( userType.c_str(), userName.c_str(), NULL );
	ptrUser->SetSkill( skillMap[ userType ], maxParamValue );
	ptrUser->SetValue<int>( CODE_QUALITY, rand() % maxParamValue );
	ptrUser->SetValue<int>( CODE_SPEED, rand() % maxParamValue );
	ptrUser->SetValue<int>( TALANT, rand() % maxParamValue );
	ptrUser->SetValue<int>( STAMINA, rand() % maxParamValue );
	ptrUser->SetValue<int>( STABILITY, rand() % maxParamValue );
	ptrUser->SetValue<int>( CHARACTER, rand() % maxParamValue );

	for( size_t cnt=0; cnt < randomParams; cnt++ )
	{
		ptrUser->SetGenreExperience( rand() % (GT_COUNT%100), rand() % maxParamValue );
		ptrUser->SetGenrePreferences( rand() % (GT_COUNT%100), rand() % maxParamValue );
	} 

	return ptrUser;
}

void CNrpApplication::BeginNewMonth_()
{
	COMPANIES_LIST::iterator cIter = companies_.begin();
	for( ; cIter != companies_.end(); cIter++)
		(*cIter)->BeginNewMonth( GetValue<SYSTEMTIME>( CURRENTTIME ));
}

void CNrpApplication::BeginNewHour_()
{
	COMPANIES_LIST::iterator cIter = companies_.begin();
	for( ; cIter != companies_.end(); cIter++)
		(*cIter)->BeginNewHour( GetValue<SYSTEMTIME>( CURRENTTIME ));
}

void CNrpApplication::AddBoxAddon( CNrpTechnology* tech )
{
	boxAddons_.push_back( tech );
}
}//namespace nrp