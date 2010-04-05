#include "StdAfx.h"
#include "NrpGameEngine.h"

namespace nrp
{

CNrpGameEngine::CNrpGameEngine( std::string name ) : INrpProject( "CNrpGameEngine", "" )
{
	SetOption<std::string>( NAME, name );
	options_[ AVGENRE_COUNT ] = new int( 0 );
	options_[ TIME_ACTUAL ] = new int( 0 );
	options_[ GENRE_MODULE_NUMBER ] = new int( 0 );
	options_[ CODEVOLUME ] = new int( 0 );
	options_[ QUALITY ] = new int( 0 );
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
	SetOption<int>( AVGENRE_COUNT, avsize );
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

}//namespace nrp