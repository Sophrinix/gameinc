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
#include "nrpScript.h"
#include "NrpHtmlEngineConfig.h"
#include "HTMLEngine.h"
#include "NrpPluginEngine.h"
#include "NrpApplication.h"
#include "NrpBank.h"
#include "nrpPlant.h"
#include "OpFileSystem.h"
#include "nrpVideoConfig.h"
#include "nrpHUDConfig.h"
#include "NrpConsoleConfig.h"
#include "NrpPlatform.h"

#include <io.h>
#include <errno.h>
#include <OleAuto.h>

static nrp::CNrpApplication* globalApplication = NULL;

namespace nrp
{
CLASS_NAME CLASS_NRPAPPLICATION( "CNrpApplication" );

CNrpApplication::CNrpApplication(void) : INrpConfig( CLASS_NRPAPPLICATION, CLASS_NRPAPPLICATION )
{
	Add<PNrpBank>( BANK, NULL );
	Add( TECHNUMBER, (int)0 );
	Add( USERNUMBER, (int)0 );
	Add( COMPANIESNUMBER, (int)0 );

	Add<NrpText>( WORKDIR, "" );
	Add<NrpText>( SAVEDIR, "save/" );
	Add<NrpText>( SAVEDIR_INVENTIONS, "" );
	Add<NrpText>( SAVEDIR_COMPANIES, "" );
	Add<NrpText>( SAVEDIR_DEVPR, "" );
	Add<NrpText>( SAVEDIR_GAMES, "" );
	Add<NrpText>( SAVEDIR_PLATFORMS, "" );
	Add<NrpText>( SAVEDIR_PROJECTS, "" );
	Add<NrpText>( SAVEDIR_ENGINES, "" );
	Add<NrpText>( SAVEDIR_USERS, "" );
	Add<NrpText>( SAVEDIR_PLANT, "" );
	Add<NrpText>( SAVEINI_PROFILE, "" );
	Add<NrpText>( SAVEDIR_PROFILE, "" );
	Add<NrpText>( SAVEDIR_TECHS, "" );
	Add( SYSTEMINI, NrpText( "config/system.ini" ) );

	IniFile rv( (NrpText)Param( SYSTEMINI ) );
	Add( PROFILENAME, rv.Get( SECTION_OPTIONS, "currentProfile", NrpText( "dalerank" ) ) );
	Add( PROFILECOMPANY, rv.Get( SECTION_OPTIONS, "currentCompany", NrpText( "daleteam" ) ) );

	Add( CURRENTTIME, SYSTEMTIME() );
	Add( BOXADDONNUMBER, (int)0 );
	Add( GAMENUMBER, (int)0 );
	Add( ENGINES_NUMBER, (int)0 );
	Add( DEVELOPPROJECTS_NUMBER, 0 );
	Add( PROJECTNUMBER, (int)0 );
	Add( PLATFORMNUMBER, (int)0 );
	Add<PNrpCompany>( PLAYERCOMPANY, NULL );
	Add( INVENTIONSNUMBER, (int)0 );
	Add( MINIMUM_USER_SALARY, (int)250 );
	Add<CNrpPda*>( PDA, new CNrpPda() );
	Add<CNrpGameTime*>( GAME_TIME, new CNrpGameTime( this ) );
	Add( PAUSEBTWSTEP, 100 );

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

		Param( COMPANIESNUMBER ) = static_cast< int >( _companies.size() );
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
		_users.push_back( user );

	Param( USERNUMBER ) = static_cast< int >( _users.size() );
	return 1;
}


void CNrpApplication::AddProject( INrpProject* project )
{
	if( FindByName<PROJECTS, INrpProject>( _projects, project->Param( NAME ) ) == NULL )
		_projects.push_back( project );
}

CNrpTechnology* CNrpApplication::GetTechnology( int index )
{
	return index < (int)_technologies.size() ?  _technologies[ index ] : NULL;
}

CNrpTechnology* CNrpApplication::GetTechnology( const NrpText& name )
{
	return FindByNameAndIntName<TECHS, CNrpTechnology>( _technologies, name );
}

void CNrpApplication::AddTechnology( CNrpTechnology* ptrTech )
{
	assert( ptrTech );
	CNrpTechnology* tech = GetTechnology( (NrpText)ptrTech->Param( INTERNAL_NAME) );
	if( !tech )
		_technologies.push_back( ptrTech );

	Param( TECHNUMBER ) = static_cast<int>( _technologies.size() );
}

IUser* CNrpApplication::GetUser( u32 index )
{
	return index < _users.size() ? _users[ index ] : NULL;
}

IUser* CNrpApplication::GetUser( const NrpText& name )
{
	return FindByNameAndIntName<USERS, IUser>( _users, name );
}

int CNrpApplication::RemoveUser( IUser* user )
{
	for( u32 pos=0; pos < _users.size(); pos++ )
		if( _users[ pos ] == user )
		{
			delete _users[ pos ];
			_users.erase( pos );
			Param( USERNUMBER ) = static_cast< int >( _users.size() );
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
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_PLATFORMS] );
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
	OpFileSystem::Move( _self[ SAVEDIR_PROFILE ], prevSaveFolder );

	_CreateDirectoriesMapForSave();

	assert( !OpFileSystem::IsExist( profileIni ) );
	IniFile sv( profileIni );

	INrpConfig::Save( profileIni );

	sv.Set( SECTION_OPTIONS, "currentProfile", (NrpText)_self[ PROFILENAME ] );
	sv.Set( SECTION_OPTIONS, "currentCompany", (NrpText)_self[ PROFILECOMPANY ] );
	
	for( u32 i=0; i < _companies.size(); i++ )
	{
		_companies[ i ]->Save( _self[ SAVEDIR_COMPANIES ] );
		sv.Set( SECTION_COMPANIES, CreateKeyCompany( i ), (NrpText)(*_companies[ i ])[ NAME ]);
	}

	for( u32 i=0; i < _devProjects.size(); i++ )
	{
		NrpText saveFile = _devProjects[ i ]->Save( _self[ SAVEDIR_DEVPR ] );
		sv.Set( SECTION_DEVPROJECTS, CreateKeyType(i), _devProjects[ i ]->ObjectTypeName() );
		sv.Set( SECTION_DEVPROJECTS, CreateKeyProject(i), saveFile );
	}

	for( u32 i=0; i < _projects.size(); i++ )
	{
		NrpText saveFile = _projects[ i ]->Save( _self[ SAVEDIR_PROJECTS ] );
		sv.Set( SECTION_DEVPROJECTS, CreateKeyType(i), _projects[ i ]->ObjectTypeName() );
		sv.Set( SECTION_PROJECTS, CreateKeyProject(i), saveFile );
	}

	for( u32 i=0; i < _users.size(); i++ )
	{
		_users[ i ]->Save( _self[ SAVEDIR_USERS ] );
		sv.Set( SECTION_USERS, CreateKeyType(i),_users[ i ]->ObjectTypeName() );
		sv.Set( SECTION_USERS, CreateKeyUser(i), (NrpText)_users[ i ]->Param( NAME ) );
	}

	for( u32 i=0; i < _technologies.size(); i++ )
	{
		NrpText saveFile = _technologies[ i ]->Save( _self[ SAVEDIR_TECHS ] );
		sv.Set( SECTION_TECHS, CreateKeyTech(i), saveFile );
	}

	for( u32 i=0; i < _inventions.size(); i++ )
	{
		NrpText inventSaveFile = _inventions[ i ]->Save( (NrpText)Param( SAVEDIR_INVENTIONS ) + 
														 (NrpText)_inventions[ i ]->Param( COMPANYNAME ), true );
		sv.Set( SECTION_INVENTIONS, CreateKeyInvention( i ), inventSaveFile );
	}

	for( u32 i=0; i < _engines.size(); i++ )
	{
		NrpText saveFolder = _engines[ i ]->Save( _self[ SAVEDIR_ENGINES ] );
		sv.Set( SECTION_ENGINES, CreateKeyEngine(i), saveFolder );
	}

	for( u32 i=0; i < _games.size(); i++ )
	{
		NrpText saveDir = _games[ i ]->Save( _self[ SAVEDIR_GAMES ] );
		sv.Set( SECTION_GAMES, CreateKeyGame( i ), saveDir );
	}

	for( u32 i=0; i < _platforms.size(); i++ )
	{
		NrpText saveDir = _platforms[ i ]->Save( _self[ SAVEDIR_PLATFORMS ] );
		sv.Set( SECTION_PLATFORMS, CreateKeyPlatform( i ), saveDir );
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
	_self[ SAVEDIR_GAMES ] = profileDir + "games/";
	_self[ SAVEDIR_PLATFORMS ] = profileDir + "platforms/";
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

	for( int i=0; i < (int)_self[ TECHNUMBER ]; i++ )
	{
		NrpText fileName = rv.Get( SECTION_TECHS, CreateKeyTech(i), NrpText("") );
		CNrpTechnology* tech = new CNrpTechnology( fileName ); //loading
		_technologies.push_back( tech );
	}

	for( int i=0; i < (int)_self[ PLATFORMNUMBER ]; i++ )
	{
		NrpText fileName = rv.Get( SECTION_PLATFORMS, CreateKeyPlatform(i), NrpText("") );
		CNrpPlatform* plt = new CNrpPlatform( fileName ); //loading
		_platforms.push_back( plt );
	}

	for( int i=0; i < (int)Param( ENGINES_NUMBER ); i++ )
	{
		NrpText saveFolder = rv.Get( SECTION_ENGINES, CreateKeyEngine(i), NrpText("") );
		_engines.push_back( new CNrpGameEngine( saveFolder, true ) );
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
		_games.push_back( game );
	}

	for( int k=0; k < (int)Param( INVENTIONSNUMBER ); k++ )
	{
		NrpText saveFile = rv.Get( SECTION_INVENTIONS, CreateKeyInvention( k ), NrpText("") );
		CNrpInvention* invention = new CNrpInvention( saveFile );
		_inventions.push_back( invention );
	}
	Param( INVENTIONSNUMBER ) = static_cast<int>( _inventions.size() );

	for( int k=0; k < (int)Param( COMPANIESNUMBER ); k++ )
	{
		NrpText fileName = rv.Get( SECTION_COMPANIES, CreateKeyCompany( k ), NrpText("") );
		CNrpCompany* cmp = new CNrpCompany( Param( SAVEDIR_COMPANIES ).As<NrpText>() + fileName );
		AddCompany( cmp );
	}
	Param( GAMENUMBER ) = static_cast<int>( _games.size() );

	CNrpScript::Instance().TemporaryScript( AFTER_LOAD_SCRIPT, CNrpScript::SA_EXEC );
}

void CNrpApplication::LoadScreenshot( const NrpText& fileName )
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
	ClearArray( _users );
	ClearArray( _technologies );
}

CNrpGameEngine* CNrpApplication::GetGameEngine( const NrpText& name ) 
{
	return FindByNameAndIntName<ENGINES, CNrpGameEngine>( _engines, name );
}

void CNrpApplication::_BeginNewDay()
{
	for( u32 i=0; i < _companies.size(); i++)
		 _companies[ i ]->BeginNewDay( Param( CURRENTTIME ) );

	CNrpPlant::Instance().BeginNewDay();
	_UpdateMarketGames();
	_UpdateInvention();
}

void CNrpApplication::_UpdateInvention()
{
	for( u32 k=0; k < _inventions.size(); k++ )
	{
		CNrpInvention* pInv = _inventions[ k ];
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

int CNrpApplication::_GetFreePlatformNumberForGame( CNrpGame* game )
{
	int yearRaznost = Param( CURRENTTIME ).As<SYSTEMTIME>().wYear - 1980;

	int summ = 5000;
	for( int k=0; k < yearRaznost; k++ )
		 summ += summ * yearRaznost;

	return summ * ((int)(*game)[ PLATFORMNUMBER ]);
}

int CNrpApplication::_GetSalesNumber( CNrpGame* game )
{
	CNrpCompany* cmp = game->Param( PARENTCOMPANY ).As<CNrpCompany*>();
	assert( cmp );
	if( !cmp )
		return 0;

	//������� ���������� �������� �� ������� ����� ���� ������� ����
	int freePlatformNumber = _GetFreePlatformNumberForGame( game );
	
	//������ ���������� ��� ����� �����
	float gamesInThisGenre = 1;
	for( u32 i=0; i < _games.size(); i++ )
	{
		CNrpGame* tmpGame = _games[ i ];
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

	//������� ��� ����� ���� ������� �������
	freePlatformNumber /= 365;
	//����������� ��� ���������� ������� �������� �� ���� ���� ����� ��������� ������ � ������
	int gameMaySaledToday = (int)((freePlatformNumber*genreInterest) / gamesInThisGenre);

	//��������� ������ ���� �� ���� ������� ����, ����������� ������� � ���������� �������������
	gameMaySaledToday = static_cast< int >(gameMaySaledToday * ( game->Param( FAMOUS ).As<float>() + userModificator + authorFamous ));

	//����������� ������ �� ����������� ��������� � ��������
	gameMaySaledToday = static_cast< int >( gameMaySaledToday * (compannyFamous + retailerFamous) * 0.5f );

	//����������� �������������� �����������
	if( PNrpGameBox box = game->Param( GBOX ).As<PNrpGameBox>() )
	{
		gameMaySaledToday = static_cast< int >( gameMaySaledToday * GetConsumerAbility_( (*box)[ PRICE ] ) );
	}

	return gameMaySaledToday;
}

void CNrpApplication::_UpdateMarketGames()
{
	for( u32 i=0; i < _games.size(); i++ )
	{
		CNrpGame* rGame = _games[ i ];
		if( !rGame->Param( GAMEISSALING ) )
			continue;

		rGame->GameBoxSaling( _GetSalesNumber( rGame ) );
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
	
	for( u32 i=0; i < _users.size(); i++ )
	{
		IUser* user = _users[ i ];
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
			tmpList.push_back( _CreateRandomUser( gIter->getKey() ) );
	}

	gIter = group.getIterator();
	for( ; !gIter.atEnd(); gIter++ )
		for( u32 cnt=0; cnt < gIter->getValue()->size(); cnt++ )
			_users.push_back( (*gIter->getValue())[ cnt ] );

	if( Param( USERNUMBER ) != static_cast< int >( _users.size() ) )
		DoLuaFunctionsByType<void>( APP_USER_MARKETUPDATE, NULL );

	Param( USERNUMBER ) = static_cast< int >( _users.size() );
}

void CNrpApplication::Init()
{
#ifdef _DEBUG
	Param( WORKDIR ) = OpFileSystem::UpDir( OpFileSystem::UpDir( __argv[ 0 ] ) );
#else
	Param( WORKDIR ) = OpFileSystem::UpDir( __argv[ 0 ] );
#endif
	Param( BANK ) = &CNrpBank::Instance();

	plugin::CNrpPluginEngine::Instance();
	//������������� ������
	CNrpHUDConfig::Instance();
	CNrpVideoConfig::Instance();
	CNrpConsoleConfig::Instance();
	CNrpPlant::Instance();
	CNrpScript::Instance();
	CNrpHtmlEngineConfig& heConfig = CNrpHtmlEngineConfig::Instance();
	heConfig[ BASEDIR ] = OpFileSystem::CheckEndSlash( Param( WORKDIR ) ) + (NrpText)heConfig[ BASEDIR ];

	//������� ��������� �����
	_nrpEngine.InitVideo();
	_nrpEngine.InitConsole();

	HTMLEngine::Instance();
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

		//��������� ������� ������� �� ����� ����
		int monthInMarket = TimeHelper::GetMonthBetweenDate( refGame[ STARTDATE ], Param( CURRENTTIME ) ) + 1;
		//�������� ������� ��-�� ������� �� �����
		rating = static_cast< int >( rating / (monthInMarket > 12 ? 12 :  monthInMarket) );

		//��������� �������� ��������
		//todo: ���� ����� �������� ������� ������� ���
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

int CNrpApplication::_GetTechsByGroup( int type, TECHS& arrayt )
{
	for( u32 i=0; i < _technologies.size(); i++ )
		if( (*_technologies[ i ])[ TECHGROUP ] == type )
			arrayt.push_back( _technologies[ i ] );

	return arrayt.size();
}

IUser* CNrpApplication::_CreateRandomUser( NrpText userType )
{
	TECHS genres;
	_GetTechsByGroup( PT_GENRE, genres );
	int randomParams = 1 + rand() % genres.size();//������� ���������� ����� ���������
	int maxParamValue = 1 + rand() % 100;//������������ �������� ����������

	std::map< NrpText, NrpText > skillMap;
	skillMap[ "coder" ] = "skill_coding";
	skillMap[ "designer" ] = "skill_drawing";
	skillMap[ "composer" ] = "skill_sound";
	skillMap[ "tester" ] = "skill_testing";

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

	if( refUser[ WANTMONEY ] < _self[ MINIMUM_USER_SALARY ] )
		refUser[ WANTMONEY ] = _self[ MINIMUM_USER_SALARY ];

	char name[64] = { 0 };
	snprintf( name, 64, "media/face/face%03d.png", rand() % 2 );
	refUser[ TEXTURENORMAL ] = NrpText( name );

	for( size_t cnt=0; cnt < randomParams; cnt++ )
	{
		NrpText genreName = (*genres[ rand() % genres.size() ])[ INTERNAL_NAME ];
		ptrUser->SetGenreExperience( genreName, rand() % maxParamValue );
		ptrUser->SetGenrePreferences( genreName, rand() % maxParamValue );
	} 

	return ptrUser;
}

void CNrpApplication::_BeginNewMonth()
{
	//������ ������ � ��������
	for( u32 i=0; i < _companies.size(); i++)
		_companies[ i ]->BeginNewMonth( Param( CURRENTTIME ));

	//��������� �������� ���
	for( u32 i=0; i < _games.size(); i++ )
		 if( _games[ i ]->Param( GAMEISSALING ) )
			 UpdateGameRatings( _games[ i ] );
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
		_boxAddons.push_back( tech );
		Param( BOXADDONNUMBER ) = static_cast< int >( _boxAddons.size() );
		return true;
	}

	return false;
}

CNrpTechnology* CNrpApplication::GetBoxAddon( const NrpText& name )
{
	return FindByNameAndIntName< TECHS, CNrpTechnology >( _boxAddons, name );
}

void CNrpApplication::AddGameToMarket( CNrpGame* game )
{
	assert( game != NULL );
	if( !game || game->Param( GAMEISSALING ) )
		return;

	CNrpGame& refGame = *game;
	refGame[ GAMEISSALING ] = true;

	//����� ���� ������� �� �����, �� ��� ������ �� ����
	for( int i=0; i < (int)refGame[ GENRE_MODULE_NUMBER ]; i++ )
	{
		NrpText genreName = game->GetGenreName( i );
		//������� �������� � ��������� �������� � ����� ����
		CNrpTechnology* tech = GetTechnology( genreName );
		assert( tech );
		if( tech != NULL )
			(*tech)[ INTEREST ] = -(int)refGame[ STARTGAMERATING ] / 1000.f;
	}

	_games.push_back( game );
	Param( GAMENUMBER ) = static_cast< int >( _games.size() );
}

//������� � ����� �������� � ��������������� ������� �� 10% �� �������� ����
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
	//���� ��������������� ��������� �������� � �����, ������� ��������
	//���� ��� ���������� �� �����
	return (summ + (int)(*game)[CURRENTGAMERATING] / 1000.f );
}

CNrpRetailer* CNrpApplication::GetRetailer( const NrpText& name )
{
	return NULL;
}

//��������� ����� ����������� � ������� ����� ������ ������� ����
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

	//!!!!!!!!���� ����� ���������� ��� ��������!!!!!!!!!!!!!!!!!
	assert( NrpText("No find free name").size() == 0 );
	return "";
}

CNrpScreenshot* CNrpApplication::GetScreenshot( const NrpText& name )
{
	return FindByName< SCREENSHOTS, CNrpScreenshot >( _screenshots, name );
}

CNrpGame* CNrpApplication::GetGame( const NrpText& name )
{
	return FindByNameAndIntName< GAMES, CNrpGame >( _games, name );
}

CNrpGame* CNrpApplication::GetGame( u32 index )
{
	assert( index < _games.size() );
	return index < _games.size() ? _games[ index ] : NULL;
}

void CNrpApplication::RemoveTechnology( CNrpTechnology* ptrTech )
{
	for( u32 i=0; i < _technologies.size(); i++ )
		if( _technologies[ i ] == ptrTech )
		{
			_technologies.erase( i );
			return;
		}

	Log(HW) << "unknown technology" << term;
}

void CNrpApplication::AddGameEngine( nrp::CNrpGameEngine* ptrEngine )
{
	if( GetGameEngine( ptrEngine->Param( INTERNAL_NAME ) ) == NULL )
	{
		_engines.push_back( ptrEngine );
		Param( ENGINES_NUMBER ) = static_cast<int>( _engines.size() );
		return;
	}

	Log( HW ) << "����� ������ ��� ����" << term;
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
	_games.push_back( ptrGame );
	Param( GAMENUMBER ) = static_cast< int >( _games.size() );
}

void CNrpApplication::AddInvention( const NrpText& name, CNrpCompany* parentCompany )
{
	CNrpInvention* tmp = GetInvention( name, parentCompany->Param( NAME ) );

	if( tmp == NULL )
	{
		CNrpTechnology* startTech = new CNrpTechnology( NrpText( "xtras/technology/" ) + name + "/item.tech" );

		CNrpInvention* inv = new CNrpInvention( startTech, parentCompany );
		parentCompany->AddInvention( inv );	
		_inventions.push_back( inv );  
		Param( INVENTIONSNUMBER ) = static_cast< int >( _inventions.size() );

		delete startTech;
	}
}

void CNrpApplication::InventionFinished( CNrpInvention* ptrInvention )
{
	//������� ��������������� ����������� ���������� 
	//���������� � � ������ ���������
	CNrpTechnology* tech = new CNrpTechnology( ptrInvention );
	AddTechnology( tech );

	int delPos = -1;
	for( u32 i=0; i < _inventions.size(); i++ )
	{
		CNrpInvention* inv = _inventions[ i ];
		if( inv->Param( NAME ) == ptrInvention->Param( NAME ) )
		{
			CNrpCompany* pCmp = inv->Param( PARENTCOMPANY ).As<PNrpCompany>();
			if( pCmp == ptrInvention->Param( PARENTCOMPANY).As<PNrpCompany>() )
			{
				delPos = i;//����� ��� ����������� � ����� ������ � ������� ��� ������...
				pCmp->AddTechnology( tech );
				pCmp->RemoveInvention( ptrInvention );
				continue;
			}
		
			pCmp->InventionReleased( ptrInvention );//��������� ��� �������� �� ����������� ����������
		}
	}

	DoLuaFunctionsByType( APP_INVENTION_FINISHED, tech );
	
	delete _inventions[ delPos ];
	_inventions.erase( delPos );
}

void CNrpApplication::InventionCanceled( CNrpInvention* ptrInvention )
{
	for( u32 i=0; i < _inventions.size(); i++ )
	{
		CNrpInvention* inv = _inventions[ i ];
		if( inv == ptrInvention )
		{
#ifdef _DEBUG
			NrpText text = NrpText( "������� ���������� " ) + inv->Param( NAME ).As<NrpText>();
			PNrpCompany cmp = inv->Param( PARENTCOMPANY ).As<PNrpCompany>();
			assert( cmp != NULL );
			text.append( cmp ? cmp->Param( NAME ).As<NrpText>() : NrpText("unknown company") );
			Log( HW ) << text << term;
#endif
			delete inv;
			_inventions.erase( i );
			break;
		}
	}
}

CNrpInvention* CNrpApplication::GetInvention( const NrpText& name, const NrpText& companyName )
{
	for( u32 i=0; i < _inventions.size(); i++ )
		if( _inventions[ i ]->Equale( name, companyName ) )
			return _inventions[ i ];

	return NULL;
}

NrpText CNrpApplication::ClassName()
{
	return CLASS_NRPAPPLICATION;
}

CNrpPlatform* CNrpApplication::GetPlatform( const NrpText& name )
{
	return FindByNameAndIntName< PLATFORMS, CNrpPlatform >( _platforms, name );
}

CNrpPlatform* CNrpApplication::GetPlatform( int index )
{
	assert( index < _platforms.size() );
	return index < _platforms.size() ? _platforms[ index ] : NULL;
}

bool CNrpApplication::AddPlatform( CNrpPlatform* platform )
{
	if( GetPlatform( (NrpText)(*platform)[ INTERNAL_NAME ] ) == NULL )
	{
		_platforms.push_back( platform );
		_self[ PLATFORMNUMBER ] = static_cast< int >( _platforms.size() );
		return true;
	}

	return false;
}

}//namespace nrp