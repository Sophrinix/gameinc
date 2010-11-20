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
#include "NrpScreenshot.h"
#include "NrpDevelopGame.h"
#include "NrpInvention.h"
#include "NrpActionType.h"
#include "NrpPda.h"
#include "NrpHistory.h"
#include "NrpGameTime.h"
#include "timeHelpers.h"
#include "IniFile.h"

#include <io.h>
#include <errno.h>
#include <OleAuto.h>

static nrp::CNrpApplication* globalApplication = NULL;

namespace nrp
{
CLASS_NAME CLASS_NRPAPPLICATION( "CNrpApplication" );

CNrpApplication::CNrpApplication(void) : INrpConfig( CLASS_NRPAPPLICATION, CLASS_NRPAPPLICATION )
{
	Push<PNrpBank>( BANK, NULL );
	Push<int>( TECHNUMBER, 0 );
	Push<int>( USERNUMBER, 0 );
	Push<int>( COMPANIESNUMBER, 0 );

	Push<NrpText>( WORKDIR, "" );
	Push<NrpText>( SAVEDIR, "save/" );
	Push<NrpText>( SAVEDIR_INVENTIONS, "" );
	Push<NrpText>( SAVEDIR_COMPANIES, "" );
	Push<NrpText>( SAVEDIR_DEVPR, "" );
	Push<NrpText>( SAVEDIR_GAMES, "" );
	Push<NrpText>( SAVEDIR_PROJECTS, "" );
	Push<NrpText>( SAVEDIR_ENGINES, "" );
	Push<NrpText>( SAVEDIR_USERS, "" );
	Push<NrpText>( SAVEDIR_PLANT, "" );
	Push<NrpText>( SAVEINI_PROFILE, "" );
	Push<NrpText>( SAVEDIR_PROFILE, "" );
	Push<NrpText>( SAVEDIR_TECHS, "" );
	Push<NrpText>( SYSTEMINI, "config/system.ini" );

	IniFile rv( (NrpText)Param( SYSTEMINI ) );
	Push<NrpText>( PROFILENAME, rv.Get( SECTION_OPTIONS, "currentProfile", NrpText( "dalerank" ) ) );
	Push<NrpText>( PROFILECOMPANY, rv.Get( SECTION_OPTIONS, "currentCompany", NrpText( "daleteam" ) ) );

	Push<SYSTEMTIME>( CURRENTTIME, SYSTEMTIME() );
	Push<int>( BOXADDONNUMBER, 0 );
	Push<int>( GAMENUMBER, 0 );
	Push<int>( ENGINES_NUMBER, 0 );
	Push<int>( DEVELOPPROJECTS_NUMBER, 0 );
	Push<int>( PROJECTNUMBER, 0 );
	Push<PNrpCompany>( PLAYERCOMPANY, NULL );
	Push<int>( INVENTIONSNUMBER, 0 );
	Push<int>( MINIMUM_USER_SALARY, 250 );
	Push<CNrpPda*>( PDA, new CNrpPda() );
	Push<CNrpGameTime*>( GAME_TIME, new CNrpGameTime( this ) );
	Push<int>( PAUSEBTWSTEP, 100 );

	srand( GetTickCount() );
}

CNrpApplication::~CNrpApplication(void)
{
}

CNrpCompany* CNrpApplication::GetCompany( const NrpText& companyName )
{
	return FindByName< COMPANIES, CNrpCompany >( _companies, companyName );
}

CNrpCompany* CNrpApplication::GetCompany( u32 index )
{
	return index < _companies.size() ? _companies[ index ] : NULL;
}

COMPANIES& CNrpApplication::GetCompanies()
{
	return _companies;
}

int CNrpApplication::AddCompany( CNrpCompany* company )
{
	assert( company );
	if( company )
	{
		_companies.push_back( company );

		Param( COMPANIESNUMBER ) = _companies.size();
		PUser ceo = (*company)[ CEO ].As<PUser>();
		
		assert( ceo );
		if( ceo && ceo->ObjectTypeName() == CNrpPlayer::ClassName() )
			Param( PLAYERCOMPANY ) = company;

		return 1;
	}
	return 0;
}

int CNrpApplication::AddUser( IUser* user )
{
	IUser* tmpUser = GetUser( user->Param( NAME ) );
	assert( tmpUser == NULL );

	if( user && tmpUser == NULL )
		users_.push_back( user );

	Param( USERNUMBER ) = static_cast< int >( users_.size() );
	return 1;
}


void CNrpApplication::AddProject( INrpProject* project )
{
	if( FindByName<PROJECTS, INrpProject>( _projects, project->Param( NAME ) ) == NULL )
		_projects.push_back( project );
}

CNrpTechnology* CNrpApplication::GetTechnology( int index )
{
	return index < (int)technologies_.size() ?  technologies_[ index ] : NULL;
}

CNrpTechnology* CNrpApplication::GetTechnology( const NrpText& name )
{
	return FindByNameAndIntName<TECHS, CNrpTechnology>( technologies_, name );
}

void CNrpApplication::AddTechnology( CNrpTechnology* ptrTech )
{
	CNrpTechnology* tech = GetTechnology( (NrpText)ptrTech->Param( INTERNAL_NAME) );
	if( !tech )
		technologies_.push_back( ptrTech );

	Param( TECHNUMBER ) = static_cast<int>( technologies_.size() );
}

IUser* CNrpApplication::GetUser( u32 index )
{
	return index < users_.size() ? users_[ index ] : NULL;
}

IUser* CNrpApplication::GetUser( const NrpText& name )
{
	return FindByNameAndIntName<USERS, IUser>( users_, name );
}

int CNrpApplication::RemoveUser( IUser* user )
{
	for( u32 pos=0; pos < users_.size(); pos++ )
		if( users_[ pos ] == user )
		{
			delete users_[ pos ];
			users_.erase( pos );
			Param( USERNUMBER ) = static_cast< int >( users_.size() );
			return 0;
		}

	return 1;
}

void CNrpApplication::_CreateDirectoriesMapForSave()
{
	OpFileSystem::CreateDirectory( Param( SAVEDIR_PROFILE ) );
	OpFileSystem::CreateDirectory( Param( SAVEDIR_INVENTIONS ) );
	OpFileSystem::CreateDirectory( Param( SAVEDIR_COMPANIES ) );
	OpFileSystem::CreateDirectory( Param( SAVEDIR_DEVPR ) );
	OpFileSystem::CreateDirectory( Param( SAVEDIR_GAMES ) );
	OpFileSystem::CreateDirectory( Param( SAVEDIR_PROJECTS ) );
	OpFileSystem::CreateDirectory( Param( SAVEDIR_ENGINES ) );
	OpFileSystem::CreateDirectory( Param( SAVEDIR_PLANT ) );
	OpFileSystem::CreateDirectory( Param( SAVEDIR_USERS ) );
}

void CNrpApplication::Save()
{
	NrpText prevSaveFolder = (NrpText)Param( SAVEDIR ) + (NrpText)Param( PROFILENAME ) + "Old/";
	NrpText profileIni = (NrpText)Param( SAVEDIR_PROFILE ) + "profile.ini";

	OpFileSystem::Remove( prevSaveFolder );
	OpFileSystem::Move( Param( SAVEDIR_PROFILE ), prevSaveFolder );

	_CreateDirectoriesMapForSave();

	assert( !OpFileSystem::IsExist( profileIni ) );
	IniFile sv( profileIni );

	INrpConfig::Save( profileIni );

	sv.Set( SECTION_OPTIONS, "currentProfile", (NrpText)Param( PROFILENAME ) );
	sv.Set( SECTION_OPTIONS, "currentCompany", (NrpText)Param( PROFILECOMPANY ) );
	
	for( u32 i=0; i < _companies.size(); i++ )
	{
		_companies[ i ]->Save( Param( SAVEDIR_COMPANIES ) );
		sv.Set( SECTION_COMPANIES, CreateKeyCompany( i ), (NrpText)_companies[ i ]->Param( NAME ) );
	}

	for( u32 i=0; i < _devProjects.size(); i++ )
	{
		NrpText saveFile = _devProjects[ i ]->Save( Param( SAVEDIR_DEVPR ) );
		sv.Set( SECTION_DEVPROJECTS, CreateKeyType(i), _devProjects[ i ]->ObjectTypeName() );
		sv.Set( SECTION_DEVPROJECTS, CreateKeyProject(i), saveFile );
	}

	for( u32 i=0; i < _projects.size(); i++ )
	{
		NrpText saveFile = _projects[ i ]->Save( Param( SAVEDIR_PROJECTS ) );
		sv.Set( SECTION_DEVPROJECTS, CreateKeyType(i), _projects[ i ]->ObjectTypeName() );
		sv.Set( SECTION_PROJECTS, CreateKeyProject(i), saveFile );
	}

	for( u32 i=0; i < users_.size(); i++ )
	{
		users_[ i ]->Save( Param( SAVEDIR_USERS ) );
		sv.Set( SECTION_USERS, CreateKeyType(i),users_[ i ]->ObjectTypeName() );
		sv.Set( SECTION_USERS, CreateKeyUser(i), (NrpText)users_[ i ]->Param( NAME ) );
	}

	for( u32 i=0; i < technologies_.size(); i++ )
	{
		NrpText saveFile = technologies_[ i ]->Save( Param( SAVEDIR_TECHS ) );
		sv.Set( SECTION_TECHS, CreateKeyTech(i), saveFile );
	}

	for( u32 i=0; i < inventions_.size(); i++ )
	{
		NrpText inventSaveFile = inventions_[ i ]->Save( (NrpText)Param( SAVEDIR_INVENTIONS ) + 
														 (NrpText)inventions_[ i ]->Param( COMPANYNAME ), true );
		sv.Set( SECTION_INVENTIONS, CreateKeyInvention( i ), inventSaveFile );
	}

	for( u32 i=0; i < engines_.size(); i++ )
	{
		NrpText saveFolder = engines_[ i ]->Save( Param( SAVEDIR_ENGINES ) );
		sv.Set( SECTION_ENGINES, CreateKeyEngine(i), saveFolder );
	}

	for( u32 i=0; i < games_.size(); i++ )
	{
		NrpText saveDir = games_[ i ]->Save( Param( SAVEDIR_GAMES ) );
		sv.Set( SECTION_GAMES, CreateKeyGame( i ), saveDir );
	}
}

void CNrpApplication::_LoadUsers( const NrpText& fileName )
{
	IniFile sv( fileName );
	int maxUser = Param( USERNUMBER );
	for( int i=0; i < maxUser; i++ )
	{
		NrpText name = sv.Get( SECTION_USERS, CreateKeyUser(i), NrpText("") );
		NrpText className = sv.Get( SECTION_USERS, CreateKeyType(i), NrpText("") );

		NrpText fileName = (NrpText)Param( SAVEDIR_USERS ) + name + ".user";
		
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

void CNrpApplication::_InitialyzeSaveDirectories( const NrpText& profileName )
{
	NrpText profileDir = OpFileSystem::CheckEndSlash( (NrpText)Param( SAVEDIR ) + profileName );
	Param( SAVEDIR_PROFILE ) = profileDir;
	Param( SAVEDIR_INVENTIONS ) = profileDir + "inventions/";
	Param( SAVEDIR_COMPANIES ) = profileDir + "companies/";
	Param( SAVEDIR_DEVPR ) = profileDir + "devProjects/";
	Param( SAVEDIR_GAMES ) = profileDir + "games/";
	Param( SAVEDIR_ENGINES ) = profileDir + "engines/";
	Param( SAVEDIR_PROJECTS ) = profileDir + "projects/";
	Param( SAVEDIR_PLANT ) = profileDir + "plant/";
	Param( SAVEDIR_USERS ) = profileDir + "users/";
	Param( SAVEINI_PROFILE ) = profileDir + "profile.ini";
	Param( SAVEDIR_TECHS ) = profileDir + "techs/";
}

void CNrpApplication::Load( const NrpText& profileName, const NrpText& companyName )
{
	_InitialyzeSaveDirectories( profileName );

	CNrpScript::Instance().TemporaryScript( AFTER_LOAD_SCRIPT, CNrpScript::SA_CREATE );

	NrpText profileIni = Param( SAVEINI_PROFILE );
	INrpConfig::Load( profileIni );

	_LoadUsers( profileIni );	

	IniFile rv( profileIni );

	for( int i=0; i < (int)Param( TECHNUMBER ); i++ )
	{
		NrpText fileName = rv.Get( SECTION_TECHS, CreateKeyTech(i), NrpText("") );
		CNrpTechnology* tech = new CNrpTechnology( fileName ); //loading
		technologies_.push_back( tech );
	}

	for( int i=0; i < (int)Param( ENGINES_NUMBER ); i++ )
	{
		NrpText saveFolder = rv.Get( SECTION_ENGINES, CreateKeyEngine(i), NrpText("") );
		engines_.push_back( new CNrpGameEngine( saveFolder, true ) );
	}

	for( int i=0; i < (int)Param( DEVELOPPROJECTS_NUMBER ); i++ )
	{
		NrpText type = rv.Get( SECTION_DEVPROJECTS, CreateKeyType( i ), NrpText("") );
		NrpText name = rv.Get( SECTION_DEVPROJECTS, CreateKeyProject( i ), NrpText("") );
		if( type == CNrpDevelopGame::ClassName() )
		{
			CNrpDevelopGame* game = new CNrpDevelopGame( Param( SAVEDIR_DEVPR ).As<NrpText>() + name );
			_devProjects.push_back( game );
		}
		else if( type == NrpText("devengine") )
		{
			//
		}
	}

	for( int i=0; i < (int)Param( PROJECTNUMBER ); i++ )
	{
		NrpText fileName = rv.Get( SECTION_PROJECTS, CreateKeyProject( i ), NrpText("") );
		NrpText type = rv.Get( SECTION_PROJECTS, CreateKeyType( i ), NrpText("") );
		if( type == CNrpGameProject::ClassName() )
		{
			CNrpDevelopGame* game = new CNrpDevelopGame( Param( SAVEDIR_PROJECTS ).As<NrpText>() + fileName );
			_projects.push_back( game );
		}
		else if( type == NrpText("devengine") )
		{
			//
		}
	}

	for( int i=0; i < (int)Param( GAMENUMBER ); i++ )
	{
		NrpText fileName = rv.Get( SECTION_GAMES,  CreateKeyGame( i ), NrpText("") );
		PNrpGame game = new CNrpGame( fileName );
		games_.push_back( game );
	}

	for( int k=0; k < (int)Param( INVENTIONSNUMBER ); k++ )
	{
		NrpText saveFile = rv.Get( SECTION_INVENTIONS, CreateKeyInvention( k ), NrpText("") );
		CNrpInvention* invention = new CNrpInvention( saveFile );
		inventions_.push_back( invention );
	}
	Param( INVENTIONSNUMBER ) = static_cast<int>( inventions_.size() );

	for( int k=0; k < (int)Param( COMPANIESNUMBER ); k++ )
	{
		NrpText fileName = rv.Get( SECTION_COMPANIES, CreateKeyCompany( k ), NrpText("") );
		CNrpCompany* cmp = new CNrpCompany( Param( SAVEDIR_COMPANIES ).As<NrpText>() + fileName );
		AddCompany( cmp );
	}
	Param( GAMENUMBER ) = static_cast<int>( games_.size() );

	CNrpScript::Instance().TemporaryScript( AFTER_LOAD_SCRIPT, CNrpScript::SA_EXEC );
}

void CNrpApplication::LoadScreenshots( const NrpText& fileName )
{
	IniFile rv( fileName );
	size_t imageListNumber = rv.Get( SECTION_OPTIONS, "screenshotNumber", (int)0 );

	if( !OpFileSystem::IsExist( fileName ) )
		return;
	
	for( size_t i=0; i < imageListNumber; i++ )
	{
		NrpText scrFile = rv.Get( SECTION_OPTIONS, CreateKeyScreenshot( i ), NrpText("") );
		_screenshots.push_back( new CNrpScreenshot( scrFile ) );
	}
}

void CNrpApplication::CreateProfile( const NrpText& profileName, const NrpText& companyName )
{
	Param( PROFILENAME ) = profileName;
	Param( PROFILECOMPANY ) = companyName;

	_InitialyzeSaveDirectories( profileName );
}

template< class T > 
void ClearArray( T& arrayt )
{
	for( u32 i=0; i < arrayt.size(); i++ )
		delete arrayt[ i ];
	arrayt.clear();
}

void CNrpApplication::ResetData()
{
	ClearArray( _companies );
	ClearArray( users_ );
	ClearArray( technologies_ );
}

CNrpGameEngine* CNrpApplication::GetGameEngine( const NrpText& name ) 
{
	return FindByNameAndIntName<ENGINES, CNrpGameEngine>( engines_, name );
}

void CNrpApplication::_BeginNewDay()
{
	for( u32 i=0; i < _companies.size(); i++)
		 _companies[ i ]->BeginNewDay( Param( CURRENTTIME ) );

	CNrpPlant::Instance().BeginNewDay();
	UpdateMarketGames_();
	UpdateInvention_();
}

void CNrpApplication::UpdateInvention_()
{
	for( u32 k=0; k < inventions_.size(); k++ )
	{
		CNrpInvention* pInv = inventions_[ k ];
		assert( pInv );
		pInv->CheckParams();

		if( (*pInv)[ READYWORKPERCENT ] >= 1.f )
		{
			(*pInv )[ ENDDATE ] = Param( CURRENTTIME );
			InventionFinished( pInv );
			k--;
		}
	}
}

float GetConsumerAbility_( int price )
{
	if( price > 150 ) return 10.f / price;

	if( price > 100 && price <= 150 ) return 20.f / price;

	if( price > 50 && price <= 100) return 40.f / price;

	if( price > 25 && price <= 50 ) return 50.f / price;

	if( price > 10 && price <= 25) return 25.f / price;
	else return 30.f / price;
}

int CNrpApplication::GetFreePlatformNumberForGame_( CNrpGame* game )
{
	int yearRaznost = Param( CURRENTTIME ).As<SYSTEMTIME>().wYear - 1980;

	int summ = 5000;
	for( int k=0; k < yearRaznost; k++ )
		 summ += summ * yearRaznost;

	return summ * ((int)(*game)[ PLATFORMNUMBER ]);
}

int CNrpApplication::GetSalesNumber_( CNrpGame* game )
{
	CNrpCompany* cmp = game->Param( PARENTCOMPANY ).As<CNrpCompany*>();
	assert( cmp );
	if( !cmp )
		return 0;

	//получим количество платформ на которых может быть продана игра
	int freePlatformNumber = GetFreePlatformNumberForGame_( game );
	
	//найдем количество игр этого жанра
	float gamesInThisGenre = 1;
	for( u32 i=0; i < games_.size(); i++ )
	{
		CNrpGame* tmpGame = games_[ i ];
		if( (game != tmpGame) && 
			game->Param( GAMEISSALING ) &&
			(tmpGame->GetGenreName( 0 ) == game->GetGenreName( 0 )) )
		  gamesInThisGenre += (int)game->Param( CURRENTGAMERATING ) / 100.f; 
	}

	freePlatformNumber -= (int)game->Param( COPYSELL );
	float userModificator = 1, compannyFamous = 1;
	if( cmp )
	{
		userModificator = cmp->GetUserModificatorForGame( game );
		compannyFamous = cmp->Param( FAMOUS ); 
	}

	float authorFamous = 1;
	authorFamous = game->GetAuthorFamous();
	NrpText retailerName = game->Param( GAMERETAILER );
	PNrpRetailer retailer = GetRetailer( retailerName );

	float retailerFamous = 0.1f;
	if( retailer )
		retailerFamous = retailer->Param( FAMOUS );

	float genreInterest = GetGameGenreInterest( game );

	//столько игр может быть продано сегодня
	freePlatformNumber /= 365;
	//вероятность что покупатель обратит внимание на нашу игру после прочтения обзора в прессе
	int gameMaySaledToday = (int)((freePlatformNumber*genreInterest) / gamesInThisGenre);

	//повышение продаж игры за счет рекламы игры, известности авторов и личностных модификаторов
	gameMaySaledToday = static_cast< int >(gameMaySaledToday * ( game->Param( FAMOUS ).As<float>() + userModificator + authorFamous ));

	//коэффициент продаж по известности ретейлера и компании
	gameMaySaledToday = static_cast< int >( gameMaySaledToday * (compannyFamous + retailerFamous) * 0.5f );

	//коэффициент покупательской способности
	if( PNrpGameBox box = game->Param( GBOX ).As<PNrpGameBox>() )
	{
		gameMaySaledToday = static_cast< int >( gameMaySaledToday * GetConsumerAbility_( (*box)[ PRICE ] ) );
	}

	return gameMaySaledToday;
}

void CNrpApplication::UpdateMarketGames_()
{
	for( u32 i=0; i < games_.size(); i++ )
	{
		CNrpGame* rGame = games_[ i ];
		if( !rGame->Param( GAMEISSALING ) )
			continue;

		rGame->GameBoxSaling( GetSalesNumber_( rGame ) );
	}
}

void CNrpApplication::CreateNewFreeUsers()
{
	USERS coders, designer, composer, tester;
	core::map< NrpText, USERS* > group;
	group[ "coder" ] = &coders;
	group[ "designer" ] = &designer;
	group[ "composer" ] = &composer;
	group[ "tester" ] = &tester;
	
	for( u32 i=0; i < users_.size(); i++ )
	{
		IUser* user = users_[ i ];
		NrpText typeName = user->ObjectTypeName();
		if( user->Param( PARENTCOMPANY ).As<PNrpCompany>() != NULL )
			continue;

		if( core::map< NrpText, USERS* >::Node* node = group.find( typeName ) )
			node->getValue()->push_back( user );
	}

	size_t USER_GROUP_COUNT = 6;

	core::map< NrpText, USERS* >::Iterator gIter = group.getIterator();
	for( ; !gIter.atEnd(); gIter++ )
	{
		USERS& tmpList = *(gIter->getValue());
		for( u32 k=0; k < tmpList.size(); k++ )
			RemoveUser( tmpList[ k ] );

		tmpList.clear();

		for( size_t cnt=tmpList.size(); cnt < USER_GROUP_COUNT; cnt++ )
			tmpList.push_back( CreateRandomUser_( gIter->getKey() ) );
	}

	gIter = group.getIterator();
	for( ; !gIter.atEnd(); gIter++ )
		for( u32 cnt=0; cnt < gIter->getValue()->size(); cnt++ )
			users_.push_back( (*gIter->getValue())[ cnt ] );

	if( Param( USERNUMBER ) != static_cast< int >( users_.size() ) )
		DoLuaFunctionsByType<void>( APP_USER_MARKETUPDATE, NULL );

	Param( USERNUMBER ) = static_cast< int >( users_.size() );
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
		return (*ptrObject)[ QUALITY ];
	return 0;
}

void CNrpApplication::_UpdateGameRating( CNrpGame* ptrGame, GAME_RATING_TYPE typeRating )
{
	assert( ptrGame );
	int rating = 0;

	CNrpCompany* cmp = ptrGame->Param( PARENTCOMPANY ).As<PNrpCompany>();
	if( ptrGame && cmp )
	{		
		CNrpGame& refGame = *ptrGame;
		int number = 0;

		rating = GetQuality_( cmp->GetGameEngine( refGame[ GAME_ENGINE ].As<NrpText>() ) );

		for( int k=0; k < (int)refGame[ MODULE_NUMBER ]; k++ )
		{
			NrpText name = ptrGame->GetTechName( k );
			rating += GetQuality_( GetTechnology( name ) );
			rating /= 2;
		}

		//вычисляем сколько месяцев на рынке игра
		int monthInMarket = TimeHelper::GetMonthBetweenDate( refGame[ STARTDATE ], Param( CURRENTTIME ) ) + 1;
		//понижаем рейтинг из-за времени на рынке
		rating = static_cast< int >( rating / (monthInMarket > 12 ? 12 :  monthInMarket) );

		//результат подсчета рейтинга
		//todo: надо както обходить рейтинг хитовых игр
		ptrGame->GetHistory()->AddStep( Param( CURRENTTIME ) )->AddValue<int>( CURRENTGAMERATING, rating );
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

IUser* CNrpApplication::CreateRandomUser_( NrpText userType )
{
	size_t randomParams = 1 + rand() % (GT_COUNT % 100);//сколько параметров будем создавать
	size_t maxParamValue = 1 + rand() % 100;//максимальное значение параметров

	std::map< NrpText, int > skillMap;
	skillMap[ "coder" ] = SKL_CODING;
	skillMap[ "designer" ] = SKL_DRAWING;
	skillMap[ "composer" ] = SKL_SOUND;
	skillMap[ "tester" ] = SKL_TESTING;

	NrpText userName;

	IUser* ptrUser = NULL;
	do 
	{
		userName = GlobalPeopleName[ rand() % PEOPLE_NAME_COUNT ] + " " + GlobalPeopleSurname[ rand() % PEOPLE_SURNAME_COUNT ];
		ptrUser = GetUser( userName );
	} while ( ptrUser != NULL );

	ptrUser = new IUser( userType, userName );
	IUser& refUser = *ptrUser;
	ptrUser->SetSkill( skillMap[ userType ], maxParamValue );
	refUser[ CODE_QUALITY ] = rand() % maxParamValue;
	refUser[ CODE_SPEED ] = rand() % maxParamValue;
	refUser[ TALANT ] = rand() % maxParamValue;
	refUser[ STAMINA ] = rand() % maxParamValue;
	refUser[ STABILITY ] = rand() % maxParamValue;
	refUser[ CHARACTER ] = rand() % maxParamValue;

	if( refUser[ WANTMONEY ] < Param( MINIMUM_USER_SALARY ) )
		refUser[ WANTMONEY ] = Param( MINIMUM_USER_SALARY );

	char name[64] = { 0 };
	snprintf( name, 64, "media/face/face%03d.png", rand() % 2 );
	refUser[ TEXTURENORMAL ] = NrpText( name );

	for( size_t cnt=0; cnt < randomParams; cnt++ )
	{
		int rMax = GT_COUNT%100;
		ptrUser->SetGenreExperience( static_cast< GENRE_TYPE >( rand() % rMax ), rand() % maxParamValue );
		ptrUser->SetGenrePreferences( static_cast< GENRE_TYPE >( rand() % rMax ), rand() % maxParamValue );
	} 

	return ptrUser;
}

void CNrpApplication::_BeginNewMonth()
{
	//начало месяца в компании
	for( u32 i=0; i < _companies.size(); i++)
		_companies[ i ]->BeginNewMonth( Param( CURRENTTIME ));

	//обновляем рейтинги игр
	for( u32 i=0; i < games_.size(); i++ )
		 if( games_[ i ]->Param( GAMEISSALING ) )
			 UpdateGameRatings( games_[ i ] );
}

void CNrpApplication::_BeginNewHour()
{
	for( u32 i=0; i < _companies.size(); i++)
		 _companies[ i ]->BeginNewHour( Param( CURRENTTIME ));
}

bool CNrpApplication::AddBoxAddon( CNrpTechnology* tech )
{
	if( GetBoxAddon( (*tech)[ NAME ].As<NrpText>() ) == NULL  )
	{
		boxAddons_.push_back( tech );
		Param( BOXADDONNUMBER ) = static_cast< int >( boxAddons_.size() );
		return true;
	}

	return false;
}

CNrpTechnology* CNrpApplication::GetBoxAddon( const NrpText& name )
{
	return FindByNameAndIntName< TECHS, CNrpTechnology >( boxAddons_, name );
}

void CNrpApplication::AddGameToMarket( CNrpGame* game )
{
	assert( game != NULL );
	if( !game || game-Param( GAMEISSALING ) )
		return;

	CNrpGame& refGame = *game;
	refGame[ GAMEISSALING ] = true;

	//когда игра выходит на рынок, то она влияет на него
	for( int i=0; i < (int)refGame[ GENRE_MODULE_NUMBER ]; i++ )
	{
		NrpText genreName = game->GetGenreName( i );
		//влияние приводит к изменению интереса к жанру игры
		CNrpTechnology* tech = GetTechnology( genreName );
		assert( tech );
		if( tech != NULL )
			(*tech)[ INTEREST ] = -(int)refGame[ STARTGAMERATING ] / 1000.f;
	}

	games_.push_back( game );
	Param( GAMENUMBER ) = static_cast< int >( games_.size() );
}

//интерес к жанру меняется в противоположную сторону на 10% от рейтинга игры
float CNrpApplication::GetGameGenreInterest( CNrpGame* game )
{
	float summ = 0;
	assert( game );
	
	CNrpTechnology* tech = GetTechnology( game->GetGenreName( 0 ) );
	assert( tech );
	if( game && tech )
	{
		summ = (*tech)[ INTEREST ];
		int gm = (*game)[ GENRE_MODULE_NUMBER ];
		for( int i=1; i < gm; i++ )
		{
			tech = GetTechnology( game->GetGenreName( i ) );	
			summ += (*tech)[ INTEREST ].As<float>() / i;
		}
	}
	//надо скомпенсировать понижение интереса к жанру, которое принесла
	//игра при размещении на рынке
	return (summ + (int)(*game)[CURRENTGAMERATING] / 1000.f );
}

CNrpRetailer* CNrpApplication::GetRetailer( const NrpText& name )
{
	return NULL;
}

//получение имени изображения с которым будет дальше связана игра
NrpText CNrpApplication::GetFreeInternalName( CNrpGame* game )
{
	SCREENSHOTS	thisYearAndGenreImgs;
	
	int minimumRating = 1;
	for( u32 i=0; i < _screenshots.size(); i++ )
	{
		CNrpGame* alsoMargetGame = GetGame( _screenshots[ i ]->Param( NAME ).As<NrpText>() );

		if( !alsoMargetGame ) 
		{
			int year = Param( CURRENTTIME ).As<SYSTEMTIME>().wYear;
			if( CNrpGameEngine* ge = GetGameEngine( game->Param( GAME_ENGINE ).As<NrpText>() ) )
				year = ge->Param( STARTDATE ).As<SYSTEMTIME>().wYear;

			if( !_screenshots[ i ]->IsMyYear( year ) )
				continue;
			
			int eqRating = _screenshots[ i ]->GetEqualeRating( game );

			if( !eqRating )
				continue;

			if( minimumRating > eqRating )
				thisYearAndGenreImgs.clear();

			minimumRating = eqRating;
			thisYearAndGenreImgs.push_back( _screenshots[ i ] );
		}
	}

	if( thisYearAndGenreImgs.size() )
	{
		int randomIndex = rand() % thisYearAndGenreImgs.size();
		return thisYearAndGenreImgs[ randomIndex ]->Param( NAME );
	}

	//!!!!!!!!надо както обработать эту ситуацию!!!!!!!!!!!!!!!!!
	assert( NrpText("No find free name").size() == 0 );
	return "";
}

CNrpScreenshot* CNrpApplication::GetScreenshot( const NrpText& name )
{
	return FindByName< SCREENSHOTS, CNrpScreenshot >( _screenshots, name );
}

CNrpGame* CNrpApplication::GetGame( const NrpText& name )
{
	return FindByNameAndIntName< GAMES, CNrpGame >( games_, name );
}

CNrpGame* CNrpApplication::GetGame( u32 index )
{
	assert( index < games_.size() );
	return index < games_.size() ? games_[ index ] : NULL;
}

void CNrpApplication::RemoveTechnology( CNrpTechnology* ptrTech )
{
	for( u32 i=0; i < technologies_.size(); i++ )
		if( technologies_[ i ] == ptrTech )
		{
			technologies_.erase( i );
			return;
		}

	Log(HW) << "unknown technology" << term;
}

void CNrpApplication::AddGameEngine( nrp::CNrpGameEngine* ptrEngine )
{
	if( GetGameEngine( ptrEngine->Param( INTERNAL_NAME ) ) == NULL )
	{
		engines_.push_back( ptrEngine );
		Param( ENGINES_NUMBER ) = static_cast<int>( engines_.size() );
		return;
	}

	Log( HW ) << "такой движок уже есть" << term;
}

INrpProject* CNrpApplication::GetProject( const NrpText& name )
{
	return FindByName< PROJECTS, INrpProject >( _projects, name );
}

void CNrpApplication::AddDevelopProject( nrp::INrpDevelopProject* project )
{
	if( FindByName< DEVPROJECTS, INrpDevelopProject >( _devProjects, project->Param( NAME ) ) )
	{
		_devProjects.push_back( project );
		Param( DEVELOPPROJECTS_NUMBER ) = static_cast< int >( _devProjects.size() );
	}
}

INrpDevelopProject* CNrpApplication::GetDevelopProject( const NrpText& name )
{
	return FindByName<DEVPROJECTS, INrpDevelopProject>( _devProjects, name );
}

void CNrpApplication::RemoveDevelopProject( const NrpText& name )
{
	for( u32 i=0; i < _devProjects.size(); i++ )
	{
		if( _devProjects[ i ]->Equale( name ) )
		{
			delete _devProjects[ i ];
			_devProjects.erase( i );
			break;
		}
	}

	Param( DEVELOPPROJECTS_NUMBER ) = static_cast< int >( _devProjects.size() );
}

void CNrpApplication::AddGame( CNrpGame* ptrGame )
{
	assert( ptrGame != NULL );
	games_.push_back( ptrGame );
	Param( GAMENUMBER ) = static_cast< int >( games_.size() );
}

void CNrpApplication::AddInvention( const NrpText& name, CNrpCompany* parentCompany )
{
	CNrpInvention* tmp = GetInvention( name, parentCompany->Param( NAME ) );

	if( tmp == NULL )
	{
		CNrpTechnology* startTech = new CNrpTechnology( NrpText( "xtras/technology/" ) + name + "/item.tech" );

		CNrpInvention* inv = new CNrpInvention( startTech, parentCompany );
		parentCompany->AddInvention( inv );	
		inventions_.push_back( inv );  
		Param( INVENTIONSNUMBER ) = static_cast< int >( inventions_.size() );

		delete startTech;
	}
}

void CNrpApplication::InventionFinished( CNrpInvention* ptrInvention )
{
	//создать соответствующую изобретению технологию 
	//разместить её в списке доступных
	CNrpTechnology* tech = new CNrpTechnology( ptrInvention );
	AddTechnology( tech );

	int delPos = -1;
	for( u32 i=0; i < inventions_.size(); i++ )
	{
		CNrpInvention* inv = inventions_[ i ];
		if( inv->Param( NAME ) == ptrInvention->Param( NAME ) )
		{
			CNrpCompany* pCmp = inv->Param( PARENTCOMPANY ).As<PNrpCompany>();
			if( pCmp == ptrInvention->Param( PARENTCOMPANY).As<PNrpCompany>() )
			{
				delPos = i;//найти это изобретение в своем списке и удалить его оттуда...
				pCmp->AddTechnology( tech );
				pCmp->RemoveInvention( ptrInvention );
				continue;
			}
		
			pCmp->InventionReleased( ptrInvention );//уведомить все компании об изобретении технологии
		}
	}

	DoLuaFunctionsByType( APP_INVENTION_FINISHED, tech );
	
	delete inventions_[ delPos ];
	inventions_.erase( delPos );
}

void CNrpApplication::InventionCanceled( CNrpInvention* ptrInvention )
{
	for( u32 i=0; i < inventions_.size(); i++ )
	{
		CNrpInvention* inv = inventions_[ i ];
		if( inv == ptrInvention )
		{
#ifdef _DEBUG
			NrpText text = NrpText( "Удалена технология " ) + inv->Param( NAME ).As<NrpText>();
			PNrpCompany cmp = inv->Param( PARENTCOMPANY ).As<PNrpCompany>();
			assert( cmp != NULL );
			text.append( cmp ? cmp->Param( NAME ).As<NrpText>() : NrpText("unknown company") );
			Log( HW ) << text << term;
#endif
			delete inv;
			inventions_.erase( i );
			break;
		}
	}
}

CNrpInvention* CNrpApplication::GetInvention( const NrpText& name, const NrpText& companyName )
{
	for( u32 i=0; i < inventions_.size(); i++ )
		if( inventions_[ i ]->Equale( name, companyName ) )
			return inventions_[ i ];

	return NULL;
}

NrpText CNrpApplication::ClassName()
{
	return CLASS_NRPAPPLICATION;
}
}//namespace nrp