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
	Push<PNrpCompany>( PARENTCOMPANY, NULL );
	Push<NrpText>( COMPANYNAME, "" );
	Push<NrpText>( NAME, "" );
	Push<NrpText>( INTERNAL_NAME, "" );
	Push<SYSTEMTIME>( STARTDATE, SYSTEMTIME() );
	Push<SYSTEMTIME>( ENDDATE, SYSTEMTIME() );
	Push<int>( MONEYONDEVELOP, 0 );
	Push<int>( CASH, 0 );
	Push<int>( COPYSELL, 0 );
	Push<int>( STARTRATING, 0 );
	Push<int>( STARTGAMERATING, 0 );
	Push<int>( STARTGRAPHICRATING, 0 );
	Push<int>( STARTGENRERATING, 0 );
	Push<int>( STARTSOUNDRATING, 0 );
	Push<int>( STARTADVFUNCRATING, 0 );
	Push<int>( CURRENTADVFUNCRATING, 0 );
	Push<int>( STARTBUGRATING, 0 );
	Push<int>( CURRENTGAMERATING, 0 );
	Push<int>( CURRENTGRAPHICRATING, 0 );
	Push<int>( CURRENTGENRERATING, 0 );
	Push<int>( CURRENTSOUNDRATING, 0 );
	Push<int>( LOCALIZATIONRATING, 0 );
	Push<int>( CURRENTBUGRATING, 0 );	
	Push<NrpText>( GAME_ENGINE, "" );
	Push<NrpText>( PREV_GAME, "" );
	Push<int>( GENRE_MODULE_NUMBER, 0 );
	Push<PNrpGameBox>( GBOX, NULL );
	Push<float>( FAMOUS, 0 );
	Push<bool>( GAMEISSALING, false );
	Push<CNrpScreenshot*>( GAMEIMAGELIST, NULL );
	Push<NrpText>( GAMERETAILER, "" );
	Push<int>( PLATFORMNUMBER, 0 );
	Push<int>( MODULE_NUMBER, 0 );
	Push<int>( USERNUMBER, 0 );
	Push<NrpText>( DESCRIPTIONPATH, "" );
	Push<NrpText>( TEXTURENORMAL, "" );
	Push<float>( READYWORKPERCENT, 1.f );
	Push<NrpText>( VIEWIMAGE, "" );
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

	Param( MODULE_NUMBER ) = refGame[ MODULE_NUMBER ];
	for( int cnt=0; cnt < (int)Param( MODULE_NUMBER ); cnt++ )
		 _techs.push_back( refGame.GetModule( cnt )->Param( INTERNAL_NAME ) );

	Param( INTERNAL_NAME ) = CNrpApplication::Instance().GetFreeInternalName( this );
	CNrpScreenshot* pgList = CNrpApplication::Instance().GetScreenshot( Param( INTERNAL_NAME ) );
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
		IUser* user = CNrpApplication::Instance().GetUser( _developers[ i ] );
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

		SYSTEMTIME curTime = CNrpApplication::Instance()[ CURRENTTIME ].As<SYSTEMTIME>();
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