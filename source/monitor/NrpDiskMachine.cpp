#include "StdAfx.h"
#include "NrpDiskMachine.h"

namespace nrp
{

CNrpDiskMachine::CNrpDiskMachine(void) : INrpConfig( CLASS_DISKMACHINE, "" )
{
	CreateValue<std::string>( NAME, "" );
	CreateValue<SYSTEMTIME>( STARTDATE, SYSTEMTIME() );	
	CreateValue<SYSTEMTIME>( ENDDATE, SYSTEMTIME() );
	CreateValue<int>( DISKPERHOUR, 0 );
	CreateValue<int>( PRICEPERHOUR, 0 );
	CreateValue<int>( RENTPRICE, 0 );
	CreateValue<std::string>( TEXTURENORMAL, "" );
}

CNrpDiskMachine::~CNrpDiskMachine(void)
{
}

void CNrpDiskMachine::Load( std::string sectionName, std::string fileName )
{
	INrpConfig::Load( fileName );
}
}//end namespace nrp