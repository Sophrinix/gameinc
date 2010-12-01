#include "StdAfx.h"
#include "NrpRetailer.h"

namespace nrp
{
CLASS_NAME CLASS_NRPRETAILER( "CNrpRetailer" );

CNrpRetailer::CNrpRetailer(const NrpText& name) : INrpProject( CLASS_NRPRETAILER, "" )
{
	Add<NrpText>( NAME, name );
	Add<float>( FAMOUS, 0 );
	Add<SYSTEMTIME>( STARTDATE, SYSTEMTIME() );
	Add<SYSTEMTIME>( ENDDATE, SYSTEMTIME() );
}

CNrpRetailer::~CNrpRetailer(void)
{
}

NrpText CNrpRetailer::ClassName()
{
	return CLASS_NRPRETAILER;
}

}//namespace nrp