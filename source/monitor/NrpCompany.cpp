#include "StdAfx.h"
#include "NrpCompany.h"
#include "IUser.h"
#include "NrpGameProject.h"

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
	return index < (int)engines_.size() ? engines_[ index ] : NULL;
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
	int tSize = (int)technologies_.size();
	if( index >= tSize )
		return NULL;

	TECH_MAP::iterator tech = technologies_.begin();
	for( int k=0; k < index; index++ ) tech++;

	return (*tech).second;
}

CNrpGameProject* CNrpCompany::AddGameProject( CNrpGameProject* ptrProject )
{
	if( projects_.find( ptrProject->GetOption<std::string>( NAME ) ) == projects_.end() )
	{
		projects_[ ptrProject->GetOption<std::string>( NAME ) ] = ptrProject;
		return ptrProject;
	}

	return NULL;
}

}//namespace nrp