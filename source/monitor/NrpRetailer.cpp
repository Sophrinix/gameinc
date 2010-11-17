#include "StdAfx.h"
#include "NrpRetailer.h"

namespace nrp
{
CLASS_NAME CLASS_NRPRETAILER( "CNrpRetailer" );

CNrpRetailer::CNrpRetailer(const NrpText& name) : INrpProject( CLASS_NRPRETAILER, "" )
{
	Push<NrpText>( NAME, name );
	Push<float>( FAMOUS, 0 );
	Push<SYSTEMTIME>( STARTDATE, SYSTEMTIME() );
	Push<SYSTEMTIME>( ENDDATE, SYSTEMTIME() );
}

CNrpRetailer::~CNrpRetailer(void)
{
}

NrpText CNrpRetailer::ClassName()
{
	return CLASS_NRPRETAILER;
}

}//namespace nrp