#include "StdAfx.h"
#include "NrpRetailer.h"
#include "NrpTime.h"

namespace nrp
{
CLASS_NAME CLASS_NRPRETAILER( "CNrpRetailer" );

CNrpRetailer::CNrpRetailer(const NrpText& name) : INrpProject( CLASS_NRPRETAILER, "" )
{
	Add<NrpText>( NAME, name );
	Add<float>( FAMOUS, 0 );
	Add( STARTDATE, NrpTime( 0. ) );
	Add( ENDDATE, NrpTime( 0. ) );
}

CNrpRetailer::~CNrpRetailer(void)
{
}

NrpText CNrpRetailer::ClassName()
{
	return CLASS_NRPRETAILER;
}

}//namespace nrp