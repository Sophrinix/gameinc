#include "StdAfx.h"
#include "NrpRetailer.h"

namespace nrp
{
CLASS_NAME CLASS_NRPRETAILER( "CNrpRetailer" );

CNrpRetailer::CNrpRetailer(const NrpText& name) : INrpProject( CLASS_NRPRETAILER, "" )
{
	CreateValue<NrpText>( NAME, name );
	CreateValue<float>( FAMOUS, 0 );
	CreateValue<SYSTEMTIME>( STARTDATE, SYSTEMTIME() );
	CreateValue<SYSTEMTIME>( ENDDATE, SYSTEMTIME() );
}

CNrpRetailer::~CNrpRetailer(void)
{
}

NrpText CNrpRetailer::ClassName()
{
	return CLASS_NRPRETAILER;
}

}//namespace nrp