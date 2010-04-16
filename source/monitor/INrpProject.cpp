#include "StdAfx.h"
#include "INrpProject.h"

namespace nrp
{
	
INrpProject::INrpProject( std::string className, std::string systemName ) : INrpConfig( className, systemName )
{
	CreateValue<std::string>( NAME, "defaultName" );
	CreateValue<int>( BALANCE, 0 );
	CreateValue<PROJECT_TYPE>( TYPE, PT_UNKNOWN );
}

INrpProject::~INrpProject(void)
{
}

}//namespace nrp