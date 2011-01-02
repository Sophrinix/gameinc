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

#include <errno.h>
#include <OleAuto.h>

namespace nrp
{
CLASS_NAME CLASS_NRPGAME( "CNrpGame" );
	
void CNrpGame::InitializeOptions_()
{
	Add<PNrpCompany>( PARENTCOMPANY, NULL );
	Add<NrpText>( COMPANYNAME, "" );
	Add<NrpText>( NAME, "" );
	Add<NrpText>( INTERNAL_NAME, "" );
	Add<SYSTEMTIME>( STARTDATE, SYSTEMTIME() );
	Add<SYSTEMTIME>( ENDDATE, SYSTEMTIME() );
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
	Add<CNrpScreenshot*>( GAMEIMAGELIST, NULL );
	Add<NrpText>( GAMERETAILER, "" );
	Add<int>( PLATFORMNUMBER, 0 );
	Add<int>( MODULE_NUMBER, 0 );
	Add<int>( USERNUMBER, 0 );
	Add<NrpText>( DESCRIPTIONPATH, "" );
	Add<NrpText>( TEXTURENORMAL, "" );
	Add<float>( READYWORKPERCENT, 1.f );
	Add<NrpText>( VIEWIMAGE, "" );
	_history = NULL;
}

CNrpGame::CNrpGame( CNrpDevelopGame* devGame, CNrpCompany* ptrCompany ) : INrpConfig( CLASS_NRPGAME, devGame->Text( NAME ) )
{
	InitializeOptions_();

	assert( devGame );
	CNrpDevelopGame& refGame = *devGame;
	Param( PARENTCOMPANY ) = ptrCompany;
	Param( COMPANYNAME ) = (*ptrCompany)[ NAME ];
	Param( NAME ) = refGame[ NAME ];
	Param( MONEYONDEVELOP ) = refGame[ MONEYONDEVELOP ];
	CNrpGameEngine* ge = refGame[ GAME_ENGINE ].As<CNrpGameEngine*>();
	if( ge )
		Param( GAME_ENGINE ) = (*ge)[ NAME ];

	Param( FAMOUS ) = refGame[ FAMOUS ];
	assert( refGame[ PLATFORMNUMBER ] != (int)0 );
	Param( PLATFORMNUMBER ) = refGame[ PLATFORMNUMBER ];
	Param( USERNUMBER ) = refGame[ USERNUMBER ];
	Param( PREV_GAME ) = refGame[ NAME ]; 
	
	_developers = devGame->GetDevelopers();

	Param( GENRE_MODULE_NUMBER ) = refGame[ GENRE_MODULE_NUMBER ];
	for( int cnt=0; cnt < (int)Param( GENRE_MODULE_NUMBER ); cnt++ )
	{
		 CNrpTechnology* genre = devGame->GetGenre( cnt );
		 if( genre )
			_genres.push_back( (*genre)[ INTERNAL_NAME ] );
	}

	_self[ MODULE_NUMBER ] = refGame[ MODULE_NUMBER ];
	for( int cnt=0; cnt < (int)Param( MODULE_NUMBER ); cnt++ )
		 _techs.push_back( refGame.GetModule( cnt )->Param( INTERNAL_NAME ) );

	_self[ INTERNAL_NAME ] = _nrpApp.GetFreeInternalName( this );
	CNrpScreenshot* pgList = _nrpApp.GetScreenshot( Param( INTERNAL_NAME ) );
	assert( pgList != NULL );
	if( pgList != NULL )
		Param( GAMEIMAGELIST ) = pgList;
}

CNrpGame::CNrpGame( const NrpText& fileName ) : INrpConfig( CLASS_NRPGAME, fileName )
{
	InitializeOptions_();
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
	for( u32 i=0; i < _techs.size(); i++ )
		 sv.Set( SECTION_TECHS, CreateKeyTech(i), _techs[ i ] );

	for( u32 i=0; i < _genres.size(); i++ )
		sv.Set( SECTION_GENRES, CreateKeyGenre(i),_genres[ i ] );

	return localFolder;
}

void CNrpGame::Load( const NrpText& loadPath )
{
	bool mayLoad = OpFileSystem::IsExist( loadPath );
	assert( mayLoad );

	if( !mayLoad )
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
	for( int i=0; i < (int)Param( MODULE_NUMBER ); ++i )
		_techs.push_back( rv.Get( SECTION_TECHS, CreateKeyTech(i), NrpText("") ) );

	for( int i=0; i < (int)Param( GENRE_MODULE_NUMBER ); ++i )
		_genres.push_back( rv.Get( SECTION_GENRES, CreateKeyGenre(i), NrpText("") ) );

	NrpText boxIni = loadFolder + "box.ini";
	if( OpFileSystem::IsExist( boxIni ) )
	{
		PNrpGameBox box = new CNrpGameBox( this );
		box->Load( boxIni );
		Param( GBOX ) = box;
	}

	CNrpScreenshot* pgList = CNrpApplication::Instance().GetScreenshot( Text( INTERNAL_NAME ) );
	assert( pgList != NULL );
	if( pgList != NULL )
		Param( GAMEIMAGELIST ) = pgList;
}

float CNrpGame::GetAuthorFamous()
{
	float summ = 0.1f;
	for( u32 i=0; i < _developers.size(); i++ )
	{
		IUser* user = _nrpApp.GetUser( _developers[ i ] );
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
	PNrpGameBox gameBox = Param( GBOX ).As<PNrpGameBox>();
	Log(HW) << "gameBox == NULL " << Text( INTERNAL_NAME ) << term;

	if( gameBox != NULL )
	{
		int boxNumber = (*gameBox)[ BOXNUMBER ];
		number = number > boxNumber ? boxNumber : number;
		
		(*gameBox)[ BOXNUMBER ] -= number;
		int price = (*gameBox)[ PRICE ];

		Param( CASH ) += price * number;
		Param( COPYSELL ) += number;

		PNrpCompany cmp = Param( PARENTCOMPANY ).As<PNrpCompany>();
		if( cmp )
			(*cmp)[ BALANCE ] += price * number;

		SYSTEMTIME curTime =_nrpApp[ CURRENTTIME ].As<SYSTEMTIME>();
	}
	//history_->AddStep( CURRENTTIME, number, price * number );
}

bool CNrpGame::IsGenreAvaible( const NrpText& name )
{
	for( size_t k=0; k < _genres.size(); k++ )
		if( _genres[ k ] == name )
			return true;

	return false;
}

CNrpHistory* CNrpGame::GetHistory()
{
	if( !_history )
		_history = new CNrpHistory();

	return _history;
}

NrpText CNrpGame::ClassName()
{
	return CLASS_NRPGAME;
}

}//namespace nrp