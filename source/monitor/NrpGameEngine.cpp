#include "StdAfx.h"
#include "NrpGameEngine.h"
#include "NrpCompany.h"
#include "OpFileSystem.h"

#include <errno.h>

namespace nrp
{

CNrpGameEngine::CNrpGameEngine( const std::string& name ) : INrpProject( CLASS_GAMEENGINE, "" )
{
	_InitialyzeOptions();

	SetValue<std::string>( NAME, name );
}

void CNrpGameEngine::_InitialyzeOptions()
{
	CreateValue<int>( AVGENRE_COUNT, 0 );
	CreateValue<std::string>( INTERNAL_NAME, "" ); 
	CreateValue<int>( TIME_ACTUAL, 0 );
	CreateValue<int>( GENRE_MODULE_NUMBER, 0 );
	CreateValue<int>( CODEVOLUME, 0 );
	CreateValue<int>( QUALITY, 0 );
	CreateValue<int>( SKILL_CODING, 0 );
	CreateValue<std::string>( COMPANYNAME, "" );
	CreateValue<PNrpCompany>( PARENTCOMPANY, NULL );
	CreateValue<std::string>( TEXTURENORMAL, "" );
}

CNrpGameEngine::CNrpGameEngine( const std::string& fileName, bool load )
: INrpProject( CLASS_GAMEENGINE, fileName )
{
	_InitialyzeOptions();

	Load( fileName );
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

std::string CNrpGameEngine::Save( const std::string& saveFolder )
{
	assert( OpFileSystem::IsExist( saveFolder ) );
	std::string localFolder = OpFileSystem::CheckEndSlash( saveFolder) + GetValue<std::string>( INTERNAL_NAME );

	OpFileSystem::CreateDirectory( localFolder );
	std::string saveFile = OpFileSystem::CheckEndSlash( localFolder ) + "item.engine";

	assert( !OpFileSystem::IsExist( saveFile ) );
	INrpProject::Save( saveFile );

	GENRE_MAP::iterator pIter = avgenres_.begin();
	for( int i=0; pIter != avgenres_.end(); pIter++, i++ )
		IniFile::Write( "avaibleGenre", IntToStr( pIter->first ), IntToStr( pIter->second ), saveFile );

	return localFolder;
}

void CNrpGameEngine::Load( const std::string& loadFolder )
{
	std::string loadFile = OpFileSystem::CheckEndSlash( loadFolder ) + "item.engine" ;
	INrpProject::Load( loadFile );

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