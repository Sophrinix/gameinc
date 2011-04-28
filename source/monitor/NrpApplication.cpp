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
#include "NrpBridge.h"
#include "NrpTester.h"

#include <io.h>
#include <errno.h>
#include <OleAuto.h>

static nrp::CNrpApplication* globalApplication = NULL;

namespace nrp
{
CLASS_NAME CLASS_NRPAPPLICATION( "CNrpApplication" );

CNrpApplication::CNrpApplication(void) : INrpConfig( CLASS_NRPAPPLICATION, CLASS_NRPAPPLICATION )
{
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
	Add<NrpText>( SAVEDIR_BRIDGE, "" );
	Add( SYSTEMINI, NrpText( "config/system.ini" ) );

	IniFile rv( (NrpText)Param( SYSTEMINI ) );
	Add( PROFILENAME, rv.Get( SECTION_OPTIONS, "currentProfile", NrpText( "dalerank" ) ) );
	Add( PROFILECOMPANY, rv.Get( SECTION_OPTIONS, "currentCompany", NrpText( "daleteam" ) ) );

	Add( CURRENTTIME, NrpTime( 0. ) );
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
	Add( INFLATION, 0.02f );
	Add( PROFIT_TAX, 0.18f );

	srand( GetTickCount() );
}

CNrpApplication::~CNrpApplication(void)
{
}

CNrpCompany* CNrpApplication::GetCompany( const NrpText& companyName )
{
	return FindByNameAndIntName< COMPANIES, CNrpCompany >( _companies, companyName );
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

		_self[ COMPANIESNUMBER ] = static_cast< int >( _companies.size() );
		PUser ceo = (*company)[ CEO ].As<PUser>();
		
		assert( ceo );
		if( ceo && ceo->ObjectTypeName() == CNrpPlayer::ClassName() )
			_self[ PLAYERCOMPANY ] = company;

		return 1;
	}
	return 0;
}

int CNrpApplication::AddUser( CNrpUser* user )
{
	CNrpUser* tmpUser = GetUser( (*user)[ NAME ] );
	assert( tmpUser == NULL );

	if( user && tmpUser == NULL )
		_users.push_back( user );

	_self[ USERNUMBER ] = static_cast< int >( _users.size() );
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
	if( GetTechnology( (NrpText)(*ptrTech)[ INTERNAL_NAME ] ) == NULL )
		_technologies.push_back( ptrTech );

	_self[ TECHNUMBER ] = static_cast<int>( _technologies.size() );
}

CNrpUser* CNrpApplication::GetUser( u32 index )
{
	return index < _users.size() ? _users[ index ] : NULL;
}

CNrpUser* CNrpApplication::GetUser( const NrpText& name )
{
	return FindByNameAndIntName<USERS, CNrpUser>( _users, name );
}

int CNrpApplication::RemoveUser( CNrpUser* user )
{
	for( u32 pos=0; pos < _users.size(); pos++ )
		if( _users[ pos ] == user )
		{
			delete _users[ pos ];
			_users.erase( pos );
			_self[ USERNUMBER ] = static_cast< int >( _users.size() );
			return 0;
		}

	return 1;
}

void CNrpApplication::_CreateDirectoriesMapForSave()
{
	OpFileSystem::CreateDirectory( _self[ SAVEDIR ] );
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_PROFILE ] );
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_INVENTIONS ] );
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_COMPANIES ] );
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_DEVPR ] );
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_GAMES ] );
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_PLATFORMS ] );
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_PROJECTS ] );
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_ENGINES ] );
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_PLANT ] );
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_USERS ] );
}

void CNrpApplication::Save()
{
	NrpText prevSaveFolder = (NrpText)_self[ SAVEDIR ] + (NrpText)_self[ PROFILENAME ] + "Old/";
	NrpText profileIni = (NrpText)_self[ SAVEDIR_PROFILE ] + "profile.ini";

	OpFileSystem::Remove( prevSaveFolder );
	OpFileSystem::Copy( _self[ SAVEDIR_PROFILE ], prevSaveFolder );
	OpFileSystem::Remove( _self[ SAVEDIR_PROFILE ] );

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
		sv.Set( SECTION_USERS, CreateKeyType(i), _users[ i ]->ObjectTypeName() );
		sv.Set( SECTION_USERS, CreateKeyUser(i), (NrpText)_users[ i ]->Param( NAME ) );
	}

	for( u32 i=0; i < _technologies.size(); i++ )
	{
		NrpText saveFile = _technologies[ i ]->Save( _self[ SAVEDIR_TECHS ] );
		sv.Set( SECTION_TECHS, CreateKeyTech(i), saveFile );
	}

	for( u32 i=0; i < _inventions.size(); i++ )
	{
		NrpText inventSaveFile = _inventions[ i ]->Save( (NrpText)_self[ SAVEDIR_INVENTIONS ] + 
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
	int maxUser = _self[ USERNUMBER ];
	for( int i=0; i < maxUser; i++ )
	{
		NrpText name = sv.Get( SECTION_USERS, CreateKeyUser(i), NrpText("") );
		NrpText className = sv.Get( SECTION_USERS, CreateKeyType(i), NrpText("") );

		NrpText fileName = (NrpText)_self[ SAVEDIR_USERS ] + name + ".user";

 		if( !OpFileSystem::IsExist( fileName ) )
		{
			assert( false && "user config file not found" );
			continue;
		}
	
		CNrpUser* user = NULL;
		if( className == CNrpPlayer::ClassName() ) 
			user = new CNrpPlayer( fileName );
		else
		{
			if( className == CNrpAiUser::ClassName() )
				user = new CNrpAiUser( fileName );
			else
			{
				if( className == NrpTester::ClassName() )
					user = new NrpTester( name );
				else
					user = new CNrpUser( className, name );


				user->Load( fileName );
			}
		}

		assert( user );
		AddUser( user );
	}
}

void CNrpApplication::_InitialyzeSaveDirectories( const NrpText& profileName )
{
	NrpText profileDir = OpFileSystem::CheckEndSlash( (NrpText)Param( SAVEDIR ) + profileName );
	_self[ SAVEDIR_PROFILE ] = profileDir;
	_self[ SAVEDIR_INVENTIONS ] = profileDir + "inventions/";
	_self[ SAVEDIR_COMPANIES ] = profileDir + "companies/";
	_self[ SAVEDIR_DEVPR ] = profileDir + "devProjects/";
	_self[ SAVEDIR_GAMES ] = profileDir + "games/";
	_self[ SAVEDIR_PLATFORMS ] = profileDir + "platforms/";
	_self[ SAVEDIR_ENGINES ] = profileDir + "engines/";
	_self[ SAVEDIR_PROJECTS ] = profileDir + "projects/";
	_self[ SAVEDIR_PLANT ] = profileDir + "plant/";
	_self[ SAVEDIR_USERS ] = profileDir + "users/";
	_self[ SAVEINI_PROFILE ] = profileDir + "profile.ini";
	_self[ SAVEDIR_TECHS ] = profileDir + "techs/";
	_self[ SAVEDIR_BRIDGE ] = profileDir + "bridge/";
}

void CNrpApplication::LoadLinks( const NrpText& fileName, const NrpText& templateName )
{
	std::auto_ptr<IniFile> ini( new IniFile( fileName ) );

	int number= ini->Get( "options", templateName + L"Number", (int)0 );
	for( int i=0; i < number; i++ )
	{
		NrpText intName = ini->Get( L"options", NrpText( "name" ) + NrpText( (int)i ), NrpText("") );
		NrpText pathto = ini->Get( L"options", templateName + NrpText( (int)i ), NrpText("") );
		SetLink( intName, pathto );
	}
}

void CNrpApplication::Load( const NrpText& profileName, const NrpText& companyName )
{
	_InitialyzeSaveDirectories( profileName );

	CNrpScript::Instance().TemporaryScript( AFTER_LOAD_SCRIPT, CNrpScript::SA_CREATE );

	NrpText profileIni = _self[ SAVEINI_PROFILE ];
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

	for( int i=0; i < (int)_self[ ENGINES_NUMBER ]; i++ )
	{
		NrpText saveFolder = rv.Get( SECTION_ENGINES, CreateKeyEngine(i), NrpText("") );
		_engines.push_back( new CNrpGameEngine( saveFolder, true ) );
	}

	for( int i=0; i < (int)_self[ DEVELOPPROJECTS_NUMBER ]; i++ )
	{
		NrpText type = rv.Get( SECTION_DEVPROJECTS, CreateKeyType( i ), NrpText("") );
		NrpText saveFile = rv.Get( SECTION_DEVPROJECTS, CreateKeyProject( i ), NrpText("") );
		if( type == CNrpDevelopGame::ClassName() )
		{
			CNrpDevelopGame* game = new CNrpDevelopGame( saveFile );
			_devProjects.push_back( game );
		}
		else if( type == NrpText("devengine") )
		{
			//
		}
	}

	for( int i=0; i < (int)_self[ PROJECTNUMBER ]; i++ )
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

	for( int i=0; i < (int)_self[ GAMENUMBER ]; i++ )
	{
		NrpText fileName = rv.Get( SECTION_GAMES,  CreateKeyGame( i ), NrpText("") );
		PNrpGame game = new CNrpGame( fileName );
		if( (bool)(*game)[ LOADOK ] )
			_games.push_back( game );
	}

	for( int k=0; k < (int)_self[ INVENTIONSNUMBER ]; k++ )
	{
		NrpText saveFile = rv.Get( SECTION_INVENTIONS, CreateKeyInvention( k ), NrpText("") );
		CNrpInvention* invention = new CNrpInvention( saveFile );
		_inventions.push_back( invention );
	}

	int maxComp = (int)_self[ COMPANIESNUMBER ];
	for( int k=0; k < maxComp; k++ )
	{
		NrpText fileName = rv.Get( SECTION_COMPANIES, CreateKeyCompany( k ), NrpText("") );
		if( !GetCompany( fileName ) )
		{
			CNrpCompany* cmp = new CNrpCompany( (NrpText)_self[ SAVEDIR_COMPANIES ] + fileName );
			AddCompany( cmp );
		}
	}

	CNrpBridge::Instance().Load( _self[ SAVEDIR_BRIDGE ] );

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

		CNrpScreenshot* pScr = new CNrpScreenshot( scrFile );
		
		if( FindByNameAndIntName< SCREENSHOTS, CNrpScreenshot>( _screenshots, (*pScr)[ INTERNAL_NAME ], NULL ) == NULL )
			_screenshots.push_back( pScr );
		else
		{
			Log( HW ) << "duplicate name screenshot " << (NrpText)(*pScr)[ INTERNAL_NAME ] << " from " << scrFile << term;
			assert( false );
		}
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
		 _companies[ i ]->BeginNewDay( _self[ CURRENTTIME ] );

	CNrpPlant::Instance().BeginNewDay();
	_UpdateMarketGames();
	_UpdateInvention();
}

void CNrpApplication::_UpdateInvention()
{
	for( u32 k=0; k < _inventions.size(); k++ )
	{
		CNrpInvention& pInv = *(_inventions[ k ]);

		pInv.CheckParams( _self[ CURRENTTIME ].As<NrpTime>() );

		if( pInv[ READYWORKPERCENT ] >= 1.f )
		{
			pInv[ ENDDATE ] = _self[ CURRENTTIME ];
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
	int yearRaznost = _self[ CURRENTTIME ].As<NrpTime>().RYear() - 1980;

	int summ = 5000;
	for( int k=0; k < yearRaznost; k++ )
		 summ += summ * yearRaznost;

	return summ * ((int)(*game)[ PLATFORMNUMBER ]);
}

int CNrpApplication::_GetSalesNumber( CNrpGame* game )
{
	assert( game );
	CNrpCompany* cmp = (*game)[ PARENTCOMPANY ].As<CNrpCompany*>();
	
	assert( cmp );
	if( !cmp )
		return 0;

	//получим количество платформ на которых может быть продана игра
	int freePlatformNumber = _GetFreePlatformNumberForGame( game );
	
	//найдем количество игр этого жанра
	float gamesInThisGenre = 1;
	for( u32 i=0; i < _games.size(); i++ )
	{
		CNrpGame* tmpGame = _games[ i ];
		if( (game != tmpGame) && 
			game->Param( GAMEISSALING ) &&
			(tmpGame->GetGenreName( 0 ) == game->GetGenreName( 0 )) )
		  gamesInThisGenre += (int)(*game)[ CURRENTGAMERATING ] / 100.f; 
	}

	freePlatformNumber -= (int)(*game)[ COPYSELL ];
	float userModificator = 1, compannyFamous = 1;
	if( cmp )
	{
		userModificator = cmp->GetUserModificatorForGame( game );
		compannyFamous = (*cmp)[ FAMOUS ]; 
	}

	float authorFamous = 1;
	authorFamous = game->GetAuthorFamous();
	NrpText retailerName = (*game)[ GAMERETAILER ];
	PNrpRetailer retailer = GetRetailer( retailerName );

	float retailerFamous = 0.1f;
	if( retailer )
		retailerFamous = (*retailer)[ FAMOUS ];

	float genreInterest = GetGameGenreInterest( game );

	//столько игр может быть продано сегодня
	freePlatformNumber /= 365;
	//вероятность что покупатель обратит внимание на нашу игру после прочтения обзора в прессе
	int gameMaySaledToday = (int)((freePlatformNumber*genreInterest) / gamesInThisGenre);

	//повышение продаж игры за счет рекламы игры, известности авторов и личностных модификаторов
	gameMaySaledToday = static_cast< int >(gameMaySaledToday * ( (float)(*game)[ FAMOUS ] + userModificator + authorFamous ));

	//коэффициент продаж по известности ретейлера и компании
	gameMaySaledToday = static_cast< int >( gameMaySaledToday * (compannyFamous + retailerFamous) * 0.5f );

	//коэффициент покупательской способности
	if( PNrpGameBox box = (*game)[ GBOX ].As<PNrpGameBox>() )
	{
		gameMaySaledToday = static_cast< int >( gameMaySaledToday * GetConsumerAbility_( (*box)[ PRICE ] ) );
	}

	return gameMaySaledToday;
}

void CNrpApplication::_UpdateMarketGames()
{
	for( u32 i=0; i < _games.size(); i++ )
	{
		CNrpGame& rGame = *_games[ i ];
		if( !(bool)rGame[ GAMEISSALING ] || (bool)rGame[ NPC_GAME ] )
			continue;

		rGame.GameBoxSaling( _GetSalesNumber( &rGame ) );
	}
}

void CNrpApplication::CreateNewFreeUsers()
{
	USERS coders, designer, composer, tester;
	core::map< NrpText, USERS* > group;
	group[ "coder" ] = &coders;
	group[ "designer" ] = &designer;
	group[ "composer" ] = &composer;
	group[ NrpTester::ClassName() ] = &tester;
	
	for( u32 i=0; i < _users.size(); i++ )
	{
		CNrpUser* user = _users[ i ];
		NrpText typeName = user->ObjectTypeName();
		if( (*user)[ PARENTCOMPANY ].As<PNrpCompany>() != NULL )
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
			tmpList.push_back( CreateRandomUser( gIter->getKey() ) );
	}

	gIter = group.getIterator();
	for( ; !gIter.atEnd(); gIter++ )
		for( u32 cnt=0; cnt < gIter->getValue()->size(); cnt++ )
			_users.push_back( (*gIter->getValue())[ cnt ] );

	if( _self[ USERNUMBER ] != static_cast< int >( _users.size() ) )
		DoLuaFunctionsByType( APP_USER_MARKETUPDATE, NULL );

	_self[ USERNUMBER ] = static_cast< int >( _users.size() );
}

void CNrpApplication::Init()
{
#ifdef _DEBUG
	_self[ WORKDIR ] = OpFileSystem::UpDir( OpFileSystem::UpDir( __argv[ 0 ] ) );
#else
	_self[ WORKDIR ] = OpFileSystem::UpDir( __argv[ 0 ] );
#endif

	CNrpBank::Instance();
	CNrpBridge::Instance();
	plugin::CNrpPluginEngine::Instance();
	//инициализация систем
	CNrpHUDConfig::Instance();
	CNrpVideoConfig::Instance();
	CNrpConsoleConfig::Instance();
	CNrpPlant::Instance();
	CNrpScript::Instance();
	CNrpHtmlEngineConfig& heConfig = CNrpHtmlEngineConfig::Instance();
	heConfig[ BASEDIR ] = OpFileSystem::CheckEndSlash( _self[ WORKDIR ] ) + (NrpText)heConfig[ BASEDIR ];

	//ожидаем подгрузки видео
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

		//вычисляем сколько месяцев на рынке игра
		int monthInMarket = refGame[ STARTDATE ].As<NrpTime>().GetMonthToDate( _self[ CURRENTTIME ].As<NrpTime>() ) + 1;
		//понижаем рейтинг из-за времени на рынке
		rating = static_cast< int >( rating / (monthInMarket > 12 ? 12 :  monthInMarket) );

		//результат подсчета рейтинга
		//todo: надо както обходить рейтинг хитовых игр
		if( CNrpHistoryStep* step = ptrGame->GetHistory()->AddStep( _self[ CURRENTTIME ].As<NrpTime>() ) )
			(*step)[ CURRENTGAMERATING ] += (int)rating;
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

CNrpUser* CNrpApplication::CreateRandomUser( NrpText userType )
{
	TECHS genres;
	_GetTechsByGroup( PT_GENRE, genres );
	size_t randomParams = 1 + rand() % genres.size();//сколько параметров будем создавать
	int maxParamValue = 1 + rand() % 100;//максимальное значение параметров

	std::map< NrpText, NrpText > skillMap;
	skillMap[ "coder" ] = SKILL_CODING;
	skillMap[ "designer" ] = SKILL_DRAWING;
	skillMap[ "composer" ] = SKILL_SOUND;
	skillMap[ NrpTester::ClassName() ] = SKILL_TESTING;

	NrpText userName;

	CNrpUser* ptrUser = NULL;
	do 
	{
		userName = GlobalPeopleName[ rand() % PEOPLE_NAME_COUNT ] + " " + GlobalPeopleSurname[ rand() % PEOPLE_SURNAME_COUNT ];
		ptrUser = GetUser( userName );
	} while ( ptrUser != NULL );

	if( userType == CNrpAiUser::ClassName() )
		ptrUser = new CNrpAiUser( userName, NULL );
	else if( userType == NrpTester::ClassName() )
		ptrUser = new NrpTester( userName );
	else
		ptrUser = new CNrpUser( userType, userName );

	CNrpUser& refUser = *ptrUser;
	ptrUser->SetSkill( skillMap[ userType ], maxParamValue );
	refUser[ WORK_QUALITY ] = rand() % maxParamValue;
	refUser[ WORK_SPEED ] = rand() % maxParamValue;
	refUser[ TALANT ] = rand() % maxParamValue;
	refUser[ STAMINA ] = rand() % maxParamValue;
	refUser[ STABILITY ] = rand() % maxParamValue;
	refUser[ CHARACTER ] = rand() % maxParamValue;

	if( (int)refUser[ WANTMONEY ] < (int)_self[ MINIMUM_USER_SALARY ] )
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
	for( u32 i=0; i < _inventions.size(); i++ )
		_inventions[ i ]->BeginNewMonth( _self[ CURRENTTIME ] );

	//начало месяца в компании
	for( u32 i=0; i < _companies.size(); i++)
		_companies[ i ]->BeginNewMonth( _self[ CURRENTTIME ] );

	//обновляем рейтинги игр
	for( u32 i=0; i < _games.size(); i++ )
		 if( _games[ i ]->Param( GAMEISSALING ) )
			 UpdateGameRatings( _games[ i ] );

	CNrpBridge::Instance().Update();
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

	if( FindByNameAndIntName< GAMES, CNrpGame >( _games, refGame[ INTERNAL_NAME ] ) == NULL )
	{
		//когда игра выходит на рынок, то она влияет на него
		for( int i=0; i < (int)refGame[ GENRE_MODULE_NUMBER ]; i++ )
		{
			NrpText genreName = game->GetGenreName( i );
			//влияние приводит к изменению интереса к жанру игры
			CNrpTechnology* tech = GetTechnology( genreName );
			//такой жанр уже есть на рынке
			if( tech != NULL )
			{
				(*tech)[ INTEREST ] -= (int)refGame[ STARTGAMERATING ] / 1000.f * (float)(*tech)[ INTEREST ];
				Log(HW) << "techName " << (NrpText)(*tech)[ NAME ] << ": Interest " << (float)(*tech)[ INTEREST ] << term;
			}
			else //игры такого жанра на рынке нет, значит надо добавить интереса к игре
			{
				NrpText fileName = GetLink( genreName );
				assert( fileName.size() > 0 );
				if( fileName.size() > 0 )
				{
					CNrpTechnology* newGenre = new CNrpTechnology( fileName );
					AddTechnology( newGenre );
				}
			}
		}

		_games.push_back( game );
		_self[ GAMENUMBER ] = static_cast< int >( _games.size() );
	}
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
			assert( tech );
			if( tech )
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
	int year = _self[ CURRENTTIME ].As<NrpTime>().RYear();
	
	if( CNrpGameEngine* ge = GetGameEngine( (*game)[ GAME_ENGINE ].As<NrpText>() ) )
		year = (*ge)[ STARTDATE ].As<NrpTime>().RYear();

	for( u32 i=0; i < _screenshots.size(); i++ )
	{
		CNrpGame* alsoMargetGame = GetGame( (NrpText)_screenshots[ i ]->Param( INTERNAL_NAME ) );

		if( !alsoMargetGame ) 
		{
			if( !_screenshots[ i ]->IsMyYear( year ) )
				continue;
			
			int eqRating = _screenshots[ i ]->GetEqualeRating( game );

			if( !eqRating )
				continue;

			if( minimumRating < eqRating )
			{
				thisYearAndGenreImgs.clear();
				minimumRating = eqRating;
			}
			
			thisYearAndGenreImgs.push_back( _screenshots[ i ] );
		}
	}

	if( thisYearAndGenreImgs.size() )
	{
		int randomIndex = rand() % thisYearAndGenreImgs.size();
		return thisYearAndGenreImgs[ randomIndex ]->Param( INTERNAL_NAME );
	}

	//!!!!!!!!надо както обработать эту ситуацию!!!!!!!!!!!!!!!!!
	assert( NrpText("No find free name").size() == 0 );
	return "";
}

CNrpScreenshot* CNrpApplication::GetScreenshot( const NrpText& name )
{
	return FindByNameAndIntName< SCREENSHOTS, CNrpScreenshot >( _screenshots, name, NULL );
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

	Log( HW ) << "такой движок уже есть" << term;
}

INrpProject* CNrpApplication::GetProject( const NrpText& name )
{
	return FindByName< PROJECTS, INrpProject >( _projects, name );
}

void CNrpApplication::AddDevelopProject( nrp::INrpDevelopProject* project )
{
	if( FindByName< DEVPROJECTS, INrpDevelopProject >( _devProjects, project->Param( NAME ) ) == NULL )
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

	_self[ DEVELOPPROJECTS_NUMBER ] = static_cast< int >( _devProjects.size() );
}

void CNrpApplication::AddGame( CNrpGame* ptrGame )
{
	assert( ptrGame != NULL );
	_games.push_back( ptrGame );
	_self[ GAMENUMBER ] = static_cast< int >( _games.size() );
}

void CNrpApplication::AddInvention( const NrpText& fileName, CNrpCompany& parentCompany )
{
	std::auto_ptr< CNrpTechnology > startTech( new CNrpTechnology( fileName ) );

	CNrpInvention* tmp = GetInvention( (*startTech)[ INTERNAL_NAME ], parentCompany[ NAME ] );

	assert( !tmp );

	if( !tmp )
	{
		CNrpInvention* inv = new CNrpInvention( *startTech, parentCompany, _self[ CURRENTTIME ].As<NrpTime>() );
		parentCompany.AddInvention( inv );	
		_inventions.push_back( inv );  
		_self[ INVENTIONSNUMBER ] = static_cast< int >( _inventions.size() );
	}
}

void CNrpApplication::InventionFinished( CNrpInvention& ptrInvention )
{
	//создать соответствующую изобретению технологию 
	//разместить её в списке доступных
	CNrpTechnology* tech = new CNrpTechnology( ptrInvention );
	AddTechnology( tech );

	int delPos = -1;
	for( u32 i=0; i < _inventions.size(); i++ )
	{
		CNrpInvention& inv = *(_inventions[ i ]);
		if( (NrpText)inv[ INTERNAL_NAME ] == (NrpText)ptrInvention[ INTERNAL_NAME ] )
		{
			CNrpCompany* pCmp = inv[ PARENTCOMPANY ].As<PNrpCompany>();
			if( pCmp == ptrInvention[ PARENTCOMPANY ].As<PNrpCompany>() )
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
			NrpText text = NrpText( "Удалена технология " ) + (NrpText)(*inv)[ INTERNAL_NAME ];
			PNrpCompany cmp = (*inv)[ PARENTCOMPANY ].As<PNrpCompany>();
			assert( cmp != NULL );
			text.append( cmp ? (NrpText)(*cmp)[ NAME ] : NrpText("unknown company") );
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

CNrpPlatform* CNrpApplication::GetPlatform( size_t index )
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

void CNrpApplication::SetLink( const NrpText& name, const NrpText& pathto )
{
	assert( !_links.find( name )  );
	assert( pathto.size() > 0 );

	if( _links.find( name ) == NULL )
	{
		_links[ name ] = pathto;
	}
}

NrpText CNrpApplication::GetLink( const NrpText& name )
{
	LINK_MAP::Node* pIter = _links.find( name );
	assert( pIter );
	if( pIter )
		return pIter->getValue();

	return NrpText();
}

}//namespace nrp