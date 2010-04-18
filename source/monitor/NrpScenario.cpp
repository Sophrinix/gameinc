#include "StdAfx.h"
#include "NrpScenario.h"

namespace nrp
{

CNrpScenario::CNrpScenario( std::string name ) : INrpProject( "CNrpScenario", "" )
{
	CreateValue<std::string>( NAME, name );
}

CNrpScenario::~CNrpScenario(void)
{
}

}//namespace nrp