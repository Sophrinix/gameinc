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

std::string CNrpScenario::Save( const std::string& pathTo )
{
	return "";
}

void CNrpScenario::Load( const std::string& pathTo )
{

}

}//namespace nrp