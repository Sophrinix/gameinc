#include "StdAfx.h"
#include "NrpGame.h"
#include "NrpCompany.h"
#include "NrpGameProject.h"
#include "NrpTechnology.h"
#include "NrpGameBox.h"
#include "NrpGameImageList.h"
#include "NrpApplication.h"
#include "NrpDevelopGame.h"
#include "NrpProjectModule.h"
#include "IUser.h"

#include <io.h>
#include <errno.h>
#include <OleAuto.h>

namespace nrp
{
	
CNrpGame::CNrpGame( std::string name ) : INrpConfig( CLASS_NRPGAME, name )
{
	InitializeOptions_();
	SetValue<std::string>( NAME, name );
}

void CNrpGame::InitializeOptions_()
{
	CreateValue<PNrpCompany>( PARENTCOMPANY, NULL );
	CreateValue<std::string>( NAME, "" );
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
	CreateValue<std::string>(GAME_ENGINE, "" );
	CreateValue<std::string>( PREV_GAME, "" );
	CreateValue<int>( GENRE_MODULE_NUMBER, 0 );
	CreateValue<PNrpGameBox>( GBOX, NULL );
	CreateValue<float>( FAMOUS, 0 );
	CreateValue<bool>( GAMEISSALING, false );
	CreateValue<std::string>( IMAGENAME, "" );
	CreateValue<CNrpGameImageList*>( GAMEIMAGELIST, NULL );
	CreateValue<std::string>( GAMERETAILER, "" );
	CreateValue<int>( PLATFORMNUMBER, 0 );
	CreateValue<int>( MODULE_NUMBER, 0 );
	CreateValue<int>( USERNUMBER, 0 );
}

CNrpGame::CNrpGame( CNrpDevelopGame* devGame, CNrpCompany* ptrCompany )
		 : INrpConfig( "CNrpGame", devGame->GetValue<std::string>( NAME ) )
{
	InitializeOptions_();

	SetValue<PNrpCompany>( PARENTCOMPANY, ptrCompany );
	SetValue<std::string>( NAME, devGame->GetValue<std::string>( NAME ) );
	SetValue<int>( MONEYONDEVELOP, devGame->GetValue<int>( MONEYONDEVELOP ) );
	INrpProject* ge = devGame->GetValue<INrpProject*>( GAME_ENGINE );
	if( ge )
		SetValue<std::string>( GAME_ENGINE, ge->GetValue<std::string>( NAME ) );
	SetValue<float>( FAMOUS, devGame->GetValue<float>( FAMOUS ) );
	assert( devGame->GetValue<int>( PLATFORMNUMBER ) != 0 );
	SetValue<int>( PLATFORMNUMBER, devGame->GetValue<int>( PLATFORMNUMBER ) );
	SetValue<int>( USERNUMBER, devGame->GetValue<int>( USERNUMBER ) );
	SetValue<std::string>( PREV_GAME, devGame->GetValue<std::string>( NAME ) ); 
	
	developers_ = devGame->GetDevelopers();

	SetValue<int>( GENRE_MODULE_NUMBER, devGame->GetValue<int>( GENRE_MODULE_NUMBER ) );
	for( int cnt=0; cnt < GetValue<int>( GENRE_MODULE_NUMBER ); cnt++ )
		 genres_.push_back( devGame->GetGenre( cnt )->GetValue<std::string>( NAME ) );

	SetValue<int>( MODULE_NUMBER, devGame->GetValue<int>( MODULE_NUMBER ) );
	for( int cnt=0; cnt < GetValue<int>( MODULE_NUMBER ); cnt++ )
		 techs_.push_back( devGame->GetModule( cnt )->GetValue<std::string>( NAME ) );

	SetValue<std::string>( IMAGENAME, CNrpApplication::Instance().GetFreeInternalName( this ) );
	CNrpGameImageList* pgList = CNrpApplication::Instance().GetGameImageList( GetValue<std::string>( IMAGENAME ) );
	assert( pgList != NULL );
	if( pgList != NULL )
		SetValue<CNrpGameImageList*>( GAMEIMAGELIST, new CNrpGameImageList( *pgList ) );
}

CNrpGame::~CNrpGame(void)
{
}

void CNrpGame::Save( std::string saveFolder )
{
	if( saveFolder[ saveFolder.length() - 1 ] != '/' && 
		saveFolder[ saveFolder.length() - 1 ] != '\\' )
		saveFolder += "/";

	if( _access( saveFolder.c_str(), 0 ) == -1 )
		CreateDirectory( saveFolder.c_str(), NULL );

	std::string localFolder = saveFolder + GetValue<std::string>( NAME ) + "/";
	if( _access( localFolder.c_str(), 0 ) == -1 )
		CreateDirectory( localFolder.c_str(), NULL );

	std::string saveFile = localFolder + "game.ini";
	INrpConfig::Save( PROPERTIES, saveFile );

	PNrpGameBox box = GetValue<PNrpGameBox>( GBOX );
	if( box )
		box->Save( PROPERTIES, localFolder + "box.ini" );

	CNrpGameImageList* pgList = GetValue<CNrpGameImageList*>( GAMEIMAGELIST );
	if( pgList )
		pgList->Save( localFolder+"imageList.ini" );

	int i=0;
	for( STRINGS::iterator gIter = techs_.begin(); 
		 gIter != techs_.end(); 
		 gIter++, i++ )
		IniFile::Write( "techs", "tech" + IntToStr(i), *gIter, saveFile );

	i=0;
	for( STRINGS::iterator gIter = genres_.begin(); 
		gIter != genres_.end(); 
		gIter++, i++ )
		IniFile::Write( "genres", "genre" + IntToStr(i), *gIter, saveFile );
}

void CNrpGame::Load( std::string loadFolder )
{
	assert( !loadFolder.empty() );

	if( loadFolder.empty() )
	{
		throw ("пустая директория для загрузки игры" + GetValue<std::string>( NAME )).c_str();	
		return; 
	}

	if( loadFolder[ loadFolder.length() -1 ] != '/' && 
		loadFolder[ loadFolder.length() -1 ] != '\\' )
		loadFolder += "/";

	std::string loadFile = loadFolder + "game.ini";

	INrpConfig::Load( PROPERTIES, loadFile );

	SetValue<SYSTEMTIME>( STARTDATE, IniFile::Read( PROPERTIES, STARTDATE, SYSTEMTIME(), loadFile ) );
	SetValue<SYSTEMTIME>( ENDDATE, IniFile::Read( PROPERTIES, ENDDATE, SYSTEMTIME(), loadFile ) );

	for( int i=0; i < GetValue<int>( MODULE_NUMBER ); ++i )
		techs_.push_back( IniFile::Read( "techs", "tech" + IntToStr(i), std::string(""), loadFile ) );

	for( int i=0; i < GetValue<int>( GENRE_MODULE_NUMBER ); ++i )
		genres_.push_back( IniFile::Read( "genres", "genre" + IntToStr(i), std::string(""), loadFile ) );

	std::string boxIni = loadFolder + "box.ini";
	if( _access( boxIni.c_str(), 0 ) == 0 )
	{
		PNrpGameBox box = new CNrpGameBox( this );
		box->Load( PROPERTIES, boxIni );
		SetValue<PNrpGameBox>( GBOX, box );
	}

	std::string imageListIni = loadFolder + "imageList.ini";
	if( _access( imageListIni.c_str(), 0 ) == 0 )
	{
		CNrpGameImageList* pList = new CNrpGameImageList( "" );
		pList->Load( imageListIni );
		SetValue<CNrpGameImageList*>( GAMEIMAGELIST, pList );
	}
}

float CNrpGame::GetAuthorFamous()
{
	float summ = 0.1f;
	DEVELOPERS_LIST::iterator uIter = developers_.begin();
	for( ; uIter != developers_.end(); uIter++ )
	{
		IUser* user = CNrpApplication::Instance().GetUser( *uIter );
		if( user )
		{
			summ += user->GetValueA<float>( FAMOUS );
			summ /= 2.f;
		}
	}
	return summ;
}

std::string CNrpGame::GetGenreName( size_t index )
{
	return index < genres_.size() ? genres_[ index ] : "";
}

std::string CNrpGame::GetTechName( size_t index )
{
    return index < techs_.size() ? techs_[ index ] : "";
}

void CNrpGame::GameBoxSaling( int number )
{
	assert( GetValue<PNrpGameBox>( GBOX ) != NULL );
	int boxNumber = GetValue<PNrpGameBox>( GBOX )->GetValue<int>( BOXNUMBER );
	number = number > boxNumber ? boxNumber : number;
	GetValue<PNrpGameBox>( GBOX )->AddValue<int>( BOXNUMBER, -number );
	int price = GetValue<PNrpGameBox>( GBOX )->GetValue<int>( PRICE );

	AddValue<int>( CASH, price * number );
	AddValue<int>( COPYSELL, number );

	if( GetValue<PNrpCompany>( PARENTCOMPANY ) )
		GetValue<PNrpCompany>( PARENTCOMPANY )->AddValue<int>( BALANCE, price * number );

	SYSTEMTIME curTime = CNrpApplication::Instance().GetValue<SYSTEMTIME>( CURRENTTIME );
	curTime.wDay = curTime.wDayOfWeek = curTime.wHour = 0;
	curTime.wMilliseconds = curTime.wMinute = curTime.wSecond = 0;
	double curMonth;
	SystemTimeToVariantTime( &curTime, &curMonth );
	SALE_HISTORY_MAP::iterator pIter = history_.find( (int)curMonth );
	if( pIter == history_.end() )
		history_[ (int)curMonth ] = new CNrpHistoryStep( (int)curMonth, number, price * number );
	else
		history_[ (int)curMonth ]->Add( number, price * number );
}

}//namespace nrp