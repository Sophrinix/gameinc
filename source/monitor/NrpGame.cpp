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
#include "OpFileSystem.h"

#include <errno.h>
#include <OleAuto.h>

namespace nrp
{
	
void CNrpGame::InitializeOptions_()
{
	CreateValue<PNrpCompany>( PARENTCOMPANY, NULL );
	CreateValue<std::string>( NAME, "" );
	CreateValue<std::string>( INTERNAL_NAME, "" );
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
	CreateValue<std::string>( GAME_ENGINE, "" );
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

CNrpGame::CNrpGame( CNrpDevelopGame* devGame, CNrpCompany* ptrCompany ) : INrpConfig( CLASS_NRPGAME, devGame->GetString( NAME ) )
{
	InitializeOptions_();

	SetValue<PNrpCompany>( PARENTCOMPANY, ptrCompany );
	SetString( NAME, devGame->GetString( NAME ) );
	SetValue<int>( MONEYONDEVELOP, devGame->GetValue<int>( MONEYONDEVELOP ) );
	INrpProject* ge = devGame->GetValue<INrpProject*>( GAME_ENGINE );
	if( ge )
		SetString( GAME_ENGINE, ge->GetString( NAME ) );

	SetValue<float>( FAMOUS, devGame->GetValue<float>( FAMOUS ) );
	assert( devGame->GetValue<int>( PLATFORMNUMBER ) != 0 );
	SetValue<int>( PLATFORMNUMBER, devGame->GetValue<int>( PLATFORMNUMBER ) );
	SetValue<int>( USERNUMBER, devGame->GetValue<int>( USERNUMBER ) );
	SetString( PREV_GAME, devGame->GetString( NAME ) ); 
	
	developers_ = devGame->GetDevelopers();

	SetValue<int>( GENRE_MODULE_NUMBER, devGame->GetValue<int>( GENRE_MODULE_NUMBER ) );
	for( int cnt=0; cnt < GetValue<int>( GENRE_MODULE_NUMBER ); cnt++ )
		 genres_.push_back( devGame->GetGenre( cnt )->GetValue<std::string>( NAME ) );

	SetValue<int>( MODULE_NUMBER, devGame->GetValue<int>( MODULE_NUMBER ) );
	for( int cnt=0; cnt < GetValue<int>( MODULE_NUMBER ); cnt++ )
		 techs_.push_back( devGame->GetModule( cnt )->GetString( NAME ) );

	SetString( IMAGENAME, CNrpApplication::Instance().GetFreeInternalName( this ) );
	CNrpGameImageList* pgList = CNrpApplication::Instance().GetGameImageList( GetString( IMAGENAME ) );
	assert( pgList != NULL );
	if( pgList != NULL )
		SetValue<CNrpGameImageList*>( GAMEIMAGELIST, new CNrpGameImageList( *pgList ) );
}

CNrpGame::CNrpGame( const std::string& fileName ) : INrpConfig( CLASS_NRPGAME, fileName )
{
	InitializeOptions_();
	Load( fileName );
}

CNrpGame::~CNrpGame(void)
{
}

std::string CNrpGame::Save( const std::string& saveFolder )
{
	bool upDirExist = OpFileSystem::IsExist( saveFolder );
	assert( upDirExist );

	if( !upDirExist )
		return "";
	
	std::string localFolder = OpFileSystem::CheckEndSlash( OpFileSystem::CheckEndSlash( saveFolder ) + GetString( NAME ) );
	std::string saveFile = localFolder + "item.game";

	OpFileSystem::CreateDirectory( localFolder );
	INrpConfig::Save( saveFile );

	if( PNrpGameBox box = GetValue<PNrpGameBox>( GBOX ) )
	{
		box->Save( localFolder + "box.ini" );
	}

	if( CNrpGameImageList* pgList = GetValue<CNrpGameImageList*>( GAMEIMAGELIST ) )
		pgList->Save( localFolder+"imageList.ini" );

	int i=0;
	for( STRINGS::iterator gIter = techs_.begin(); 
		 gIter != techs_.end(); 
		 gIter++, i++ )
		IniFile::Write( SECTION_TECHS, KEY_TECH(i), *gIter, saveFile );

	i=0;
	for( STRINGS::iterator gIter = genres_.begin(); 
		gIter != genres_.end(); 
		gIter++, i++ )
		IniFile::Write( SECTION_GENRES, KEY_GENRE(i), *gIter, saveFile );

	return localFolder;
}

void CNrpGame::Load( const std::string& loadPath )
{
	bool mayLoad = OpFileSystem::IsExist( loadPath );
	assert( mayLoad );

	if( !mayLoad )
		return;

	std::string loadFolder, loadFile ;
	if( OpFileSystem::IsFolder( loadPath ) )
	{
		loadFolder = loadPath; 
		//стандартное имя
		loadFile = OpFileSystem::CheckEndSlash( loadFolder ) + "item.game";
	}
	else 
	{
		assert( OpFileSystem::GetExtension( loadPath ) == ".game" );
		if( OpFileSystem::GetExtension( loadPath ) == ".game" )
		{
			loadFile = loadPath;
			loadFolder = OpFileSystem::UpDir( loadPath );
		}
		else
			return;
	}

	INrpConfig::Load( loadFile );

	for( int i=0; i < GetValue<int>( MODULE_NUMBER ); ++i )
		techs_.push_back( IniFile::Read( SECTION_TECHS, KEY_TECH(i), std::string(""), loadFile ) );

	for( int i=0; i < GetValue<int>( GENRE_MODULE_NUMBER ); ++i )
		genres_.push_back( IniFile::Read( SECTION_GENRES, KEY_GENRE(i), std::string(""), loadFile ) );

	std::string boxIni = loadFolder + "box.ini";
	if( OpFileSystem::IsExist( boxIni ) )
	{
		PNrpGameBox box = new CNrpGameBox( this );
		box->Load( boxIni );
		SetValue<PNrpGameBox>( GBOX, box );
	}

	std::string imageListIni = loadFolder + "imageList.ini";
	if( OpFileSystem::IsExist( imageListIni ) )
	{
		CNrpGameImageList* pList = new CNrpGameImageList( imageListIni, true );
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
			summ += user->GetValue<float>( FAMOUS );
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

	//history_->AddStep( CURRENTTIME, number, price * number );
}

}//namespace nrp