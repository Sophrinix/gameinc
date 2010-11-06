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
	CreateValue<float>( DISCOUNT, 0.f );
	CreateValue<float>( MAXDISCOUNT, 0.f );
	CreateValue<float>( REJECT, 0.f );
	CreateValue<float>( LINEDISCOUNT, 0.f );
}

CNrpDiskMachine::~CNrpDiskMachine(void)
{
}

void CNrpDiskMachine::Load( const std::string& fileName )
{
	INrpConfig::Load( fileName );
}

void CNrpDiskMachine::AddProducedDisk( const std::string& companyName, int valuel )
{
	std::string valueName = DISKPRODUCED + companyName;
	if( !IsValueExist( valueName ) )
		CreateValue<int>( valueName, valuel );
	else
		AddValue<int>( valueName, valuel );
}
}//end namespace nrp