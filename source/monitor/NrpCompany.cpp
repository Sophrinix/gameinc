#include "StdAfx.h"
#include "NrpCompany.h"
#include "IUser.h"

namespace nrp
{

CNrpCompany::CNrpCompany( const char* name ) : INrpConfig( "CNrpCompany", name)
{
	options_[ BALANCE ] = new int( 100000 );
	options_[ NAME ] = new std::string( name );
	options_[ CEO ] = new PUser( NULL );
	options_[ ENGINES_NUMBER ] = new int( 0 );
	options_[ TECHS_NUMBER ] = new int( 0 );
}

CNrpCompany::~CNrpCompany(void)
{
}

CNrpGameEngine* CNrpCompany::GetGameEngine( int index )
{
	return engines_.at( index );
}

void CNrpCompany::AddGameEngine( CNrpGameEngine* ptrEng )
{
	if( ptrEng != NULL )
		engines_.push_back( ptrEng );

	int k = engines_.size();
	SetOption<int>( ENGINES_NUMBER, k );
}

CNrpTechnology* CNrpCompany::GetTech( int index )
{
	size_t tSize = technologies_.size();
	if( index >= tSize )
		return NULL;

	TECH_MAP::iterator tech = technologies_.begin();
	for( int k=0; k < index; index++ ) tech++;

	return (*tech).second;
}

}//namespace nrp