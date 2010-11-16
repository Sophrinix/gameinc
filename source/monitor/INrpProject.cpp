#include "StdAfx.h"
#include "INrpProject.h"

namespace nrp
{
	
INrpProject::INrpProject( const NrpText& className, const NrpText& systemName ) : INrpConfig( className, systemName )
{
	CreateValue<NrpText>( NAME, systemName );
	CreateValue<NrpText>( INTERNAL_NAME, systemName );
	CreateValue<int>( BALANCE, 0 );
	CreateValue<PROJECT_TYPE>( TECHTYPE, PT_UNKNOWN );
}

INrpProject::~INrpProject(void)
{
}

NrpText INrpProject::Save( const NrpText& fileName )
{
	return INrpConfig::Save( fileName );
}

bool INrpProject::Equale( const NrpText& name )
{
	return (GetString( NAME ) == name) || (GetString( INTERNAL_NAME ) == name);
}

}//namespace nrp