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
#include "NrpDiskMachine.h"
#include "NrpPlant.h"
#include "NrpGameBox.h"
#include "NrpRetailer.h"
#include "NrpGameImageList.h"
#include "NrpDevelopGame.h"
#include "NrpInvention.h"

#include <io.h>
#include <errno.h>
#include <OleAuto.h>

static nrp::CNrpApplication* globalApplication = NULL;
					 //€нв фев мрт апр май июн июл авг снт окт но€ дек	
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
	std::string profileName = IniFile::Read( SECTION_OPTIONS, "currentProfile", std::string( "dalerank" ), "config/system.ini" );
	std::string profileCompany = IniFile::Read( SECTION_OPTIONS, "currentCompany", std::string( "daleteam" ), "config/system.ini" );
	CreateValue<std::string>( PROFILENAME, profileName );
	CreateValue<std::string>( PROFILECOMPANY, profileCompany );
	CreateValue<SYSTEMTIME>( CURRENTTIME, time );
	CreateValue<int>( DISKMACHINENUMBER, 0 );
	CreateValue<int>( BOXADDONNUMBER, 0 );
	CreateValue<int>( GAMENUMBER, 0 );
	CreateValue<int>( ENGINES_NUMBER, 0 );
	CreateValue<int>( DEVELOPPROJECTS_NUMBER, 0 );
	CreateValue<int>( PROJECTNUMBER, 0 );
	CreateValue<PNrpCompany>( PLAYERCOMPANY, NULL );
	CreateValue<int>( INVENTIONSNUMBER, 0 );

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
	return index < (int)companies_.size() ? companies_[ index ] : NULL;
}

CNrpApplication::COMPANIES_LIST& CNrpApplication::GetCompanies()
{
	return companies_;
}

int CNrpApplication::AddCompany( CNrpCompany* company )
{
	if( company )
		companies_.push_back( company );

	SetValue<int>( COMPANIESNUMBER, companies_.size() );
	PUser ceo = company->GetValue<PUser>( CEO );
	if( ceo && ceo->ClassName() == CLASS_REALPLAYER )
		SetValue<PNrpCompany>( PLAYERCOMPANY, company );

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
	time.wDayOfWeek = time.wMilliseconds = time.wSecond = 0;
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

void CNrpApplication::AddProject( INrpProject* project )
{
	projects_[ project->GetValue<std::string>( NAME ) ] = project;
}

CNrpTechnology* CNrpApplication::GetTechnology( int index ) const
{
	return index < (int)technologies_.size() ?  technologies_[ index ] : NULL;
}

CNrpTechnology* CNrpApplication::GetTechnology( const std::string& name ) const
{
	TECH_LIST::const_iterator pIter = technologies_.begin();

	for( ; pIter != technologies_.end(); pIter++ )
		if( (*pIter)->GetValue<std::string>( NAME ) == name ||
			(*pIter)->GetValue<std::string>( INTERNAL_NAME ) == name )
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
		if( (*pIter)->GetValue<std::string>( NAME ) == name )
			return (*pIter);

	return NULL;	
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

	OpFileSystem::MoveDirectory( saveFolder, prevSaveFolder );
	if( _access( saveFolder.c_str(), 0 ) == -1 )
		CreateDirectory( saveFolder.c_str(), NULL );

	std::string profileIni = saveFolder + "profile.ini";
	DeleteFile( profileIni.c_str() );
	INrpConfig::Save( SECTION_PROPERTIES, profileIni );

	std::string config = "config/system.ini";
	IniFile::Write( SECTION_OPTIONS, "currentProfile", GetValue<std::string>( PROFILENAME ), config );
	IniFile::Write( SECTION_OPTIONS, "currentCompany", GetValue<std::string>( PROFILECOMPANY ), config );
	
	COMPANIES_LIST::iterator cIter = companies_.begin();
	for( int i=0; cIter != companies_.end(); cIter++, i++ )
	{
		(*cIter)->Save( saveFolder + "companies/" );
		IniFile::Write( "companies", "company_" + IntToStr(i), (*cIter)->GetValue<std::string>( NAME ), profileIni );
	}

	PROJECTS_MAP::iterator dIter = devProjects_.begin();
	for( int i=0; dIter != devProjects_.end(); dIter++, i++ )
	{
		dynamic_cast< INrpDevelopProject* >( dIter->second )->Save( saveFolder + "devProjects/" );
		IniFile::Write( "devprojects", "project_" + IntToStr(i), dIter->second->ClassName() + ":" + dIter->first, profileIni );
	}

	PROJECTS_MAP::iterator ppIter = projects_.begin();
	for( int i=0; ppIter != projects_.end(); ppIter++, i++ )
	{
		ppIter->second->Save( SECTION_PROPERTIES, saveFolder + "Projects/" + ppIter->first + ".prj" );
		IniFile::Write( "projects", "project_" + IntToStr(i), ppIter->second->ClassName() + ":" + ppIter->first, profileIni );
	}

	USER_LIST::iterator uIter = users_.begin();
	for( int i=0; uIter != users_.end(); uIter++, i++ )
	{
		(*uIter)->Save( saveFolder + "users/" );
		std::string text = (*uIter)->ClassName() + ":" + (*uIter)->GetValue<std::string>( NAME );
		IniFile::Write( "users", "user_" + IntToStr(i), text, profileIni );
	}

	TECH_LIST::iterator tIter = technologies_.begin();
	for( int i=0; tIter != technologies_.end(); tIter++, i++ )
	{
		(*tIter)->Save( saveFolder + "techs/" );
		IniFile::Write( "technologies", "technology_" + IntToStr(i), (*tIter)->GetValue<std::string>( NAME ), profileIni );
	}

	INVENTION_LIST::iterator invIter = inventions_.begin();
	for( int i=0; invIter != inventions_.end(); invIter++ )
	{
		(*invIter)->Save( saveFolder + "inventions/" );
		IniFile::Write( "inventions", "invention_" + IntToStr( i ), (*invIter)->GetValue<std::string>( NAME ), profileIni );
	}

	GAMEENGINES_MAP::iterator eIter = engines_.begin();
	for( int i=0; eIter != engines_.end(); eIter++, i++ )
	{
		eIter->second->Save( saveFolder + "engines/" );
		IniFile::Write( "engines", "engine_" + IntToStr(i), eIter->first,  profileIni );
	}

	GAMES_LIST::iterator gameIter = games_.begin();
	for( int i=0; gameIter != games_.end(); gameIter++, i++ )
	{
		const std::string& name = (*gameIter)->GetValue<std::string>( NAME );
		(*gameIter)->Save( saveFolder + "games/" );
		IniFile::Write( "games", "game_"+IntToStr( i ), name, profileIni );
	}

	std::string imageListIni = saveFolder + "imageList.ini";
	GAMEIMAGES_MAP::iterator gameListIter = gameImages_.begin();
	int gli = 0;
	for( ; gameListIter != gameImages_.end(); gameListIter++ )
		if( !gameListIter->second->GetValue<bool>( ISUSED ) )
		{
			IniFile::Write( "imageList", "imageList_" + IntToStr( gli ), 
							gameListIter->second->GetValue<std::string>( NAME ), imageListIni );
			gli++;
		}
	IniFile::Write( SECTION_PROPERTIES, "ImageListNumber", gli, imageListIni );
}

void CNrpApplication::LoadProfile( std::string profileName, std::string companyName )
{
	CNrpScript::Instance().CreateTemporaryScript( "UsersAction" );

	std::string saveFolder = "save/" + profileName + "/";
	std::string profileIni = saveFolder + "profile.ini";
	INrpConfig::Load( SECTION_PROPERTIES, profileIni );

	std::string saveFolderUsers = saveFolder + "users/";
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
			usert = new IUser( className, "" );

		users_.push_back( usert );
		usert->Load( saveFolderUsers + name + ".ini" );
	}

	std::string saveFolderTech = saveFolder + "Techs/";
	for( int i=0; i < GetValue<int>( TECHNUMBER ); i++ )
	{
		std::string name = IniFile::Read( "technologies", "technology_" + IntToStr(i), std::string(""), profileIni );
		CNrpTechnology* tech = new CNrpTechnology( PROJECT_TYPE(0) );
		technologies_.push_back( tech );
		tech->Load( saveFolderTech + name + ".tech" );
	}

	std::string saveFolderEngines = saveFolder + "engines/";
	for( int i=0; i < GetValue<int>( ENGINES_NUMBER ); i++ )
	{
		std::string name = IniFile::Read( "engines", "engine_" + IntToStr(i), std::string(""), profileIni );
		CNrpGameEngine* eg = new CNrpGameEngine( "tmp" );
		eg->Load( saveFolderEngines + name );
		engines_[ name ] = eg;
	}

	std::string saveDevelopProjects = saveFolder + "devProjects/";
	for( int i=0; i < GetValue<int>( DEVELOPPROJECTS_NUMBER ); i++ )
	{
		std::string name = IniFile::Read( "devProjects", "project_" + IntToStr( i ), std::string(""), profileIni );
		std::string type = name.substr( 0, name.find( ':' ) );
		name = name.substr( name.find( ':' ) + 1, name.length() );
		if( type == CLASS_DEVELOPGAME )
		{
			CNrpDevelopGame* game = new CNrpDevelopGame( "tmp", NULL );
			game->Load( saveDevelopProjects + name + "/" );
			devProjects_[ name ] = game;
		}
		else if( type == "devengine" )
		{
			//
		}
	}

	std::string saveProjects = saveFolder + "Projects/";
	for( int i=0; i < GetValue<int>( PROJECTNUMBER ); i++ )
	{
		std::string name = IniFile::Read( "Projects", "project_" + IntToStr( i ), std::string(""), profileIni );
		std::string type = name.substr( 0, name.find( ':' ) );
		name = name.substr( name.find( ':' ) + 1, name.length() );
		if( type == CLASS_GAMEPROJECT )
		{
			CNrpDevelopGame* game = new CNrpDevelopGame( "tmp", NULL );
			game->Load( saveDevelopProjects + name + "/" );
			devProjects_[ name ] = game;
		}
		else if( type == "devengine" )
		{
			//
		}
	}

	for( int i=0; i < GetValue<int>( GAMENUMBER ); i++ )
	{
		std::string name = IniFile::Read( "games",  "game_"+IntToStr( i ), std::string(""), profileIni );
		PNrpGame game = new CNrpGame( name );
		game->Load( saveFolder + "games/" + name );
		games_.push_back( game );
	}

	for( int k=0; k < GetValue<int>( INVENTIONSNUMBER ); k++ )
	{
		std::string name = IniFile::Read( SECTION_INVENTIONS, "invention_"+ IntToStr(k), std::string(""), profileIni );
		CNrpInvention* invention = new CNrpInvention( saveFolder + "inventions/" + name + ".invent" );
		inventions_.push_back( invention );
	}
	SetValue<int>( INVENTIONSNUMBER, inventions_.size() );

	std::string saveFolderCompanies = saveFolder + "companies/";
	for( int k=0; k < GetValue<int>( COMPANIESNUMBER ); k++ )
	{
		std::string name = IniFile::Read( "companies", "company_" + IntToStr(k), std::string(""), profileIni );
		CNrpCompany* cmp = new CNrpCompany( name, NULL );
		cmp->Load( saveFolderCompanies + name + "/" );
		AddCompany( cmp );
	}
	SetValue<int>( GAMENUMBER, games_.size() );

    LoadFreeImageLists_( saveFolder + "imageList.ini" );
	CNrpScript::Instance().DoFile( "tmp/UsersAction.lua" );
}

void CNrpApplication::LoadFreeImageLists_( const std::string& fileName )
{
	size_t imageListNumber = IniFile::Read( SECTION_PROPERTIES, "ImageListNumber", (int)0, fileName );

	if( _access( fileName.c_str(), 0 ) == 0 && imageListNumber > 0)
	{
		GAMEIMAGES_MAP::iterator pIter = gameImages_.begin();
		for( ; pIter != gameImages_.end(); pIter++ )
			 pIter->second->SetValue<bool>( ISUSED, true );

		for( size_t i=0; i < imageListNumber; i++ )
		{
			std::string name = IniFile::Read( SECTION_PROPERTIES,  "imageList_"+IntToStr( i ), std::string(""), fileName );
			GAMEIMAGES_MAP::iterator findIter = gameImages_.find( name );
			if( findIter != gameImages_.end() )
				findIter->second->SetValue<bool>( ISUSED, false );
		}
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
		 (*cIter)->BeginNewDay( GetValue<SYSTEMTIME>( CURRENTTIME ) );

	CNrpPlant::Instance().BeginNewDay();
	UpdateMarketGames_();
	UpdateInvention_();
}

void CNrpApplication::UpdateInvention_()
{
	INVENTION_LIST::iterator pIter = inventions_.begin();
	for( ; pIter != inventions_.end(); pIter++ )
	{
		if( (*pIter)->GetValue<float>( READYWORKPERCENT ) >= 1 )
		{
			InventionFinished( *pIter );
		}
		else
		{
			PNrpCompany cmp = (*pIter)->GetValue<PNrpCompany>( PARENTCOMPANY );
			if( cmp )
				cmp->AddValue<int>( BALANCE, -(*pIter)->GetValue<int>( INVESTIMENT ) / 30 );
			(*pIter)->CheckParams();
		}
	} 
}

float GetConsumerAbility_( float price )
{
	if( price > 150 ) return 10 / price;

	if( price > 100 && price <= 150 ) return 20 / price;

	if( price > 50 && price <= 100) return 40 / price;

	if( price > 25 && price <= 50 ) return 50 / price;

	if( price > 10 && price <= 25) return 25 / price;
	else return 30 / price;
}

int CNrpApplication::GetFreePlatformNumberForGame_( CNrpGame* game )
{
	int yearRaznost = GetValue<SYSTEMTIME>( CURRENTTIME ).wYear - 1980;

	int summ = 5000;
	for( int k=0; k < yearRaznost; k++ )
		 summ += summ * yearRaznost;

	return summ * game->GetValue<int>( PLATFORMNUMBER );
}

int CNrpApplication::GetSalesNumber_( CNrpGame* game, CNrpCompany* cmp )
{
	GAMES_LIST::iterator pIter = games_.begin();
	//получим количество платформ на которых может быть продана игра
	int freePlatformNumber = GetFreePlatformNumberForGame_( game );
	
	//найдем количество игр этого жанра
	float gamesInThisGenre = 1;
	for( ; pIter != games_.end(); pIter++ )
	  if( (game != (*pIter)) && 
		  game->GetValue<bool>( GAMEISSALING ) &&
		  ((*pIter)->GetGenreName( 0 ) == game->GetGenreName( 0 )) )
		  gamesInThisGenre += (*pIter)->GetValue<int>( CURRENTGAMERATING ) / 100.f; 

	freePlatformNumber -= game->GetValue<int>( COPYSELL );
	float userModificator = 1, compannyFamous = 1;
	if( cmp )
	{
		userModificator = cmp->GetUserModificatorForGame( game );
		compannyFamous = cmp->GetValue< float>( FAMOUS ); 
	}

	float authorFamous = 1;
	authorFamous = game->GetAuthorFamous();
	std::string retailerName = game->GetValue<std::string>( GAMERETAILER );
	PNrpRetailer retailer = GetRetailer( retailerName );

	float retailerFamous = 0.1f;
	if( retailer )
		retailerFamous = retailer->GetValue<float>( FAMOUS );

	float genreInterest = GetGameGenreInterest( game );

	//столько игр может быть продано сегодн€
	freePlatformNumber /= 365;
	//веро€тность что покупатель обратит внимание на нашу игру после прочтени€ обзора в прессе
	int gameMaySaledToday = (int)((freePlatformNumber*genreInterest) / gamesInThisGenre);

	//повышение продаж игры за счет рекламы игры, известности авторов и личностных модификаторов
	gameMaySaledToday *= ( game->GetValue<float>( FAMOUS ) + userModificator + authorFamous );

	//коэффициент продаж по известности ретейлера и компании
	gameMaySaledToday *= (compannyFamous + retailerFamous)*0.5f;

	//коэффициент покупательской способности
	gameMaySaledToday *= GetConsumerAbility_( static_cast<float>( game->GetValue<PNrpGameBox>( GBOX )->GetValue<int>( PRICE ) ) );

	return gameMaySaledToday;
}

void CNrpApplication::UpdateMarketGames_()
{
	GAMES_LIST::iterator pIter = games_.begin();
	for( ; pIter != games_.end(); pIter++ )
	{
		if( !(*pIter)->GetValue<bool>( GAMEISSALING ) )
			continue;

		PNrpCompany cmp = (*pIter)->GetValue<PNrpCompany>( PARENTCOMPANY );
		int salesNumber = GetSalesNumber_( *pIter, cmp );

		(*pIter)->GameBoxSaling( salesNumber );
	}
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

template< class B > int GetQuality_( B* ptrObject )
{
	if( ptrObject )
		return ptrObject->GetValue<int>( QUALITY );
	return 0;
}

int CNrpApplication::GetGameRating_( CNrpGame* ptrGame, GAME_RATING_TYPE typeRating )
{
	CNrpCompany* cmp = ptrGame->GetValue<PNrpCompany>( PARENTCOMPANY );
	int rating = 0;
	int number = 0;

	rating = GetQuality_( cmp->GetGameEngine( ptrGame->GetValue<std::string>( GAME_ENGINE ) ) );

	for( int k=0; k < ptrGame->GetValue<int>( MODULE_NUMBER ); k++ )
	{
		std::string name = ptrGame->GetTechName( k );
		rating += GetQuality_( GetTechnology( name ) );
		rating /= 2;
	}

	//вычисл€ем сколько мес€цев на рынке игра
	SYSTEMTIME startTime = ptrGame->GetValue<SYSTEMTIME>( STARTDATE );
	SYSTEMTIME curTime = GetValue<SYSTEMTIME>( CURRENTTIME );
	double sT, cT;
	SystemTimeToVariantTime( &startTime, &sT );
	SystemTimeToVariantTime( &curTime, &cT );
	int monthInMarket = static_cast< int >( cT - sT ) / 30;
	//понижаем рейтинг из-за времени на рынке
	rating *= ( monthInMarket > 10 ? 0.1f : 1.f - log( (float)monthInMarket ) );

	//результат подсчета рейтинга
	//todo: надо както обходить рейтинг хитовых игр
	return rating;
}

void CNrpApplication::UpdateGameRatings( CNrpGame* ptrGame, bool firstTime )
{
		ptrGame->SetValue<int>( firstTime ? STARTGAMERATING : CURRENTGAMERATING, GetGameRating_( ptrGame, GRT_GENERAL ) );
	/*	ptrGame->SetValue<int>( STARTGRAPHICRATING, GetGameRating_( ptrGame, GRT_VIDEO ) );
		ptrGame->SetValue<int>( STARTGENRERATING, GetGameRating_( ptrGame, GRT_GENRE ) );
		ptrGame->SetValue<int>( STARTSOUNDRATING, GetGameRating_( ptrGame, GRT_SOUND ) );
		ptrGame->SetValue<int>( STARTADVFUNCRATING, GetGameRating_( ptrGame, GRT_ADVFUNC ) ); */
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
		ptrUser = GetUser( userName );
	} while ( ptrUser != NULL );

	ptrUser = new IUser( userType, userName );
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
	//начало мес€ца в компании
	for( COMPANIES_LIST::iterator cIter = companies_.begin();
		 cIter != companies_.end(); cIter++)
		(*cIter)->BeginNewMonth( GetValue<SYSTEMTIME>( CURRENTTIME ));

	//обновл€ем рейтинги игр
	for( GAMES_LIST::iterator pIter = games_.begin(); 
		 pIter != games_.end(); pIter++ )
		 if( (*pIter)->GetValue<bool>( GAMEISSALING ) )
			 UpdateGameRatings( *pIter );
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
	SetValue<int>( BOXADDONNUMBER, boxAddons_.size() );
}

CNrpTechnology* CNrpApplication::GetBoxAddon( std::string name )
{
	TECH_LIST::iterator tIter = boxAddons_.begin();
	for( ; tIter != boxAddons_.end(); tIter++)
		if( (*tIter)->GetValue<std::string>( NAME ) == name )
		return *tIter;

	return NULL;	
}

CNrpDiskMachine* CNrpApplication::GetDiskMachine( std::string name )
{
	DISKMACHINES_LIST::iterator dIter = diskMachines_.begin();
	for( ; dIter != diskMachines_.end(); dIter++)
		if( (*dIter)->GetValue<std::string>( NAME ) == name )
			return *dIter;

	return NULL;		
}

CNrpDiskMachine* CNrpApplication::GetDiskMachine( size_t index )
{
	return index < diskMachines_.size() ? diskMachines_[ index ] : NULL;
}

void CNrpApplication::AddDiskMachine( CNrpDiskMachine* pDm )
{
	diskMachines_.push_back( pDm );
	SetValue<int>( DISKMACHINENUMBER, diskMachines_.size() );
}

void CNrpApplication::AddGameToMarket( CNrpGame* game )
{
	assert( game != NULL );
	if( !game || game->GetValue<bool>( GAMEISSALING ) )
		return;

	game->SetValue<bool>( GAMEISSALING, true );

	//когда игра выходит на рынок, то она вли€ет на него
	for( int i=0; i <  game->GetValue<int>( GENRE_MODULE_NUMBER ); i++ )
	{
		std::string genreName = game->GetGenreName( i );
		//вли€ние приводит к изменению интереса к жанру игры
		CNrpTechnology* tech = GetTechnology( genreName );
		if( tech != NULL )
			tech->AddValue<float>( INTEREST, -game->GetValue<int>( CURRENTGAMERATING ) / 1000.f );
	}
}

//интерес к жанру мен€етс€ в противоположную сторону на 10% от рейтинга игры
float CNrpApplication::GetGameGenreInterest( CNrpGame* game )
{
	CNrpTechnology* tech = GetTechnology( game->GetGenreName( 0 ) );
	float summ = tech != NULL ? tech->GetValue<float>( INTEREST ) : 0.1f;
	int gm = game->GetValue<int>( GENRE_MODULE_NUMBER );
	for( int i=1; i < gm; i++ )
	{
		tech = GetTechnology( game->GetGenreName( i ) );	
		summ += ( tech != NULL ? (tech->GetValue<float>( INTEREST ) / i) : 0 );
	}

	//надо скомпенсировать понижение интереса к жанру, которое принесла
	//игра при размещении на рынке
	return (summ + game->GetValue<int>( CURRENTGAMERATING ) / 1000.f );
}

CNrpRetailer* CNrpApplication::GetRetailer( std::string name )
{
	return NULL;
}

//получение имени изображени€ с которым будет дальше св€зана игра
std::string CNrpApplication::GetFreeInternalName( CNrpGame* game )
{
	std::vector< CNrpGameImageList* > thisYearAndGenreImgs;
	
	for( GAMEIMAGES_MAP::iterator pIter = gameImages_.begin();
		 pIter != gameImages_.end(); 
		 pIter++ )
		if( !pIter->second->GetValue<bool>( ISUSED ) && 
			pIter->second->GetValue<std::string>( GENRETECH ) == game->GetGenreName( 0 ) && 
			pIter->second->GetValue<int>( YEAR ) == GetValue<SYSTEMTIME>( CURRENTTIME ).wYear )
			thisYearAndGenreImgs.push_back( pIter->second );

	if( thisYearAndGenreImgs.size() )
	{
		int randomIndex = rand() % thisYearAndGenreImgs.size();
		thisYearAndGenreImgs[ randomIndex ]->SetValue<bool>( ISUSED, true );

		return thisYearAndGenreImgs[ randomIndex ]->GetValue<std::string>( NAME );
	}

	return "";
}

CNrpGameImageList* CNrpApplication::GetGameImageList( std::string name )
{
	GAMEIMAGES_MAP::iterator pIter = gameImages_.begin();
	for( ; pIter != gameImages_.end(); pIter++ )
		if(	pIter->second->GetValue<std::string>( NAME ) == name )
			return pIter->second;
	return NULL;
}

void CNrpApplication::AddGameImageList( CNrpGameImageList* pGList )
{
	gameImages_[ pGList->GetValue<std::string>( NAME ) ] = pGList;
}

void CNrpApplication::ClearImageList()
{
	GAMEIMAGES_MAP::iterator pIter = gameImages_.begin();
	for( ; pIter != gameImages_.end(); pIter++ )
		delete pIter->second;

	gameImages_.clear();
}

CNrpGame* CNrpApplication::GetGame( const std::string& name )
{
	GAMES_LIST::iterator pIter = games_.begin();
	for( ; pIter != games_.end(); pIter++ )
		if( (*pIter)->GetValue<std::string>( NAME ) == name )
			return *pIter;

	return NULL;
}

CNrpGame* CNrpApplication::GetGame( size_t index )
{
	assert( index < games_.size() );
	return index < games_.size() ? games_[ index ] : NULL;
}

void CNrpApplication::RemoveTechnology( CNrpTechnology* ptrTech )
{
	TECH_LIST::iterator pIter = technologies_.begin();

	for( ; pIter != technologies_.end(); pIter++ )
		if( *pIter == ptrTech )
		{
			technologies_.erase( pIter );
			return;
		}

	OutputDebugString( "unknown technology" );
}

void CNrpApplication::AddGameEngine( nrp::CNrpGameEngine* ptrEngine )
{
	if( engines_.find( ptrEngine->GetValue<std::string>( NAME ) ) == engines_.end() )
	{
		engines_[ ptrEngine->GetValue<std::string>( NAME ) ] = ptrEngine;
		SetValue<int>( ENGINES_NUMBER, engines_.size() );
		return;
	}

	OutputDebugString( "такой движок уже есть" );
}

INrpProject* CNrpApplication::GetProject( const std::string& name )
{
	PROJECTS_MAP::iterator pIter = projects_.find( name );
	return pIter != projects_.end() ? pIter->second : NULL;
}

void CNrpApplication::AddDevelopProject( nrp::INrpProject* project )
{
	devProjects_[ project->GetValue<std::string>( NAME ) ] = project;
	SetValue<int>( DEVELOPPROJECTS_NUMBER, devProjects_.size() );
}

INrpProject* CNrpApplication::GetDevelopProject( const std::string&  name ) const
{
	PROJECTS_MAP::const_iterator pIter = devProjects_.find( name );
	if( pIter != devProjects_.end() )
		return pIter->second;

	return NULL;
}

void CNrpApplication::RemoveDevelopProject( const std::string& name )
{
	PROJECTS_MAP::iterator pIter = devProjects_.find( name );
	if( pIter != devProjects_.end() )
	{
		delete pIter->second;
		devProjects_.erase( pIter );
	}
	else
		throw ( "не удалось найти запрошенный объект" );
}

void CNrpApplication::AddGame( CNrpGame* ptrGame )
{
	assert( ptrGame != NULL );
	games_.push_back( ptrGame );
	SetValue<int>( GAMENUMBER, games_.size() );
}

void CNrpApplication::AddInvention( const std::string& name, CNrpCompany* parentCompany )
{
	CNrpTechnology* startTech = new CNrpTechnology( PROJECT_TYPE(0) );
	startTech->Load( "xtras/technology/" + name + "/item.tech" );

	CNrpInvention* inv = new CNrpInvention( startTech, parentCompany );
	parentCompany->AddInvention( inv );	
	inventions_.push_back( inv );  
	SetValue<int>( INVENTIONSNUMBER, inventions_.size() );

	delete startTech;
}

void CNrpApplication::InventionFinished( CNrpInvention* ptrInvention )
{
	//создать соответствующую изобретению технологию 
	//разместить еЄ в списке доступных
	CNrpTechnology* tech = new CNrpTechnology( *ptrInvention );
	AddTechnology( tech );

	INVENTION_LIST::iterator pIter = inventions_.begin();
	INVENTION_LIST::iterator delIter;
	for( ; pIter != inventions_.end(); pIter++ )
	{
		if( (*pIter)->GetValue<std::string>( NAME ) == ptrInvention->GetValue<std::string>( NAME ) )
		{
			PNrpCompany pCmp = (*pIter)->GetValue<PNrpCompany>( PARENTCOMPANY );
			if( pCmp == ptrInvention->GetValue<PNrpCompany>( PARENTCOMPANY) )
			{
				delIter = pIter;//найти это изобретение в своем списке и удалить его оттуда...
				pCmp->AddTechnology( tech );
				pCmp->RemoveInvention( ptrInvention );
				continue;
			}
		
			pCmp->InventionReleased( ptrInvention );//уведомить все компании об изобретении технологии
		}
	}

	DoLuaFunctionsByType( APP_INVENTION_FINISHED, tech );
	
	delete *delIter;
	inventions_.erase( delIter );
}

void CNrpApplication::InventionCanceled( CNrpInvention* ptrInvention )
{
	INVENTION_LIST::iterator pIter = inventions_.begin();
	for( ; pIter != inventions_.end(); pIter++ )
	{
		if( *pIter == ptrInvention )
		{
#ifdef _DEBUG
			std::string text = "”далена технологи€";
			text += (*pIter)->GetValue<std::string>( NAME );
			PNrpCompany cmp = (*pIter)->GetValue<PNrpCompany>( PARENTCOMPANY );
			assert( cmp != NULL );
			text += cmp ? cmp->GetValue<std::string>( NAME ) : std::string("unknown company");
			OutputDebugString( text.c_str() );
#endif
			delete *pIter;
			inventions_.erase( pIter );
			break;
		}
	}
}

CNrpInvention* CNrpApplication::GetInvention( std::string name, std::string companyName )
{
	INVENTION_LIST::iterator pIter = inventions_.begin();
	for( ; pIter != inventions_.end(); pIter++ )
	{
		if( ((*pIter)->GetValue<std::string>( NAME ) == name ||
			 (*pIter)->GetValue<std::string>( INTERNAL_NAME ) == name )
			 && (*pIter)->GetValue<std::string>( COMPANYNAME ) == companyName )
				return *pIter;
	}

	return NULL;
}
}//namespace nrp