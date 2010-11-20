#include "StdAfx.h"
#include "INrpProject.h"

namespace nrp
{
	
INrpProject::INrpProject( const NrpText& className, const NrpText& systemName ) : INrpConfig( className, systemName )
{
	Push( NAME, systemName );
	Push( INTERNAL_NAME, systemName );
	Push( BALANCE, (int)0 );
	Push( TECHTYPE, PT_UNKNOWN );
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
	return ( Param( NAME ) == name || Param( INTERNAL_NAME ) == name);
}

}//namespace nrp