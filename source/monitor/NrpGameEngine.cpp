#include "StdAfx.h"
#include "NrpGameEngine.h"
#include "NrpCompany.h"
#include "OpFileSystem.h"
#include "IniFile.h"

#include <errno.h>

namespace nrp
{
CLASS_NAME CLASS_GAMEENGINE( "CNrpGameEngine" );

CNrpGameEngine::CNrpGameEngine( const NrpText& name ) : INrpProject( CLASS_GAMEENGINE, "" )
{
	_InitialyzeOptions();

	SetValue<NrpText>( NAME, name );
}

void CNrpGameEngine::_InitialyzeOptions()
{
	CreateValue<int>( AVGENRE_COUNT, 0 );
	CreateValue<NrpText>( INTERNAL_NAME, "" ); 
	CreateValue<int>( TIME_ACTUAL, 0 );
	CreateValue<int>( GENRE_MODULE_NUMBER, 0 );
	CreateValue<int>( CODEVOLUME, 0 );
	CreateValue<int>( QUALITY, 0 );
	CreateValue<int>( SKILL_CODING, 0 );
	CreateValue<NrpText>( COMPANYNAME, "" );
	CreateValue<PNrpCompany>( PARENTCOMPANY, NULL );
	CreateValue<NrpText>( TEXTURENORMAL, "" );
	CreateValue<SYSTEMTIME>( STARTDATE, SYSTEMTIME() );
}

CNrpGameEngine::CNrpGameEngine( const NrpText& fileName, bool load )
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
	if( _avgenres.find( typen ) == NULL )
		_avgenres[ typen ] = 1;

	SetValue<int>( AVGENRE_COUNT, _avgenres.size() );
}

bool CNrpGameEngine::IsGenreAvailble( GENRE_TYPE typen )
{
	return (_avgenres.find( typen ) != NULL );
}

nrp::GENRE_TYPE CNrpGameEngine::GetGenre( int index )
{
	REQUIRE_MAP::Iterator pIter = _avgenres.getIterator();

	for( int i=0; pIter.atEnd(), i < index; pIter++, i++ );

	return pIter.atEnd() ? GT_UNKNOWN : GENRE_TYPE( pIter->getKey() );
}

NrpText CNrpGameEngine::Save( const NrpText& saveFolder )
{
	assert( OpFileSystem::IsExist( saveFolder ) );
	NrpText localFolder = OpFileSystem::CheckEndSlash( saveFolder) + GetString( INTERNAL_NAME );

	OpFileSystem::CreateDirectory( localFolder );
	NrpText saveFile = OpFileSystem::CheckEndSlash( localFolder ) + "item.engine";

	assert( !OpFileSystem::IsExist( saveFile ) );
	INrpProject::Save( saveFile );

	IniFile sv( saveFile );
	sv.Set( SECTION_GENRES, _avgenres );

	return localFolder;
}

void CNrpGameEngine::Load( const NrpText& loadFolder )
{
	NrpText loadFile = OpFileSystem::CheckEndSlash( loadFolder ) + "item.engine" ;
	INrpProject::Load( loadFile );

	IniFile rv( loadFile );

	rv.Get( SECTION_GENRES, _avgenres );
}

NrpText CNrpGameEngine::ClassName()
{
	return CLASS_GAMEENGINE;
}

}//namespace nrp