#include "StdAfx.h"
#include "INrpProject.h"

namespace nrp
{
	
INrpProject::INrpProject( const std::string& className, const std::string& systemName ) : INrpConfig( className, systemName )
{
	CreateValue<std::string>( NAME, "defaultName" );
	CreateValue<int>( BALANCE, 0 );
	CreateValue<PROJECT_TYPE>( TECHTYPE, PT_UNKNOWN );
}

INrpProject::~INrpProject(void)
{
}

std::string INrpProject::Save( const std::string& fileName )
{
	return INrpConfig::Save( fileName );
}

}//namespace nrp