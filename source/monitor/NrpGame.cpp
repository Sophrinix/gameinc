#include "StdAfx.h"
#include "NrpGame.h"
#include "NrpCompany.h"
#include "NrpGameProject.h"
#include "NrpGameEngine.h"
#include "NrpTechnology.h"
#include "NrpGameBox.h"

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
	CreateValue<int>( STARTADVFUNC, 0 );
	CreateValue<int>( CURRENTADVFUNC, 0 );
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
}

CNrpGame::CNrpGame( CNrpGameProject* ptrProject, CNrpCompany* ptrCompany )
		 : INrpConfig( "CNrpGame", ptrProject->GetValue<std::string>( NAME ) )
{
	InitializeOptions_();

	SetValue<std::string>( COMPANYNAME, ptrProject->GetValue<std::string>( COMPANYNAME ) );
	SetValue<std::string>( NAME, ptrProject->GetValue<std::string>( NAME ) );
	SetValue<int>( MONEYONDEVELOP, ptrProject->GetValue<int>( MONEYONDEVELOP ) );
	SetValue<std::string>( GAME_ENGINE, ptrProject->GetValue<PNrpGameEngine>( GAME_ENGINE )->GetValue<std::string>( NAME ) );
	SetValue<int>( LOCALIZATION, ptrProject->GetValue<PNrpTechnology>( LOCALIZATION )->GetValue<int>( QUALITY ) );
	SetValue<int>( CROSSPLATFORMCODE, ptrProject->GetValue<PNrpTechnology>( CROSSPLATFORMCODE )->GetValue<int>( QUALITY ) );
	SetValue<float>( FAMOUS, ptrProject->GetValue<float>( FAMOUS ) );

	INrpConfig* component = ptrProject->GetValue<PNrpGame>( PREV_GAME );
	if( component ) SetValue<std::string>( PREV_GAME, component->GetValue<std::string>( NAME ) ); 

	component = ptrProject->GetValue<PNrpTechnology>( SCRIPTENGINE );
	if( component )	SetValue<std::string>( SCRIPTENGINE, component->GetValue<std::string>( NAME ) );

	component =  ptrProject->GetValue<PNrpTechnology>( MINIGAMEENGINE );
	if( component ) SetValue<std::string>( MINIGAMEENGINE, component->GetValue<std::string>( NAME ) );

	component =  ptrProject->GetValue<PNrpTechnology>( PHYSICSENGINE );
	if( component )	SetValue<std::string>( PHYSICSENGINE, component->GetValue<std::string>( NAME ) ); 

	SetValue<int>( VIDEOTECHNUMBER, ptrProject->GetValue<int>( VIDEOTECHNUMBER ) );
	for( int cnt=0; cnt < GetValue<int>( VIDEOTECHNUMBER ); cnt++ )
		videoTechs_.push_back( ptrProject->GetVideoTech( cnt )->GetValue<std::string>( NAME ) );

	component = ptrProject->GetValue<PNrpTechnology>( SOUNDQUALITY );
	if( component ) SetValue<std::string>( SOUNDQUALITY, component->GetValue<std::string>( NAME ) );

	SetValue<int>( SOUNDTECHNUMBER, ptrProject->GetValue<int>( SOUNDTECHNUMBER ) );
	for( int cnt=0; cnt < GetValue<int>( SOUNDTECHNUMBER ); cnt++ )
		soundTechs_.push_back( ptrProject->GetSoundTech( cnt )->GetValue<std::string>( NAME ) );
	
	SetValue<int>( ADVTECHNUMBER, ptrProject->GetValue<int>( ADVTECHNUMBER ) );
	for( int cnt=0; cnt < GetValue<int>( ADVTECHNUMBER ); cnt++ )
		advTechs_.push_back( ptrProject->GetTechnology(cnt)->GetValue<std::string>( NAME ) );
	
	for( int cnt=0; cnt < ptrProject->GetValue<int>( GENRE_MODULE_NUMBER ); cnt++ )
	{	
		component = ptrProject->GetGenre( cnt );
		if( component ) 
		{
			genreTechs_.push_back( component->GetValue<std::string>( NAME ) );
			AddValue<int>( GENRE_MODULE_NUMBER, 1 );
		}
	}
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
}

void CNrpGame::Load( std::string loadFolder )
{
	std::string loadFile = loadFolder + "game.ini";
	INrpConfig::Load( PROPERTIES, loadFile );

	SetValue<SYSTEMTIME>( STARTDATE, IniFile::Read( PROPERTIES, STARTDATE, SYSTEMTIME(), loadFile ) );
	SetValue<SYSTEMTIME>( ENDDATE, IniFile::Read( PROPERTIES, ENDDATE, SYSTEMTIME(), loadFile ) );

	std::string boxIni = loadFolder + "box.ini";
	if( _access( boxIni.c_str(), 0 ) == 0 )
	{
		PNrpGameBox box = new CNrpGameBox( this );
		box->Load( PROPERTIES, boxIni );
		SetValue<PNrpGameBox>( GBOX, box );
	}
}

float CNrpGame::GetAuthorFamous()
{
	return 1;
}
}//namespace nrp