#include "StdAfx.h"
#include "INrpProject.h"

namespace nrp
{
	
INrpProject::INrpProject( std::string className, std::string systemName ) : INrpConfig( className, systemName )
{
	options_[ NAME ] = new std::string("defaultName");
	options_[ BALANCE ] = new int( 0 );
	options_[ TYPE ] = new PROJECT_TYPE( PT_UNKNOWN );
}

INrpProject::~INrpProject(void)
{
}

}//namespace nrp