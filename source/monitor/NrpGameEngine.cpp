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

	_self[ NAME ] = name;
}

void CNrpGameEngine::_InitialyzeOptions()
{
	Add<int>( AVGENRE_COUNT, 0 );
	Add<int>( BALANCE, 0 );
	Add<NrpText>( INTERNAL_NAME, "" ); 
	Add<int>( TIME_ACTUAL, 0 );
	Add<int>( GENRE_MODULE_NUMBER, 0 );
	Add<int>( CODEVOLUME, 0 );
	Add<int>( QUALITY, 0 );
	Add<int>( SKILL_CODING, 0 );
	Add<NrpText>( COMPANYNAME, "" );
	Add<PNrpCompany>( PARENTCOMPANY, NULL );
	Add<NrpText>( TEXTURENORMAL, "" );
	Add( STARTDATE, NrpTime( 0. ) );
	Add( CPU, 0.f );
	Add( ADVTECHNUMBER, 0 );

	_self[ TECHGROUP ] = static_cast< int >( PT_GAMEENGINE );
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
void CNrpGameEngine::AddGenre( const NrpText& name )
{
	if( _techs.find( name ) == NULL )
		_techs[ name ] = 1;

	_self[ AVGENRE_COUNT ] = static_cast< int >( _techs.size() );
}

bool CNrpGameEngine::IsMyTech( const NrpText& name )
{
	return (_techs.find( name ) != NULL );
}

const NrpText& CNrpGameEngine::GetGenre( int index )
{
	KNOWLEDGE_MAP::Iterator pIter = _techs.getIterator();

	for( int i=0; pIter.atEnd(), i < index; pIter++, i++ );

	return pIter.atEnd() ? "" : pIter->getKey();
}

NrpText CNrpGameEngine::Save( const NrpText& saveFolder )
{
	assert( OpFileSystem::IsExist( saveFolder ) );
	NrpText localFolder = OpFileSystem::CheckEndSlash( saveFolder) + Text( INTERNAL_NAME );

	OpFileSystem::CreateDirectory( localFolder );
	NrpText saveFile = OpFileSystem::CheckEndSlash( localFolder ) + "item.engine";

	assert( !OpFileSystem::IsExist( saveFile ) );
	INrpProject::Save( saveFile );

	IniFile sv( saveFile );
	sv.Set( SECTION_GENRES, _techs );

	sv.Save();
	return localFolder;
}

void CNrpGameEngine::Load( const NrpText& loadFolder )
{
	NrpText loadFile = OpFileSystem::CheckEndSlash( loadFolder ) + "item.engine" ;
	INrpProject::Load( loadFile );

	IniFile rv( loadFile );

	rv.Get( SECTION_GENRES, _techs );
}

NrpText CNrpGameEngine::ClassName()
{
	return CLASS_GAMEENGINE;
}

}//namespace nrp