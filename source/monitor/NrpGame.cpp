#include "StdAfx.h"
#include "NrpGame.h"
#include "NrpCompany.h"
#include "NrpGameProject.h"
#include "NrpTechnology.h"
#include "NrpGameBox.h"
#include "NrpScreenshot.h"
#include "NrpApplication.h"
#include "NrpDevelopGame.h"
#include "NrpProjectModule.h"
#include "IUser.h"
#include "OpFileSystem.h"
#include "NrpHistory.h"
#include "NrpGameEngine.h"
#include "IniFile.h"
#include "NrpTime.h"

#include <errno.h>
#include <OleAuto.h>

namespace nrp
{
CLASS_NAME CLASS_NRPGAME( "CNrpGame" );
	
void CNrpGame::_InitializeOptions()
{
	Add<PNrpCompany>( PARENTCOMPANY, NULL );
	Add<NrpText>( COMPANYNAME, "" );
	Add<NrpText>( NAME, "" );
	Add<NrpText>( INTERNAL_NAME, "" );
	Add<NrpTime>( STARTDATE, NrpTime( 0. ) );
	Add<NrpTime>( ENDDATE, NrpTime( 0. ) );
	Add<int>( MONEYONDEVELOP, 0 );
	Add<int>( CASH, 0 );
	Add<NrpText>( OWNER, "" );
	Add<int>( COPYSELL, 0 );
	Add<int>( STARTRATING, 0 );
	Add<int>( STARTGAMERATING, 0 );
	Add<int>( STARTGRAPHICRATING, 0 );
	Add<int>( STARTGENRERATING, 0 );
	Add<int>( STARTSOUNDRATING, 0 );
	Add<int>( STARTADVFUNCRATING, 0 );
	Add<int>( CURRENTADVFUNCRATING, 0 );
	Add<int>( STARTBUGRATING, 0 );
	Add<int>( CURRENTGAMERATING, 0 );
	Add<int>( CURRENTGRAPHICRATING, 0 );
	Add<int>( CURRENTGENRERATING, 0 );
	Add<int>( CURRENTSOUNDRATING, 0 );
	Add<int>( LOCALIZATIONRATING, 0 );
	Add<int>( CURRENTBUGRATING, 0 );	
	Add<NrpText>( GAME_ENGINE, "" );
	Add<NrpText>( PREV_GAME, "" );
	Add<int>( GENRE_MODULE_NUMBER, 0 );
	Add<PNrpGameBox>( GBOX, NULL );
	Add<float>( FAMOUS, 0 );
	Add<bool>( GAMEISSALING, false );
	Add<CNrpExtInfo*>( GAMEIMAGELIST, NULL );
	Add<NrpText>( GAMERETAILER, "" );
	Add<int>( PLATFORMNUMBER, 0 );
	Add<int>( MODULE_NUMBER, 0 );
	Add<int>( USERNUMBER, 0 );
	Add<NrpText>( DESCRIPTIONPATH, "" );
	Add<NrpText>( TEXTURENORMAL, "" );
	Add<float>( READYWORKPERCENT, 1.f );
	Add<NrpText>( VIEWIMAGE, "" );
	Add<NrpText>( RECENSE, "" );
	Add<bool>( NPC_GAME, false );
	Add<bool>( LOADOK, false );
	Add<bool>( BESTSALER, false );
	_history = NULL;
}

CNrpGame::CNrpGame( CNrpDevelopGame* devGame, CNrpCompany* ptrCompany ) : INrpConfig( CLASS_NRPGAME, devGame->Text( NAME ) )
{
	_InitializeOptions(); 

	assert( devGame );
	CNrpDevelopGame& refGame = *devGame;
	_self[ PARENTCOMPANY ] = ptrCompany;
	_self[ COMPANYNAME ] = (*ptrCompany)[ NAME ];
	_self[ NAME ] = refGame[ NAME ];
	_self[ MONEYONDEVELOP ] = refGame[ MONEYONDEVELOP ];
	CNrpGameEngine* ge = refGame[ GAME_ENGINE ].As<CNrpGameEngine*>();
	if( ge )
		_self[ GAME_ENGINE ] = (*ge)[ NAME ];

	_self[ FAMOUS ] = refGame[ FAMOUS ];
	assert( refGame[ PLATFORMNUMBER ] != (int)0 );
	_self[ PLATFORMNUMBER ] = refGame[ PLATFORMNUMBER ];
	_self[ USERNUMBER ] = refGame[ USERNUMBER ];
	_self[ PREV_GAME ] = refGame[ NAME ]; 
	
	_developers = devGame->GetDevelopers();

	_self[ GENRE_MODULE_NUMBER ] = refGame[ GENRE_MODULE_NUMBER ];
	for( int cnt=0; cnt < (int)_self[ GENRE_MODULE_NUMBER ]; cnt++ )
	{
		 CNrpTechnology* genre = devGame->GetGenre( cnt );
		 if( genre )
			_genres.push_back( (*genre)[ INTERNAL_NAME ] );
	}

	_self[ MODULE_NUMBER ] = refGame[ MODULE_NUMBER ];
	for( int cnt=0; cnt < (int)_self[ MODULE_NUMBER ]; cnt++ )
		 _techs.push_back( refGame.GetModule( cnt )->Param( INTERNAL_NAME ) );

	_self[ INTERNAL_NAME ] = _nrpApp.GetFreeInternalName( *this );
	assert( ((NrpText)_self[ INTERNAL_NAME ]).size() > 0 );

	CNrpExtInfo* pgList = _nrpApp.GetExtInfo( _self[ INTERNAL_NAME ] );
	assert( pgList != NULL );
	if( pgList != NULL )
	{
		_self[ GAMEIMAGELIST ] = pgList;
		_self[ VIEWIMAGE ] = pgList->GetBoxImages()[ 0 ];
	}

	_history = new CNrpHistory();
}

CNrpGame::CNrpGame( const NrpText& fileName ) : INrpConfig( CLASS_NRPGAME, fileName )
{
	_InitializeOptions();
	Load( fileName );
}

CNrpGame::CNrpGame() : INrpConfig( CLASS_NRPGAME, "" )
{

}

CNrpGame::~CNrpGame(void)
{
}

NrpText CNrpGame::Save( const NrpText& saveFolder )
{
	bool upDirExist = OpFileSystem::IsExist( saveFolder );
	assert( upDirExist );

	if( !upDirExist )
		return "";
	
	NrpText localFolder = OpFileSystem::CheckEndSlash( saveFolder ) + Text( NAME ); //директория куда сохраняем
	localFolder = OpFileSystem::CheckEndSlash( localFolder ); //добавляем слеш в конец
	NrpText saveFile = localFolder + "item.game";

	OpFileSystem::CreateDirectory( localFolder );
	INrpConfig::Save( saveFile );

	if( PNrpGameBox box = Param( GBOX ).As<PNrpGameBox>() )
	{
		box->Save( localFolder + "box.ini" );
	}

	IniFile sv( saveFile );
	sv.Set( SECTION_TECHS, CreateKeyTech, _techs );
	sv.Set( SECTION_GENRES, CreateKeyGenre, _genres);
	sv.Set( SECTION_PLATFORMS, CreateKeyPlatform, _platforms );

	assert( _history );
	if( _history )
		_history->Save( localFolder + "game.history" );

	sv.Save();

	return localFolder;
}

void CNrpGame::Load( const NrpText& loadPath )
{
	_self[ LOADOK ] = OpFileSystem::IsExist( loadPath );
	assert( (bool)_self[ LOADOK ] && "can't load game config file" );

	if( !(bool)_self[ LOADOK ] )
		return;

	NrpText loadFolder, loadFile ;
	if( OpFileSystem::IsFolder( loadPath ) )
	{
		loadFolder = loadPath; 
		//стандартное имя
		loadFile = OpFileSystem::CheckEndSlash( loadFolder ) + "item.game";
	}
	else 
	{
		assert( OpFileSystem::GetExtension( loadPath ).equals_ignore_case( ".game" ) );
		if( OpFileSystem::GetExtension( loadPath ).equals_ignore_case( ".game" ) )
		{
			loadFile = loadPath;
			loadFolder = OpFileSystem::UpDir( loadPath );
		}
		else
			return;
	}

	INrpConfig::Load( loadFile );
	IniFile rv( loadFile );

	rv.Get( SECTION_TECHS, CreateKeyTech, -1, _techs );
	_self[ MODULE_NUMBER ] = static_cast< int >( _techs.size() );

	rv.Get( SECTION_GENRES, CreateKeyGenre, -1,	_genres );
	_self[ GENRE_MODULE_NUMBER ] = static_cast< int >( _genres.size() );

	rv.Get( SECTION_PLATFORMS, CreateKeyPlatform, -1, _platforms );
	_self[ PLATFORMNUMBER ] = static_cast< int >( _platforms.size() );

	NrpText boxIni = loadFolder + "box.ini";
	if( OpFileSystem::IsExist( boxIni ) )
	{
		PNrpGameBox box = new CNrpGameBox( this );
		box->Load( boxIni );
		_self[ GBOX ] = box;
	}

	NrpText historyIni = loadFolder + "game.history";
	if( OpFileSystem::IsExist( historyIni ) )
		_history = new CNrpHistory( historyIni );
	else
		_CreateHistory();

	CNrpExtInfo* pgList = CNrpApplication::Instance().GetExtInfo( _self[ INTERNAL_NAME ] );
	if( !pgList )
		Log( HW ) << "Can't find extInfo for " << (NrpText)_self[ INTERNAL_NAME ] << " when load from " << loadFile << term;

	assert( pgList != NULL );
	if( pgList != NULL )
	{
		_self[ GAMEIMAGELIST ] = pgList;

		assert( pgList->GetBoxImages().size() > 0 );
		if( pgList->GetBoxImages().size() )	
			_self[ VIEWIMAGE ] = pgList->GetBoxImages()[ 0 ];
	}
}

void CNrpGame::_CreateHistory()
{
	_history = new CNrpHistory();
	assert( _history );

	int fMonth = NrpTime( _self[ STARTDATE ] ).GetMonthToDate( _self[ ENDDATE ].As<NrpTime>() );//полное количество месяцев жизни игры
	int sale = _self[ COPYSELL ];
	int profit = _self[ CASH ];

	//стандартный расчет для продаж игры
	//продажи максимальны на старте и уменьшаются с каждым месяцем
	float step = 1.f / fMonth;
	for( int i=0; i < fMonth; i++ )
	{
		NrpTime ft( _self[ STARTDATE ] );
		CNrpHistoryStep* historyStep = _history->AddStep( ft.AppendMonth( i ) );
		//вычисляем коеффициент продаж в этом месяце
		float percent = ( 1 - i * step ) * step * 2;
		(*historyStep)[ BOXNUMBER ] = static_cast< int >( sale * percent );
		(*historyStep)[ BALANCE ] = static_cast< int >( profit * percent );
	}
}

float CNrpGame::GetAuthorFamous()
{
	float summ = 0.1f;
	for( u32 i=0; i < _developers.size(); i++ )
	{
		CNrpUser* user = _nrpApp.GetUser( _developers[ i ] );
		if( user )
		{
			summ += (*user)[ FAMOUS ].As<float>();
			summ /= 2.f;
		}
	}
	return summ;
}

NrpText CNrpGame::GetGenreName( size_t index )
{
	return index < _genres.size() ? _genres[ index ] : "";
}

NrpText CNrpGame::GetTechName( size_t index )
{
    return index < _techs.size() ? _techs[ index ] : "";
}

void CNrpGame::GameBoxSaling( int number )
{
	PNrpGameBox gameBox = _self[ GBOX ].As<PNrpGameBox>();
	Log(HW) << "gameBox == NULL " << Text( INTERNAL_NAME ) << term;

	if( gameBox != NULL )
	{
		CNrpGameBox& refBox = *gameBox;
		int boxNumber = refBox[ BOXNUMBER ];
		number = number > boxNumber ? boxNumber : number;
		
		refBox[ BOXNUMBER ] -= number;
		int price = refBox[ PRICE ];

		_self[ CASH ] += price * number;
		_self[ COPYSELL ] += number;

		PNrpCompany cmp = _self[ PARENTCOMPANY ].As<PNrpCompany>();
		if( cmp )
			(*cmp)[ BALANCE ] += price * number;

		NrpTime curTime =_nrpApp[ CURRENTTIME ].As<NrpTime>();

		assert( _history );
		if( _history )
		{
			if( CNrpHistoryStep* step = _history->AddStep( curTime ) )
			{
				step->AddValue( BOXNUMBER, number );
				step->AddValue( BALANCE,  price * number );
			}
		}
	}
}

bool CNrpGame::IsGenreAvaible( const NrpText& name ) const
{
	for( size_t k=0; k < _genres.size(); k++ )
		if( _genres[ k ] == name )
			return true;

	return false;
}

CNrpHistory* CNrpGame::GetHistory()
{
	assert( _history );
	return _history;
}

NrpText CNrpGame::ClassName()
{
	return CLASS_NRPGAME;
}

}//namespace nrp