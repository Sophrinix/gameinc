#include "StdAfx.h"
#include "NrpGameEngine.h"

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

}
}//namespace nrp