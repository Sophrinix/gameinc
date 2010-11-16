#include "StdAfx.h"
#include "NrpLicense.h"

namespace nrp
{
CLASS_NAME CLASS_LICENSE( "CNrpLicense" );

CNrpLicense::CNrpLicense( const NrpText& name) : INrpProject( CLASS_LICENSE, "" )
{
	CreateValue<NrpText>( NAME, name );
}

CNrpLicense::~CNrpLicense(void)
{
}

void CNrpLicense::Load( const NrpText& pathTo )
{

}

NrpText CNrpLicense::Save( const NrpText& pathTo )
{
	return INrpProject::Save( pathTo );
}

NrpText CNrpLicense::ClassName()
{
	return CLASS_LICENSE;
}

}//namespace nrp