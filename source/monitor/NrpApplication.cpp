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
#include "NrpActionType.h"
#include "NrpPda.h"
#include "NrpHistory.h"

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
	time.wMonth = 1;
	time.wDay = 1;
	time.wHour = 0;
	time.wMinute = 0;
	lastTimeUpdate_ = 0;

	CreateValue<PNrpBank>( BANK, NULL );
	CreateValue<int>( TECHNUMBER, 0 );
	CreateValue<int>( USERNUMBER, 0 );
	CreateValue<int>( COMPANIESNUMBER, 0 );

	CreateValue<std::string>( WORKDIR, "" );
	CreateValue<std::string>( SAVEDIR, "save/" );
	CreateValue<std::string>( SAVEDIR_INVENTIONS, "" );
	CreateValue<std::string>( SAVEDIR_COMPANIES, "" );
	CreateValue<std::string>( SAVEDIR_DEVPR, "" );
	CreateValue<std::string>( SAVEDIR_GAMES, "" );
	CreateValue<std::string>( SAVEDIR_PROJECTS, "" );
	CreateValue<std::string>( SAVEDIR_ENGINES, "" );
	CreateValue<std::string>( SAVEDIR_USERS, "" );
	CreateValue<std::string>( SAVEDIR_PLANT, "" );
	CreateValue<std::string>( SAVEINI_PROFILE, "" );
	CreateValue<std::string>( SAVEDIR_PROFILE, "" );
	CreateValue<std::string>( SAVEDIR_TECHS, "" );
	CreateValue<std::string>( SYSTEMINI, "config/system.ini" );
	CreateValue<std::string>( PROFILENAME, IniFile::Read( SECTION_OPTIONS, "currentProfile", std::string( "dalerank" ), GetString( SYSTEMINI ) ) );
	CreateValue<std::string>( PROFILECOMPANY, IniFile::Read( SECTION_OPTIONS, "currentCompany", std::string( "daleteam" ), GetString( SYSTEMINI ) ) );

	CreateValue<SYSTEMTIME>( CURRENTTIME, time );
	CreateValue<int>( DISKMACHINENUMBER, 0 );
	CreateValue<int>( BOXADDONNUMBER, 0 );
	CreateValue<int>( GAMENUMBER, 0 );
	CreateValue<int>( ENGINES_NUMBER, 0 );
	CreateValue<int>( DEVELOPPROJECTS_NUMBER, 0 );
	CreateValue<int>( PROJECTNUMBER, 0 );
	CreateValue<PNrpCompany>( PLAYERCOMPANY, NULL );
	CreateValue<int>( INVENTIONSNUMBER, 0 );
	CreateValue<int>( MINIMUM_USER_SALARY, 250 );
	CreateValue<CNrpPda*>( PDA, new CNrpPda() );

	srand( GetTickCount() );
}

CNrpApplication::~CNrpApplication(void)
{
}

CNrpCompany* CNrpApplication::GetCompany( std::string companyName ) const
{
	COMPANIES_LIST::const_iterator pIter = companies_.begin();

	for( ; pIter != companies_.end(); pIter++)
		if( (*pIter)->GetString( NAME ) == companyName )
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
	if( ceo && ceo->ObjectTypeName() == CNrpPlayer::ClassName() )
		SetValue<PNrpCompany>( PLAYERCOMPANY, company );

	return 1;
}

int CNrpApplication::AddUser( IUser* user )
{
	IUser* tmpUser = GetUser( user->GetString( NAME ) );
	assert( tmpUser == NULL );

	if( user && tmpUser == NULL )
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
				DoLuaFunctionsByType( APP_DAY_CHANGE, this );
				BeginNewDay_();
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
				DoLuaFunctionsByType( APP_MONTH_CHANGE, this );
				BeginNewMonth_();
			}
		}

		if( spd == SPD_MONTH )
		{
			time.wMonth++;
			if( time.wMonth > 12 )
			{
				time.wMonth = 1;
				time.wYear++;
				DoLuaFunctionsByType( APP_YEAR_CHANGE, this );
			}
		}

		return true;
	}

	return false;
}

void CNrpApplication::AddProject( INrpProject* project )
{
	projects_[ project->GetString( NAME ) ] = project;
}

CNrpTechnology* CNrpApplication::GetTechnology( int index ) const
{
	return index < (int)technologies_.size() ?  technologies_[ index ] : NULL;
}

CNrpTechnology* CNrpApplication::GetTechnology( const std::string& name ) const
{
	TECH_LIST::const_iterator pIter = technologies_.begin();

	for( ; pIter != technologies_.end(); pIter++ )
		if( (*pIter)->GetString( NAME ) == name ||
			(*pIter)->GetString( INTERNAL_NAME ) == name )
			return (*pIter);

	return NULL;
}

void CNrpApplication::AddTechnology( CNrpTechnology* ptrTech )
{
	CNrpTechnology* tech = GetTechnology( ptrTech->GetString( INTERNAL_NAME) );
	if( !tech )
		technologies_.push_back( ptrTech );

	SetValue<int>( TECHNUMBER, technologies_.size() );
}

IUser* CNrpApplication::GetUser( int index ) const
{
	return index < (int)users_.size() ? users_[ index ] : NULL;
}

IUser* CNrpApplication::GetUser( const std::string& name ) const
{
	USER_LIST::const_iterator pIter = users_.begin();
	for( ; pIter != users_.end(); pIter++ )
		if( (*pIter)->GetString( NAME ) == name )
			return (*pIter);

	return NULL;	
}

int CNrpApplication::RemoveUser( IUser* user )
{
	USER_LIST::iterator pIter = users_.begin();
	for( ; pIter != users_.end(); pIter++ )
		if( (*pIter) == user )
		{
			delete *pIter;
			users_.erase( pIter );
			SetValue<int>( USERNUMBER, users_.size() );
			return 0;
		}

	return 1;
}

void CNrpApplication::_CreateDirectoriesMapForSave()
{
	OpFileSystem::CreateDirectory( GetString( SAVEDIR_PROFILE ) );
	OpFileSystem::CreateDirectory( GetString( SAVEDIR_INVENTIONS ) );
	OpFileSystem::CreateDirectory( GetString( SAVEDIR_COMPANIES ) );
	OpFileSystem::CreateDirectory( GetString( SAVEDIR_DEVPR ) );
	OpFileSystem::CreateDirectory( GetString( SAVEDIR_GAMES ) );
	OpFileSystem::CreateDirectory( GetString( SAVEDIR_PROJECTS ) );
	OpFileSystem::CreateDirectory( GetString( SAVEDIR_ENGINES ) );
	OpFileSystem::CreateDirectory( GetString( SAVEDIR_PLANT ) );
	OpFileSystem::CreateDirectory( GetString( SAVEDIR_USERS ) );
}

void CNrpApplication::SaveProfile()
{
	std::string prevSaveFolder = GetString( SAVEDIR ) + GetString( PROFILENAME ) + "Old/";
	std::string profileIni = GetString( SAVEDIR_PROFILE ) + "profile.ini";

	OpFileSystem::Remove( prevSaveFolder );
	OpFileSystem::Move( GetString( SAVEDIR_PROFILE ), prevSaveFolder );

	_CreateDirectoriesMapForSave();

	assert( !OpFileSystem::IsExist( profileIni ) );
	INrpConfig::Save( profileIni );

	IniFile::Write( SECTION_OPTIONS, "currentProfile", GetString( PROFILENAME ), GetString( SYSTEMINI ) );
	IniFile::Write( SECTION_OPTIONS, "currentCompany", GetString( PROFILECOMPANY ), GetString( SYSTEMINI ) );
	
	COMPANIES_LIST::iterator cIter = companies_.begin();
	for( int i=0; cIter != companies_.end(); cIter++, i++ )
	{
		(*cIter)->Save( GetString( SAVEDIR_COMPANIES ) );
		IniFile::Write( SECTION_COMPANIES, KEY_COMPANY( i ), (*cIter)->GetString( NAME ), profileIni );
	}

	DEVPROJECTS_MAP::iterator dIter = devProjects_.begin();
	for( int i=0; dIter != devProjects_.end(); dIter++, i++ )
	{
		dIter->second->Save( GetString( SAVEDIR_DEVPR ) );
		IniFile::Write( SECTION_DEVPROJECTS, KEY_TYPE(i), dIter->second->ObjectTypeName(), profileIni );
		IniFile::Write( SECTION_DEVPROJECTS, KEY_PROJECT(i), dIter->first, profileIni );
	}

	PROJECTS_MAP::iterator ppIter = projects_.begin();
	for( int i=0; ppIter != projects_.end(); ppIter++, i++ )
	{
		ppIter->second->Save( GetString( SAVEDIR_PROJECTS ) + ppIter->first + ".prj" );
		IniFile::Write( SECTION_PROJECTS, KEY_PROJECT(i), ppIter->second->ObjectTypeName() + ":" + ppIter->first, profileIni );
	}

	USER_LIST::iterator uIter = users_.begin();
	for( int i=0; uIter != users_.end(); uIter++, i++ )
	{
		(*uIter)->Save( GetString( SAVEDIR_USERS ) );

		IniFile::Write( SECTION_USERS, KEY_TYPE(i), (*uIter)->ObjectTypeName() , profileIni );
		IniFile::Write( SECTION_USERS, KEY_USER(i), (*uIter)->GetString( NAME ), profileIni );
	}

	TECH_LIST::iterator tIter = technologies_.begin();
	for( int i=0; tIter != technologies_.end(); tIter++, i++ )
	{
		std::string saveFile = (*tIter)->Save( GetString( SAVEDIR_TECHS ) );
		IniFile::Write( SECTION_TECHS, KEY_TECH(i), saveFile, profileIni );
	}

	INVENTION_LIST::iterator invIter = inventions_.begin();
	for( int i=0; invIter != inventions_.end(); invIter++ )
	{
		std::string inventSaveFile = (*invIter)->Save( GetString( SAVEDIR_INVENTIONS ) + (*invIter)->GetString( COMPANYNAME ), true );
		IniFile::Write( SECTION_INVENTIONS, KEY_INVENTION( i ), 
						inventSaveFile,	profileIni );
	}

	GAMEENGINES_LIST::iterator eIter = engines_.begin();
	for( int i=0; eIter != engines_.end(); eIter++, i++ )
	{
		std::string saveFolder = (*eIter)->Save( GetString( SAVEDIR_ENGINES ) );
		IniFile::Write( SECTION_ENGINES, KEY_ENGINE(i), saveFolder, profileIni );
	}

	GAMES_LIST::iterator gameIter = games_.begin();
	for( int i=0; gameIter != games_.end(); gameIter++, i++ )
	{
		std::string saveDir = (*gameIter)->Save( GetString( SAVEDIR_GAMES ) );
		IniFile::Write( SECTION_GAMES, KEY_GAME( i ), saveDir, profileIni );
	}

	std::string imageListIni = GetString( SAVEDIR_PROFILE ) + "imageList.ini";
	GAMEIMAGES_MAP::iterator gameListIter = gameImages_.begin();
	int gli = 0;
	for( ; gameListIter != gameImages_.end(); gameListIter++ )
		if( !gameListIter->second->GetValue<bool>( ISUSED ) )
		{
			IniFile::Write( "imageList", "imageList_" + IntToStr( gli ), 
							gameListIter->second->GetString( NAME ), imageListIni );
			gli++;
		}

	IniFile::Write( SECTION_PROPERTIES, "ImageListNumber", gli, imageListIni );
}

void CNrpApplication::_LoadUsers( const std::string& iniFile )
{
	int maxUser = GetValue<int>( USERNUMBER );
	for( int i=0; i < maxUser; i++ )
	{
		std::string name = IniFile::Read( SECTION_USERS, KEY_USER(i), std::string(""), iniFile );
		std::string className = IniFile::Read( SECTION_USERS, KEY_TYPE(i), std::string(""), iniFile );

		std::string fileName = GetString( SAVEDIR_USERS ) + name + ".user";
		
		IUser* user = NULL;
		if( className == CNrpPlayer::ClassName() ) 
			user = new CNrpPlayer( fileName );
		else
		{
			if( className == CNrpAiUser::ClassName() )
				user = new CNrpAiUser( fileName );
			else
			{
				user = new IUser( className, "" );
				user->Load( fileName );
			}
		}

		AddUser( user );
	}
}

void CNrpApplication::_InitialyzeSaveDirectories( const std::string& profileName )
{
	std::string profileDir = OpFileSystem::CheckEndSlash( GetString( SAVEDIR ) + profileName );
	SetString( SAVEDIR_PROFILE, profileDir );
	SetString( SAVEDIR_INVENTIONS, profileDir + "inventions/" );
	SetString( SAVEDIR_COMPANIES, profileDir + "companies/" );
	SetString( SAVEDIR_DEVPR, profileDir + "devProjects/" );
	SetString( SAVEDIR_GAMES, profileDir + "games/");
	SetString( SAVEDIR_ENGINES, profileDir + "engines/" );
	SetString( SAVEDIR_PROJECTS, profileDir + "projects/" );
	SetString( SAVEDIR_PLANT, profileDir + "plant/" );
	SetString( SAVEDIR_USERS, profileDir + "users/" );
	SetString( SAVEINI_PROFILE, profileDir + "profile.ini" );
	SetString( SAVEDIR_TECHS, profileDir + "techs/" );
}

void CNrpApplication::LoadProfile( const std::string& profileName, const std::string& companyName )
{
	_InitialyzeSaveDirectories( profileName );

	CNrpScript::Instance().TemporaryScript( AFTER_LOAD_SCRIPT, CNrpScript::SA_CREATE );

	std::string profileIni = GetString( SAVEINI_PROFILE );
	INrpConfig::Load( profileIni );

	_LoadUsers( profileIni );	

	for( int i=0; i < GetValue<int>( TECHNUMBER ); i++ )
	{
		std::string fileName = IniFile::Read( SECTION_TECHS, KEY_TECH(i), std::string(""), profileIni );
		CNrpTechnology* tech = new CNrpTechnology( fileName ); //loading
		technologies_.push_back( tech );
	}

	for( int i=0; i < GetValue<int>( ENGINES_NUMBER ); i++ )
	{
		std::string saveFolder = IniFile::Read( SECTION_ENGINES, KEY_ENGINE(i), std::string(""), profileIni );
		engines_.push_back( new CNrpGameEngine( saveFolder, true ) );
	}

	for( int i=0; i < GetValue<int>( DEVELOPPROJECTS_NUMBER ); i++ )
	{
		std::string type = IniFile::Read( SECTION_DEVPROJECTS, KEY_TYPE( i ), std::string(""), profileIni );
		std::string name = IniFile::Read( SECTION_DEVPROJECTS, KEY_PROJECT( i ), std::string(""), profileIni );
		if( type == CNrpDevelopGame::ClassName() )
		{
			CNrpDevelopGame* game = new CNrpDevelopGame( GetString( SAVEDIR_DEVPR ) + name );
			devProjects_[ game->GetString( NAME ) ] = game;
		}
		else if( type == "devengine" )
		{
			//
		}
	}

	for( int i=0; i < GetValue<int>( PROJECTNUMBER ); i++ )
	{
		std::string fileName = IniFile::Read( SECTION_PROJECTS, KEY_PROJECT( i ), std::string(""), profileIni );
		std::string type = IniFile::Read( SECTION_PROJECTS, KEY_TYPE( i ), std::string(""), profileIni );
		if( type == CNrpGameProject::ClassName() )
		{
			CNrpDevelopGame* game = new CNrpDevelopGame( GetString( SAVEDIR_PROJECTS ) + fileName );
			devProjects_[ game->GetString( NAME ) ] = game;
		}
		else if( type == "devengine" )
		{
			//
		}
	}

	for( int i=0; i < GetValue<int>( GAMENUMBER ); i++ )
	{
		std::string fileName = IniFile::Read( SECTION_GAMES,  KEY_GAME( i ), std::string(""), profileIni );
		PNrpGame game = new CNrpGame( fileName );
		games_.push_back( game );
	}

	for( int k=0; k < GetValue<int>( INVENTIONSNUMBER ); k++ )
	{
		std::string saveFile = IniFile::Read( SECTION_INVENTIONS, KEY_INVENTION( k ), std::string(""), profileIni );
		CNrpInvention* invention = new CNrpInvention( saveFile );
		inventions_.push_back( invention );
	}
	SetValue<int>( INVENTIONSNUMBER, inventions_.size() );

	for( int k=0; k < GetValue<int>( COMPANIESNUMBER ); k++ )
	{
		std::string fileName = IniFile::Read( SECTION_COMPANIES, KEY_COMPANY( k ), std::string(""), profileIni );
		CNrpCompany* cmp = new CNrpCompany( GetString( SAVEDIR_COMPANIES ) + fileName );
		AddCompany( cmp );
	}
	SetValue<int>( GAMENUMBER, games_.size() );

    LoadFreeImageLists_( GetString( SAVEDIR_PROFILE ) + "imageList.ini" );
	CNrpScript::Instance().TemporaryScript( AFTER_LOAD_SCRIPT, CNrpScript::SA_EXEC );
}

void CNrpApplication::LoadFreeImageLists_( const std::string& fileName )
{
	size_t imageListNumber = IniFile::Read( SECTION_PROPERTIES, "ImageListNumber", (int)0, fileName );

	if( OpFileSystem::IsExist( fileName ) && imageListNumber > 0)
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

void CNrpApplication::CreateProfile( const std::string& profileName, const std::string& companyName )
{
	SetString( PROFILENAME, profileName );
	SetString( PROFILECOMPANY, companyName );

	_InitialyzeSaveDirectories( profileName );
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

template< typename ARRAY_TYPE, typename OBJECT_TYPE >  
OBJECT_TYPE* _FindByNamesIn( const ARRAY_TYPE& arrayT, const std::string& someName )
{
	ARRAY_TYPE::const_iterator arIter = arrayT.begin();
	for( ; arIter != arrayT.end(); arIter++ )
	{
		if( ((*arIter)->GetString( NAME ) == someName ||
			(*arIter)->GetString( INTERNAL_NAME ) == someName ) )
			return *arIter;
	}

	return NULL;
}

CNrpGameEngine* CNrpApplication::GetGameEngine( const std::string& name ) const
{
	return _FindByNamesIn<GAMEENGINES_LIST, CNrpGameEngine>( engines_, name );
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
	for( size_t k=0; k < inventions_.size(); k++ )
	{
		inventions_[ k ]->CheckParams();

		if( inventions_[ k ]->GetValue<float>( READYWORKPERCENT ) >= 1 )
		{
			inventions_[ k ]->SetValue<SYSTEMTIME>( ENDDATE, GetValue<SYSTEMTIME>( CURRENTTIME ) );
			InventionFinished( inventions_[ k ] );
			k--;
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
	if( !cmp )
		return 0;

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
	std::string retailerName = game->GetString( GAMERETAILER );
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
	gameMaySaledToday *= (compannyFamous + retailerFamous) * 0.5f;

	//коэффициент покупательской способности
	if( game->GetValue<PNrpGameBox>( GBOX ) )
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

	USER_LIST coders, designer, composer, tester;
	std::map< std::string, USER_LIST* > group;
	group[ "coder" ] = &coders;
	group[ "designer" ] = &designer;
	group[ "composer" ] = &composer;
	group[ "tester" ] = &tester;
	
	for( ; pIter != users_.end(); pIter++ )
	{
		std::string typeName = (*pIter)->ObjectTypeName();
		if( (*pIter)->GetValue<PNrpCompany>( PARENTCOMPANY ) != NULL )
			continue;

		if( group.find( typeName ) != group.end() )
			group[ typeName ]->push_back( *pIter );
	}

	size_t USER_GROUP_COUNT = 6;

	std::map< std::string, USER_LIST* >::iterator gIter = group.begin();
	for( ; gIter != group.end(); gIter++ )
	{
		USER_LIST& tmpList = *(gIter->second);
		for( size_t k=0; k < tmpList.size(); k++ )
			RemoveUser( tmpList[ k ] );

		tmpList.clear();

		for( size_t cnt=tmpList.size(); cnt < USER_GROUP_COUNT; cnt++ )
			tmpList.push_back( CreateRandomUser_( gIter->first ) );
	}

	gIter = group.begin();
	for( ; gIter != group.end(); gIter++ )
		for(  size_t cnt=0; cnt < gIter->second->size(); cnt++ )
			users_.push_back( gIter->second->at( cnt ) );

	if( GetValue<int>( USERNUMBER ) != users_.size() )
		DoLuaFunctionsByType<void>( APP_USER_MARKETUPDATE, NULL );

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

void CNrpApplication::_UpdateGameRating( CNrpGame* ptrGame, GAME_RATING_TYPE typeRating )
{
	int rating = 0;

	CNrpCompany* cmp = ptrGame->GetValue<PNrpCompany>( PARENTCOMPANY );
	if( cmp != NULL )
	{		
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
		ptrGame->GetHistory()->GetStep( GetValue<SYSTEMTIME>( CURRENTTIME ) )->SetValue<int>( CURRENTGAMERATING, rating );
	}
}

void CNrpApplication::UpdateGameRatings( CNrpGame* ptrGame, bool firstTime )
{
		_UpdateGameRating( ptrGame, GRT_GENERAL );
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

	if( ptrUser->GetValue<int>( WANTMONEY ) < GetValue<int>( MINIMUM_USER_SALARY ) )
		ptrUser->SetValue<int>( WANTMONEY, GetValue<int>( MINIMUM_USER_SALARY ) );

	char name[64] = { 0 };
	snprintf( name, 64, "media/face/face%03d.png", rand() % 2 );
	ptrUser->SetString( TEXTURENORMAL, name );

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
		if( (*tIter)->GetString( NAME ) == name )
		return *tIter;

	return NULL;	
}

CNrpDiskMachine* CNrpApplication::GetDiskMachine( std::string name )
{
	DISKMACHINES_LIST::iterator dIter = diskMachines_.begin();
	for( ; dIter != diskMachines_.end(); dIter++)
		if( (*dIter)->GetString( NAME ) == name )
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
			tech->AddValue<float>( INTEREST, -game->GetValue<int>( STARTGAMERATING ) / 1000.f );
	}

	games_.push_back( game );
	SetValue( GAMENUMBER, games_.size() );
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

		return thisYearAndGenreImgs[ randomIndex ]->GetString( NAME );
	}

	return "";
}

CNrpGameImageList* CNrpApplication::GetGameImageList( std::string name )
{
	GAMEIMAGES_MAP::iterator pIter = gameImages_.begin();
	for( ; pIter != gameImages_.end(); pIter++ )
		if(	pIter->second->GetString( NAME ) == name )
			return pIter->second;
	return NULL;
}

void CNrpApplication::AddGameImageList( CNrpGameImageList* pGList )
{
	gameImages_[ pGList->GetString( NAME ) ] = pGList;
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
		if( (*pIter)->GetString( NAME ) == name ||
			(*pIter)->GetString( INTERNAL_NAME ) == name )
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

	Log(HW) << "unknown technology" << term;
}

void CNrpApplication::AddGameEngine( nrp::CNrpGameEngine* ptrEngine )
{
	if( GetGameEngine( ptrEngine->GetString( INTERNAL_NAME ) ) == NULL )
	{
		engines_.push_back( ptrEngine );
		SetValue<int>( ENGINES_NUMBER, engines_.size() );
		return;
	}

	Log( HW ) << "такой движок уже есть" << term;
}

INrpProject* CNrpApplication::GetProject( const std::string& name )
{
	PROJECTS_MAP::iterator pIter = projects_.find( name );
	return pIter != projects_.end() ? pIter->second : NULL;
}

void CNrpApplication::AddDevelopProject( nrp::INrpDevelopProject* project )
{
	devProjects_[ project->GetString( NAME ) ] = project;
	SetValue<int>( DEVELOPPROJECTS_NUMBER, devProjects_.size() );
}

INrpDevelopProject* CNrpApplication::GetDevelopProject( const std::string&  name ) const
{
	DEVPROJECTS_MAP::const_iterator pIter = devProjects_.find( name );
	if( pIter != devProjects_.end() )
		return pIter->second;

	return NULL;
}

void CNrpApplication::RemoveDevelopProject( const std::string& name )
{
	DEVPROJECTS_MAP::iterator pIter = devProjects_.find( name );
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
	CNrpInvention* tmp = GetInvention( name, parentCompany->GetValue<std::string>( NAME ) );

	if( tmp == NULL )
	{
		CNrpTechnology* startTech = new CNrpTechnology( "xtras/technology/" + name + "/item.tech" );

		CNrpInvention* inv = new CNrpInvention( startTech, parentCompany );
		parentCompany->AddInvention( inv );	
		inventions_.push_back( inv );  
		SetValue<int>( INVENTIONSNUMBER, inventions_.size() );

		delete startTech;
	}
}

void CNrpApplication::InventionFinished( CNrpInvention* ptrInvention )
{
	//создать соответствующую изобретению технологию 
	//разместить еЄ в списке доступных
	CNrpTechnology* tech = new CNrpTechnology( ptrInvention );
	AddTechnology( tech );

	INVENTION_LIST::iterator pIter = inventions_.begin();
	INVENTION_LIST::iterator delIter;
	for( ; pIter != inventions_.end(); pIter++ )
	{
		if( (*pIter)->GetString( NAME ) == ptrInvention->GetString( NAME ) )
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
			text += (*pIter)->GetString( NAME );
			PNrpCompany cmp = (*pIter)->GetValue<PNrpCompany>( PARENTCOMPANY );
			assert( cmp != NULL );
			text += cmp ? cmp->GetString( NAME ) : std::string("unknown company");
			Log( HW ) << text << term;
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
		if( ((*pIter)->GetString( NAME ) == name ||
			 (*pIter)->GetString( INTERNAL_NAME ) == name )
			 && (*pIter)->GetString( COMPANYNAME ) == companyName )
				return *pIter;
	}

	return NULL;
}

}//namespace nrp