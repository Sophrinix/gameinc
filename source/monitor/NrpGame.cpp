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
	CreateValue<PNrpCompany>( PARENTCOMPANY, NULL );
	CreateValue<NrpText>( COMPANYNAME, "" );
	CreateValue<NrpText>( NAME, "" );
	CreateValue<NrpText>( INTERNAL_NAME, "" );
	CreateValue<SYSTEMTIME>( STARTDATE, SYSTEMTIME() );
	CreateValue<SYSTEMTIME>( ENDDATE, SYSTEMTIME() );
	CreateValue<int>( MONEYONDEVELOP, 0 );
	CreateValue<int>( CASH, 0 );
	CreateValue<int>( COPYSELL, 0 );
	CreateValue<int>( STARTRATING, 0 );
	CreateValue<int>( STARTGAMERATING, 0 );
	CreateValue<int>( STARTGRAPHICRATING, 0 );
	CreateValue<int>( STARTGENRERATING, 0 );
	CreateValue<int>( STARTSOUNDRATING, 0 );
	CreateValue<int>( STARTADVFUNCRATING, 0 );
	CreateValue<int>( CURRENTADVFUNCRATING, 0 );
	CreateValue<int>( STARTBUGRATING, 0 );
	CreateValue<int>( CURRENTGAMERATING, 0 );
	CreateValue<int>( CURRENTGRAPHICRATING, 0 );
	CreateValue<int>( CURRENTGENRERATING, 0 );
	CreateValue<int>( CURRENTSOUNDRATING, 0 );
	CreateValue<int>( LOCALIZATIONRATING, 0 );
	CreateValue<int>( CURRENTBUGRATING, 0 );	
	CreateValue<NrpText>( GAME_ENGINE, "" );
	CreateValue<NrpText>( PREV_GAME, "" );
	CreateValue<int>( GENRE_MODULE_NUMBER, 0 );
	CreateValue<PNrpGameBox>( GBOX, NULL );
	CreateValue<float>( FAMOUS, 0 );
	CreateValue<bool>( GAMEISSALING, false );
	CreateValue<CNrpScreenshot*>( GAMEIMAGELIST, NULL );
	CreateValue<NrpText>( GAMERETAILER, "" );
	CreateValue<int>( PLATFORMNUMBER, 0 );
	CreateValue<int>( MODULE_NUMBER, 0 );
	CreateValue<int>( USERNUMBER, 0 );
	CreateValue<NrpText>( DESCRIPTIONPATH, "" );
	CreateValue<NrpText>( TEXTURENORMAL, "" );
	CreateValue<float>( READYWORKPERCENT, 1.f );
	CreateValue<NrpText>( VIEWIMAGE, "" );
	_history = NULL;
}

CNrpGame::CNrpGame( CNrpDevelopGame* devGame, CNrpCompany* ptrCompany ) : INrpConfig( CLASS_NRPGAME, devGame->GetString( NAME ) )
{
	InitializeOptions_();

	SetValue<PNrpCompany>( PARENTCOMPANY, ptrCompany );
	SetString( COMPANYNAME, ptrCompany->GetString( NAME ) );
	SetString( NAME, devGame->GetString( NAME ) );
	SetValue<int>( MONEYONDEVELOP, devGame->GetValue<int>( MONEYONDEVELOP ) );
	CNrpGameEngine* ge = devGame->GetValue<CNrpGameEngine*>( GAME_ENGINE );
	if( ge )
		SetString( GAME_ENGINE, ge->GetString( NAME ) );

	SetValue<float>( FAMOUS, devGame->GetValue<float>( FAMOUS ) );
	assert( devGame->GetValue<int>( PLATFORMNUMBER ) != 0 );
	SetValue<int>( PLATFORMNUMBER, devGame->GetValue<int>( PLATFORMNUMBER ) );
	SetValue<int>( USERNUMBER, devGame->GetValue<int>( USERNUMBER ) );
	SetString( PREV_GAME, devGame->GetString( NAME ) ); 
	
	_developers = devGame->GetDevelopers();

	SetValue<int>( GENRE_MODULE_NUMBER, devGame->GetValue<int>( GENRE_MODULE_NUMBER ) );
	for( int cnt=0; cnt < GetValue<int>( GENRE_MODULE_NUMBER ); cnt++ )
	{
		 CNrpTechnology* genre = devGame->GetGenre( cnt );
		 if( genre )
			_genres.push_back( genre->GetString( INTERNAL_NAME ) );
	}

	SetValue<int>( MODULE_NUMBER, devGame->GetValue<int>( MODULE_NUMBER ) );
	for( int cnt=0; cnt < GetValue<int>( MODULE_NUMBER ); cnt++ )
		 _techs.push_back( devGame->GetModule( cnt )->GetString( INTERNAL_NAME ) );

	SetString( INTERNAL_NAME, CNrpApplication::Instance().GetFreeInternalName( this ) );
	CNrpScreenshot* pgList = CNrpApplication::Instance().GetScreenshot( GetString( INTERNAL_NAME ) );
	assert( pgList != NULL );
	if( pgList != NULL )
		SetValue<CNrpScreenshot*>( GAMEIMAGELIST, pgList );
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
	
	NrpText localFolder = OpFileSystem::CheckEndSlash( OpFileSystem::CheckEndSlash( saveFolder ) + GetString( NAME ) );
	NrpText saveFile = localFolder + "item.game";

	OpFileSystem::CreateDirectory( localFolder );
	INrpConfig::Save( saveFile );

	if( PNrpGameBox box = GetValue<PNrpGameBox>( GBOX ) )
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
	for( int i=0; i < GetValue<int>( MODULE_NUMBER ); ++i )
		_techs.push_back( rv.Get( SECTION_TECHS, CreateKeyTech(i), NrpText("") ) );

	for( int i=0; i < GetValue<int>( GENRE_MODULE_NUMBER ); ++i )
		_genres.push_back( rv.Get( SECTION_GENRES, CreateKeyGenre(i), NrpText("") ) );

	NrpText boxIni = loadFolder + "box.ini";
	if( OpFileSystem::IsExist( boxIni ) )
	{
		PNrpGameBox box = new CNrpGameBox( this );
		box->Load( boxIni );
		SetValue<PNrpGameBox>( GBOX, box );
	}

	CNrpScreenshot* pgList = CNrpApplication::Instance().GetScreenshot( GetString( INTERNAL_NAME ) );
	assert( pgList != NULL );
	if( pgList != NULL )
		SetValue<CNrpScreenshot*>( GAMEIMAGELIST, pgList );
}

float CNrpGame::GetAuthorFamous()
{
	float summ = 0.1f;
	for( u32 i=0; i < _developers.size(); i++ )
	{
		IUser* user = CNrpApplication::Instance().GetUser( _developers[ i ] );
		if( user )
		{
			summ += user->GetValue<float>( FAMOUS );
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
	PNrpGameBox gameBox = GetValue<PNrpGameBox>( GBOX );
	Log(HW) << "gameBox == NULL " << GetString( INTERNAL_NAME ) << term;

	if( gameBox != NULL )
	{
		int boxNumber = GetValue<PNrpGameBox>( GBOX )->GetValue<int>( BOXNUMBER );
		number = number > boxNumber ? boxNumber : number;
		GetValue<PNrpGameBox>( GBOX )->AddValue<int>( BOXNUMBER, -number );
		int price = GetValue<PNrpGameBox>( GBOX )->GetValue<int>( PRICE );

		AddValue<int>( CASH, price * number );
		AddValue<int>( COPYSELL, number );

		if( GetValue<PNrpCompany>( PARENTCOMPANY ) )
			GetValue<PNrpCompany>( PARENTCOMPANY )->AddValue<int>( BALANCE, price * number );

		SYSTEMTIME curTime = CNrpApplication::Instance().GetValue<SYSTEMTIME>( CURRENTTIME );
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