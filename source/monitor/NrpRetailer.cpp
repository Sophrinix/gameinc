#include "StdAfx.h"
#include "NrpRetailer.h"

namespace nrp
{

CNrpRetailer::CNrpRetailer(std::string name) : INrpProject( CLASS_NRPRETAILER, "" )
{
	CreateValue<std::string>( NAME, name );
	CreateValue<float>( FAMOUS, 0 );
	CreateValue<SYSTEMTIME>( STARTDATE, SYSTEMTIME() );
	CreateValue<SYSTEMTIME>( ENDDATE, SYSTEMTIME() );
}

CNrpRetailer::~CNrpRetailer(void)
{
}

}//namespace nrp