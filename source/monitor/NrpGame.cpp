#include "StdAfx.h"
#include "NrpGame.h"
#include "NrpCompany.h"

#include <io.h>
#include <errno.h>

namespace nrp
{
	
CNrpGame::CNrpGame( std::string name ) : INrpConfig( "CNrpGame", "" )
{
	CreateValue<std::string>( COMPANYNAME, "" );
	CreateValue<SYSTEMTIME>( STARTDATE, SYSTEMTIME() );
	CreateValue<SYSTEMTIME>( ENDDATE, SYSTEMTIME() );
	CreateValue<int>( MONEYONDEVELOP, 0 );
	CreateValue<int>( CASH, 0 );
	CreateValue<int>( COPYSELL, 0 );
	CreateValue<int>( STARTRATING, 0 );
	CreateValue<int>( CURRENTRATING, 0 );
	CreateValue<std::string>( NAME, name );
}

CNrpGame::~CNrpGame(void)
{
}

void CNrpGame::Save( std::string saveFolder )
{
	std::string localFolder = saveFolder + GetValue<std::string>( NAME ) + "/";
	if( _access( localFolder.c_str(), 0 ) == -1 )
		CreateDirectory( localFolder.c_str(), NULL );

	std::string saveFile = localFolder + "game.ini";
	INrpConfig::Save( PROPERTIES, saveFile );
}

void CNrpGame::Load( std::string loadFolder )
{
	std::string loadFile = loadFolder + "game.ini";
	INrpConfig::Load( PROPERTIES, loadFile );

	SetValue<SYSTEMTIME>( STARTDATE, IniFile::Read( PROPERTIES, STARTDATE, SYSTEMTIME(), loadFile ) );
	SetValue<SYSTEMTIME>( ENDDATE, IniFile::Read( PROPERTIES, ENDDATE, SYSTEMTIME(), loadFile ) );
}
}//namespace nrp