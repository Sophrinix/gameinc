#include "StdAfx.h"
#include "NrpLicense.h"

namespace nrp
{
CLASS_NAME CLASS_LICENSE( "CNrpLicense" );

CNrpLicense::CNrpLicense( const NrpText& saveFile ) : INrpProject( CLASS_LICENSE, "" )
{
	RegProperty<NrpText>( NAME, "" );

	Load( saveFile );
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