#include "StdAfx.h"
#include "NrpScenario.h"

namespace nrp
{
CLASS_NAME CLASS_SCENARIO( "CNrpScenario" );

CNrpScenario::CNrpScenario( const NrpText& name ) : INrpProject( CLASS_SCENARIO, "" )
{
	Add<NrpText>( NAME, name );
}

CNrpScenario::~CNrpScenario(void)
{
}

NrpText CNrpScenario::Save( const NrpText& pathTo )
{
	return INrpProject::Save( pathTo );
}

void CNrpScenario::Load( const NrpText& pathTo )
{

}

NrpText CNrpScenario::ClassName()
{
	return CLASS_SCENARIO;
}

}//namespace nrp