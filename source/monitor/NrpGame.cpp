#include "StdAfx.h"
#include "NrpGame.h"
#include "NrpCompany.h"
#include "NrpGameProject.h"
#include "NrpTechnology.h"
#include "NrpGameBox.h"
#include "NrpGameImageList.h"
#include "NrpApplication.h"
#include "NrpDevelopGame.h"
#include "IUser.h"

#include <io.h>
#include <errno.h>

namespace nrp
{
	
CNrpGame::CNrpGame( std::string name ) : INrpConfig( CLASS_NRPGAME, name )
{
	InitializeOptions_();
	SetValue<std::string>( NAME, name );
}

void CNrpGame::InitializeOptions_()
{
	CreateValue<std::string>( COMPANYNAME, "" );
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
	CreateValue<int>( LOCALIZATION, 0 );
	CreateValue<int>( CROSSPLATFORMCODE, 0 );
	CreateValue<std::string>( PREV_GAME, "" );
	CreateValue<std::string>( SCRIPTENGINE, "" );
	CreateValue<std::string>( MINIGAMEENGINE, "" );
	CreateValue<std::string>( PHYSICSENGINE, "" );
	CreateValue<int>( VIDEOTECHNUMBER, 0 );
	CreateValue<std::string>( SOUNDQUALITY, "" );
	CreateValue<int>( SOUNDTECHNUMBER, 0 );
	CreateValue<int>( ADVTECHNUMBER, 0 );
	CreateValue<int>( GENRE_MODULE_NUMBER, 0 );
	CreateValue<PNrpGameBox>( GBOX, NULL );
	CreateValue<float>( FAMOUS, 0 );
	CreateValue<bool>( GAMEISSALING, false );
	CreateValue<std::string>( IMAGENAME, "" );
	CreateValue<CNrpGameImageList*>( GAMEIMAGELIST, NULL );
	CreateValue<std::string>( VIEWIMAGE, "" );
	CreateValue<std::string>( GAMERETAILER, "" );
	CreateValue<int>( PLATFORMNUMBER, 0 );
	CreateValue<int>( USERNUMBER, 0 );
}

CNrpGame::CNrpGame( CNrpDevelopGame* devGame, CNrpCompany* ptrCompany )
		 : INrpConfig( "CNrpGame", devGame->GetValue<std::string>( NAME ) )
{
	InitializeOptions_();

	SetValue<std::string>( COMPANYNAME, devGame->GetValue<std::string>( COMPANYNAME ) );
	SetValue<std::string>( NAME, devGame->GetValue<std::string>( NAME ) );
	SetValue<int>( MONEYONDEVELOP, devGame->GetValue<int>( MONEYONDEVELOP ) );
	SetValue<std::string>( GAME_ENGINE, devGame->GetValue<INrpProject*>( GAME_ENGINE )->GetValue<std::string>( NAME ) );
	SetValue<int>( LOCALIZATION, devGame->GetValue<INrpProject*>( LOCALIZATION )->GetValue<int>( QUALITY ) );
	SetValue<int>( CROSSPLATFORMCODE, devGame->GetValue<INrpProject*>( CROSSPLATFORMCODE )->GetValue<int>( QUALITY ) );
	SetValue<float>( FAMOUS, devGame->GetValue<float>( FAMOUS ) );
	SetValue<int>( PLATFORMNUMBER, devGame->GetValue<int>( PLATFORMNUMBER ) );
	SetValue<int>( USERNUMBER, devGame->GetValue<int>( USERNUMBER ) );
	SetValue<std::string>( PREV_GAME, devGame->GetValue<std::string>( NAME ) ); 
	
	developers_ = devGame->GetDevelopers();

	INrpConfig* component;

	component = devGame->GetValue<PNrpTechnology>( SCRIPTENGINE );
	if( component )	SetValue<std::string>( SCRIPTENGINE, component->GetValue<std::string>( NAME ) );

	component =  devGame->GetValue<PNrpTechnology>( MINIGAMEENGINE );
	if( component ) SetValue<std::string>( MINIGAMEENGINE, component->GetValue<std::string>( NAME ) );

	component =  devGame->GetValue<PNrpTechnology>( PHYSICSENGINE );
	if( component )	SetValue<std::string>( PHYSICSENGINE, component->GetValue<std::string>( NAME ) ); 

	SetValue<int>( VIDEOTECHNUMBER, devGame->GetValue<int>( VIDEOTECHNUMBER ) );
	for( int cnt=0; cnt < GetValue<int>( VIDEOTECHNUMBER ); cnt++ )
		videoTechs_.push_back( devGame->GetVideoTech( cnt )->GetValue<std::string>( NAME ) );

	component = devGame->GetValue<PNrpTechnology>( SOUNDQUALITY );
	if( component ) SetValue<std::string>( SOUNDQUALITY, component->GetValue<std::string>( NAME ) );

	SetValue<int>( SOUNDTECHNUMBER, devGame->GetValue<int>( SOUNDTECHNUMBER ) );
	for( int cnt=0; cnt < GetValue<int>( SOUNDTECHNUMBER ); cnt++ )
		soundTechs_.push_back( devGame->GetSoundTech( cnt )->GetValue<std::string>( NAME ) );
	
	SetValue<int>( ADVTECHNUMBER, devGame->GetValue<int>( ADVTECHNUMBER ) );
	for( int cnt=0; cnt < GetValue<int>( ADVTECHNUMBER ); cnt++ )
		advTechs_.push_back( devGame->GetTechnology(cnt)->GetValue<std::string>( NAME ) );
	
	for( int cnt=0; cnt < devGame->GetValue<int>( GENRE_MODULE_NUMBER ); cnt++ )
	{	
		component = devGame->GetGenre( cnt );
		if( component ) 
		{
			genreTechs_.push_back( component->GetValue<std::string>( NAME ) );
			AddValue<int>( GENRE_MODULE_NUMBER, 1 );
		}
	}

	SetValue<std::string>( IMAGENAME, CNrpApplication::Instance().GetFreeInternalName( this ) );
	CNrpGameImageList* pgList = CNrpApplication::Instance().GetGameImageList( GetValue<std::string>( IMAGENAME ) );
	SetValue<CNrpGameImageList*>( GAMEIMAGELIST, new CNrpGameImageList( *pgList ) );
}

CNrpGame::~CNrpGame(void)
{
}

void CNrpGame::Save( std::string saveFolder )
{
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
		pgList->Save( localFolder+"/imageList.ini" );

	int i=0;
	for( TECH_LIST::iterator tIter = advTechs_.begin(); 
		 tIter != advTechs_.end(); 
		 tIter++, i++ )
		IniFile::Write( ADVTECH, ADVTECH + IntToStr(i), *tIter, saveFile );

	i=0;	
	for( TECH_LIST::iterator gIter = genreTechs_.begin(); 
		 gIter != genreTechs_.end(); 
		 gIter++, i++ )
		IniFile::Write( GENRETECH, GENRETECH + IntToStr(i), *gIter, saveFile );

	i=0;
	for( TECH_LIST::iterator vIter = videoTechs_.begin(); 
		 vIter != videoTechs_.end(); 
		 vIter++, i++ )
		IniFile::Write( VIDEOTECH, VIDEOTECH + IntToStr(i), *vIter, saveFile );

	i=0;
	for( TECH_LIST::iterator sIter = soundTechs_.begin(); 
		 sIter != soundTechs_.end(); 
		 sIter++, i++ )
		IniFile::Write( SOUNDTECH, SOUNDTECH + IntToStr(i), *sIter, saveFile );
}

void CNrpGame::Load( std::string loadFolder )
{
	std::string loadFile = loadFolder + "game.ini";
	INrpConfig::Load( PROPERTIES, loadFile );

	SetValue<SYSTEMTIME>( STARTDATE, IniFile::Read( PROPERTIES, STARTDATE, SYSTEMTIME(), loadFile ) );
	SetValue<SYSTEMTIME>( ENDDATE, IniFile::Read( PROPERTIES, ENDDATE, SYSTEMTIME(), loadFile ) );

	for( int i=0; i < GetValue<int>( ADVTECHNUMBER ); ++i )
		advTechs_.push_back( IniFile::Read( ADVTECH, ADVTECH + IntToStr(i), std::string(""), loadFile ) );

	for( int i=0; i < GetValue<int>( GENRE_MODULE_NUMBER ); ++i )
		genreTechs_.push_back( IniFile::Read( GENRETECH, GENRETECH + IntToStr(i), std::string(""), loadFile ) );

	for( int i=0; i < GetValue<int>( VIDEOTECHNUMBER ); ++i )
		videoTechs_.push_back( IniFile::Read( VIDEOTECH, VIDEOTECH + IntToStr(i), std::string(""), loadFile ) );

	for( int i=0; i < GetValue<int>( SOUNDTECHNUMBER ); ++i )
		soundTechs_.push_back( IniFile::Read( SOUNDTECH, SOUNDTECH + IntToStr(i), std::string(""), loadFile ) );


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

std::string CNrpGame::GetGenreTech( int index )
{
	return index < (int)genreTechs_.size() ? genreTechs_[ index ] : "";
}
}//namespace nrp