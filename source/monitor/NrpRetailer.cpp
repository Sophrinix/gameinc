#include "StdAfx.h"
#include "NrpRetailer.h"
#include "NrpTime.h"

namespace nrp
{
CLASS_NAME CLASS_NRPRETAILER( "CNrpRetailer" );

CNrpRetailer::CNrpRetailer(const NrpText& name) : INrpProject( CLASS_NRPRETAILER, "" )
{
	RegProperty<NrpText>( NAME, name );
	RegProperty<float>( FAMOUS, 0 );
	RegProperty( STARTDATE, NrpTime( 0. ) );
	RegProperty( ENDDATE, NrpTime( 0. ) );
}

CNrpRetailer::~CNrpRetailer(void)
{
}

NrpText CNrpRetailer::ClassName()
{
	return CLASS_NRPRETAILER;
}

}//namespace nrp