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
#include "nrploadingscreen.h"
#include "NrpHistory.h"
#include "NrpGameTime.h"
#include "NrpTime.h"
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
#include "NrpLaborMarket.h"
#include "NrpSoundEngine.h"
#include "NrpQuestEngine.h"

#include <io.h>
#include <errno.h>
#include <OleAuto.h>

static nrp::CNrpApplication* globalApplication = NULL;

namespace nrp
{
CLASS_NAME CLASS_NRPAPPLICATION( "CNrpApplication" );

const NrpText CNrpApplication::FileName = "profile.ini";

CNrpApplication::CNrpApplication(void) : INrpConfig( CLASS_NRPAPPLICATION, CLASS_NRPAPPLICATION )
{
	Add( TECHNUMBER, (int)0 );
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
	Add<NrpText>( SAVEDIR_BANK, "" );
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
	Add( DEV_FORCE, 1.f );
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
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_BANK ] );
	OpFileSystem::CreateDirectory( _self[ SAVEDIR_BRIDGE ] );
}

void CNrpApplication::Save()
{
	scene::CLoadingScreen ld( _nrpEngine.GetVideoDriver(), _nrpEngine.GetGuiEnvironment()->getFont( "font_14" ) );

	NrpText prevSaveFolder = (NrpText)_self[ SAVEDIR ] + (NrpText)_self[ PROFILENAME ] + "Old/";
    NrpText profileIni = (NrpText)_self[ SAVEDIR_PROFILE ] + CNrpApplication::FileName;

	ld.render( 10, "Создаем структуру для сохранения" );
	OpFileSystem::Remove( prevSaveFolder );
	OpFileSystem::Copy( _self[ SAVEDIR_PROFILE ], prevSaveFolder );
    OpFileSystem::Rename( prevSaveFolder + CNrpApplication::FileName, prevSaveFolder + CNrpApplication::FileName + ".bak" );
	OpFileSystem::Remove( _self[ SAVEDIR_PROFILE ] );

	_CreateDirectoriesMapForSave();

	assert( !OpFileSystem::IsExist( profileIni ) );

	ld.render( 10, "Сохраняем основные данные" );
	INrpConfig::Save( profileIni );

	IniFile sv( profileIni );

	sv.Set( SECTION_OPTIONS, "currentProfile", (NrpText)_self[ PROFILENAME ] );
	sv.Set( SECTION_OPTIONS, "currentCompany", (NrpText)_self[ PROFILECOMPANY ] );
	
	ld.render( 10, "Сохраняем данные о компаниях" );
	for( u32 i=0; i < _companies.size(); i++ )
	{
		_companies[ i ]->Save( _self[ SAVEDIR_COMPANIES ] );
		sv.Set( SECTION_COMPANIES, CreateKeyCompany( i ), (NrpText)(*_companies[ i ])[ NAME ]);
	}

	ld.render( 10, "Сохраняем данные о проектах в разработке" );
	for( u32 i=0; i < _devProjects.size(); i++ )
	{
		NrpText saveFile = _devProjects[ i ]->Save( _self[ SAVEDIR_DEVPR ] );
		sv.Set( SECTION_DEVPROJECTS, CreateKeyType(i), _devProjects[ i ]->ObjectTypeName() );
		sv.Set( SECTION_DEVPROJECTS, CreateKeyProject(i), saveFile );
	}

	ld.render( 10, "Сохраняем данные о проектах" );
	for( u32 i=0; i < _projects.size(); i++ )
	{
		NrpText saveFile = _projects[ i ]->Save( _self[ SAVEDIR_PROJECTS ] );
		sv.Set( SECTION_DEVPROJECTS, CreateKeyType(i), _projects[ i ]->ObjectTypeName() );
		sv.Set( SECTION_PROJECTS, CreateKeyProject(i), saveFile );
	}

	ld.render( 10, "Сохраняем данные о технологиях" );
	for( u32 i=0; i < _technologies.size(); i++ )
	{
		NrpText saveFile = _technologies[ i ]->Save( _self[ SAVEDIR_TECHS ] );
		sv.Set( SECTION_TECHS, CreateKeyTech(i), saveFile );
	}

	ld.render( 10, "Сохраняем данные oб изобретениях" );
	for( u32 i=0; i < _inventions.size(); i++ )
	{
		NrpText inventSaveFile = _inventions[ i ]->Save( (NrpText)_self[ SAVEDIR_INVENTIONS ] + 
														 (NrpText)_inventions[ i ]->Param( COMPANYNAME ), true );
		sv.Set( SECTION_INVENTIONS, CreateKeyInvention( i ), inventSaveFile );
	}

	ld.render( 10, "Сохраняем данные о видеодвижках" );
	for( u32 i=0; i < _engines.size(); i++ )
	{
		NrpText saveFolder = _engines[ i ]->Save( _self[ SAVEDIR_ENGINES ] );
		sv.Set( SECTION_ENGINES, CreateKeyEngine(i), saveFolder );
	}

	ld.render( 10, "Сохраняем данные об играх" );
	for( u32 i=0; i < _games.size(); i++ )
	{
		NrpText saveDir = _games[ i ]->Save( _self[ SAVEDIR_GAMES ] );
		sv.Set( SECTION_GAMES, CreateKeyGame( i ), saveDir );
	}

	ld.render( 10, "Сохраняем данные о платформах" );
	for( u32 i=0; i < _platforms.size(); i++ )
	{
		NrpText saveDir = _platforms[ i ]->Save( _self[ SAVEDIR_PLATFORMS ] );
		sv.Set( SECTION_PLATFORMS, CreateKeyPlatform( i ), saveDir );
	}

	sv.Save();

	ld.render( 10, "Сохраняем данные банка" );
	CNrpBank::Instance().Save( _self[ SAVEDIR_BANK ] );

	ld.render( 10, "Сохраняем данные о биржи" );
	CNrpBridge::Instance().Save( _self[ SAVEDIR_BRIDGE ] );

	ld.render( 10, "Сохраняем данные о сотрудниках" );
	CNrpLaborMarket::Instance().Save( _self[ SAVEDIR_PROFILE ] );

    ld.render( 10, "Сохраняем результаты квестов" );
    CNrpQuestEngine::Instance().Save( _self[ SAVEDIR_PROFILE ] );
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
	_self[ SAVEDIR_BANK ] = profileDir + "bank/";
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

	CNrpLaborMarket::Instance().Load( _self[ SAVEDIR_PROFILE ] );

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
		if( !OpFileSystem::IsExist( fileName ) )
			continue;

		PNrpGame game = new CNrpGame( fileName );
		if( (bool)(*game)[ LOADOK ] )
			_games.push_back( game );
		else
			Log( HW ) << "!!!WARNING!!! Can't load game from " << fileName << term;
	}
	_self[ GAMENUMBER ] = static_cast< int >( _games.size() );

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
	CNrpBank::Instance().Load( _self[ SAVEDIR_BANK ] );
	CNrpScript::Instance().TemporaryScript( AFTER_LOAD_SCRIPT, CNrpScript::SA_EXEC );
    CNrpQuestEngine::Instance().Load( (NrpText)_self[ SAVEDIR_PROFILE ] );
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

		CNrpExtInfo* pScr = new CNrpExtInfo( scrFile );
		
		if( FindByNameAndIntName< SCREENSHOTS, CNrpExtInfo>( _screenshots, (*pScr)[ INTERNAL_NAME ], NULL ) == NULL )
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
	_self[ PROFILENAME ] = profileName;
	_self[ PROFILECOMPANY ] = companyName;

	_InitialyzeSaveDirectories( profileName );
}

void CNrpApplication::ResetData()
{
	ClearArray( _companies );
	CNrpLaborMarket::Instance().Reset();
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
	_nrpSound.Instance();
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

void CNrpApplication::_UpdateGameRating( CNrpGame& game )
{
	if( !(bool)game[ GAMEISSALING ] )
		return;

	std::map< int, int > qualityMap;

	//вычисляем сколько месяцев на рынке игра понижаем рейтинг из-за времени на рынке
	int monthInMarket = game[ STARTDATE ].As<NrpTime>().GetMonthToDate( _self[ CURRENTTIME ].As<NrpTime>() ) + 1;
	int bs = (bool)game[ BESTSALER ] ? 50 : 0;

	if( (bool)game[ NPC_GAME ] )
	{
		//игры которые не выпущены игроком, расчитывают рейтинг простым снижением
		qualityMap[ PT_VIDEOTECH ] = (int)game[ STARTGRAPHICRATING ] / monthInMarket;
		qualityMap[ PT_SOUNDTECH ] = (int)game[ STARTSOUNDRATING ] / monthInMarket;
		qualityMap[ PT_GENRE ] = (int)game[ CURRENTGENRERATING ] / monthInMarket;
	}
	else
	{
		//подсчитаем рейтинги модулей для игр, которые были выпущены игроком
		for( int k=0; k < (int)game[ MODULE_NUMBER ]; k++ )
		{
			NrpText name = game.GetTechName( k );
			if( CNrpTechnology* tech = GetTechnology( name ) )
			{
				int tg = (*tech)[ TECHGROUP ];
				int rt = (int)(*tech)[ QUALITY ] / monthInMarket;
				
				//распределим технологии по группам
				if( qualityMap[ tg ] == 0 )
					qualityMap[ tg ] = rt;
				else
					qualityMap[ tg ] = (qualityMap[ tg ] + rt) / 2;		
			}
		}
	}

	//результат подсчета рейтинга
	if( CNrpHistoryStep* step = game.GetHistory()->AddStep( _self[ CURRENTTIME ].As<NrpTime>() ) )
	{
		//занесем рейтинг в историю игры
		//если игрушка хитовая, то её рейтинг не опускается ниже 50%
		//todo: Слежение за хитовостью игры ограничением рейтинга можно перенести в саму игру
		game[ CURRENTGRAPHICRATING ] = (std::max)( bs, qualityMap[ PT_VIDEOTECH ] );
		step->AddValue( CURRENTGRAPHICRATING, (int)game[ CURRENTGRAPHICRATING ] );
		
		game[ CURRENTSOUNDRATING ] = (std::max)( bs, qualityMap[ PT_SOUNDTECH ] );
		step->AddValue( CURRENTSOUNDRATING, (int)game[ CURRENTSOUNDRATING ] );

		game[ CURRENTGENRERATING ] = (std::max)( bs, qualityMap[ PT_GENRE ] );
		step->AddValue( CURRENTGENRERATING, (int)game[ CURRENTGENRERATING ] );

		step->AddValue( CURRENTBUGRATING, (int)game[ CURRENTBUGRATING ] );
	
		int midRating = ( qualityMap[ PT_VIDEOTECH ] + qualityMap[ PT_SOUNDTECH ] + qualityMap[ PT_GENRE ] ) / 3;
		step->AddValue( CURRENTGAMERATING, midRating );
	}
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
		_UpdateGameRating( *_games[ i ] );

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

void CNrpApplication::AddGameToMarket( CNrpGame& game )
{
	if( (bool)game[ GAMEISSALING ] )
		return;

	game[ GAMEISSALING ] = true;

	//посмотрим чтобы такой игры не было на рынке
	if( FindByNameAndIntName< GAMES, CNrpGame >( _games, game[ INTERNAL_NAME ] ) == NULL )
	{
		//если игра не скриптовая, то надо расчитать и запомнить начальные рейтинги
		if( !(bool)game[ NPC_GAME ] )
		{
			_UpdateGameRating( game );
			game[ STARTGAMERATING ] = game[ CURRENTGAMERATING ];
			game[ STARTGRAPHICRATING ] = game[ CURRENTGRAPHICRATING ];
			game[ STARTSOUNDRATING ] = game[ CURRENTSOUNDRATING ];
			game[ STARTGENRERATING ] = game[ CURRENTGENRERATING ];
		}
		else
		{//когда игра загружается по ходу игры, то её начальные рейтинги становятся текущими
			//до первого обновления рейтингов
			game[ CURRENTGAMERATING ] = game[ STARTGAMERATING ];
			game[ CURRENTGRAPHICRATING ] = game[ STARTGRAPHICRATING ];
			game[ CURRENTSOUNDRATING ] = game[ STARTSOUNDRATING ];
			game[ CURRENTGENRERATING ] = game[ STARTGENRERATING ];
		}

		//когда игра выходит на рынок, то она влияет на него
		for( int i=0; i < (int)game[ GENRE_MODULE_NUMBER ]; i++ )
		{
			NrpText genreName = game.GetGenreName( i );
			//влияние приводит к изменению интереса к жанру игры
			CNrpTechnology* tech = GetTechnology( genreName );
			//такой жанр уже есть на рынке
			if( tech != NULL )
			{
				//значение каждого последующего жанра в игре оказывает все меньшее влияние
				float koeff = (int)game[ STARTGAMERATING ] / (1000.f + i*300.f) * (float)(*tech)[ INTEREST ];
				(*tech)[ INTEREST ] -= koeff;
				Log(HW) << "techName " << (NrpText)(*tech)[ NAME ] << ": Interest " << (float)(*tech)[ INTEREST ] << term;
			}
			else //игры такого жанра на рынке нет, значит надо добавить интереса к игре
			{
				NrpText fileName = GetLink( genreName );
				assert( fileName.size() > 0 );
				//и вывести технологию на рынок
				if( fileName.size() > 0 )
				{
					CNrpTechnology* newGenre = new CNrpTechnology( fileName );
					AddTechnology( newGenre );
				}
			}
		}

		AddGame( &game );
		PCall( APP_NEWGAME_ONMARKET, this, &game );
	}
	else
		Log( HW ) << "!!!CNrpApplication::AddGameToMarket::Warning!!! Game " << (NrpText)game[ INTERNAL_NAME ] << " also have on market" << term;
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
NrpText CNrpApplication::GetFreeInternalName( const CNrpGame& game )
{
	SCREENSHOTS	thisYearAndGenreImgs;
	
	int minimumRating = 1;
	//инициализация года выпуска игры
	int year = _self[ CURRENTTIME ].As<NrpTime>().RYear();
	
	//если у игры есть движок, то возьмем год его выпуска... +-1 год
	if( CNrpGameEngine* ge = GetGameEngine( game[ GAME_ENGINE ].As<NrpText>() ) )
		year = (*ge)[ STARTDATE ].As<NrpTime>().RYear();

	for( u32 i=0; i < _screenshots.size(); i++ )
	{
		//получим процент совпадения жанров
		int eqRating = _screenshots[ i ]->GetEqualeRating( game );

		//если слишком разнятся годы выпуска или не совпадает по жанрам
		if( !_screenshots[ i ]->IsMyYear( year ) || !eqRating || eqRating < minimumRating )
			continue;

		if( minimumRating < eqRating )
		{
			thisYearAndGenreImgs.clear();
			minimumRating = eqRating;
		}

		thisYearAndGenreImgs.push_back( _screenshots[ i ] );
	}

	if( thisYearAndGenreImgs.size() )
	{
		int randomIndex = rand() % thisYearAndGenreImgs.size();
		return thisYearAndGenreImgs[ randomIndex ]->Param( INTERNAL_NAME );
	}

	//!!!!!!!!надо както обработать эту ситуацию!!!!!!!!!!!!!!!!!
	assert( false && "No find free name" );
	return "";
}

CNrpExtInfo* CNrpApplication::GetExtInfo( const NrpText& name )
{
	return FindByNameAndIntName< SCREENSHOTS, CNrpExtInfo >( _screenshots, name, NULL ); 
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
	CNrpGameEngine* engine = GetGameEngine( (*ptrEngine)[ INTERNAL_NAME ] );
	if( engine == NULL )
	{
		_engines.push_back( ptrEngine );
		_self[ ENGINES_NUMBER ] = static_cast<int>( _engines.size() );
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
	CNrpGame* hG = GetGame( (NrpText)(*ptrGame)[ INTERNAL_NAME ] );

	if( !hG ) 
	{
		_games.push_back( ptrGame );
		_self[ GAMENUMBER ] = static_cast< int >( _games.size() );
	}
	else
		Log( HW ) << "Also have game " << (NrpText)(*ptrGame)[ INTERNAL_NAME ] << " on market" << term;
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

	PCall( APP_INVENTION_FINISHED, tech );
	
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
	LINK_MAP::Node* linkNode = _links.find( name );
	assert( !linkNode  );

	assert( pathto.size() > 0 );

	if( !linkNode )
		_links[ name ] = pathto;
	else
		Log( HW ) << "Link with internal name " << name << " also exist in " << linkNode->getValue() << term;
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