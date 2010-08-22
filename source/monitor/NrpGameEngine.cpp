#include "StdAfx.h"
#include "NrpGameEngine.h"
#include "NrpCompany.h"

#include <io.h>
#include <errno.h>

namespace nrp
{

CNrpGameEngine::CNrpGameEngine( std::string name ) : INrpProject( "CNrpGameEngine", "" )
{
	SetValue<std::string>( NAME, name );
	CreateValue<int>( AVGENRE_COUNT, 0 );
	CreateValue<int>( TIME_ACTUAL, 0 );
	CreateValue<int>( GENRE_MODULE_NUMBER, 0 );
	CreateValue<int>( CODEVOLUME, 0 );
	CreateValue<int>( QUALITY, 0 );
	CreateValue<int>( SKILL_CODING, 0 );
	CreateValue<std::string>( COMPANYNAME, "" );
	CreateValue<PNrpCompany>( PARENTCOMPANY, NULL );
}

CNrpGameEngine::~CNrpGameEngine(void)
{
}

//! добавить жанр двига... тестовая функция( жанры будут настледоваться от проекта )
void CNrpGameEngine::AddGenre( GENRE_TYPE typen )
{
	if( avgenres_.find( typen ) == avgenres_.end() )
		avgenres_[ typen ] = 1;
	int avsize = avgenres_.size();
	SetValue<int>( AVGENRE_COUNT, avsize );
}

bool CNrpGameEngine::IsGenreAvailble( GENRE_TYPE typen )
{
	return (avgenres_.find( typen ) != avgenres_.end() );
}

nrp::GENRE_TYPE CNrpGameEngine::GetGenreType( int index )
{
	GENRE_MAP::iterator pIter = avgenres_.begin();
	for( int i=0; pIter != avgenres_.end(), i < index; pIter++, i++ );

	return pIter != avgenres_.end() ? pIter->first : GT_UNKNOWN;
}

void CNrpGameEngine::Save( std::string saveFolder )
{
	if( _access( saveFolder.c_str(), 0 ) == -1 )
		CreateDirectory( saveFolder.c_str(), NULL );

	std::string localFolder = saveFolder + GetValue<std::string>( NAME ) + "/";

	if( _access( localFolder.c_str(), 0 ) == -1 )
		CreateDirectory( localFolder.c_str(), NULL );
	std::string saveFile = localFolder + "engine.ini";

	DeleteFile( saveFile.c_str() );
	INrpProject::Save( SECTION_PROPERTIES, saveFile );

	GENRE_MAP::iterator pIter = avgenres_.begin();
	for( int i=0; pIter != avgenres_.end(); pIter++, i++ )
		IniFile::Write( "avaibleGenre", IntToStr( pIter->first ), IntToStr( pIter->second ), saveFile );
}

void CNrpGameEngine::Load( std::string loadFolder )
{
	std::string loadFile = loadFolder + "engine.ini";
	INrpProject::Load( SECTION_PROPERTIES, loadFile );

	char buffer[ 32000 ];
	memset( buffer, 0, 32000 );
	GetPrivateProfileSection( "avaibleGenre", buffer, 32000, loadFile.c_str() );

	std::string readLine = buffer;
	while( readLine != "" )
	{
		std::string name, valuel;
		name = readLine.substr( 0, readLine.find( '=' ) );
		valuel = readLine.substr( readLine.find( '=' ) + 1, 0xff );
		avgenres_[ GENRE_TYPE( StrToInt( name.c_str() ) ) ] = StrToInt( valuel.c_str() );
		memcpy( buffer, buffer + strlen(buffer) + 1, 32000 );  
		readLine = buffer;
	}
}
}//namespace nrp