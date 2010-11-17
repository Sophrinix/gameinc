#include "StdAfx.h"
#include "NrpDiskMachine.h"

namespace nrp
{
CLASS_NAME CLASS_DISKMACHINE( "CNrpDiskMachine" );

CNrpDiskMachine::CNrpDiskMachine(void) : INrpConfig( CLASS_DISKMACHINE, "" )
{
	Push<NrpText>( NAME, "" );
	Push<SYSTEMTIME>( STARTDATE, SYSTEMTIME() );	
	Push<SYSTEMTIME>( ENDDATE, SYSTEMTIME() );
	Push<int>( DISKPERHOUR, 0 );
	Push<int>( PRICEPERHOUR, 0 );
	Push<int>( RENTPRICE, 0 );
	Push<NrpText>( TEXTURENORMAL, "" );
	Push<float>( DISCOUNT, 0.f );
	Push<float>( MAXDISCOUNT, 0.f );
	Push<float>( REJECT, 0.f );
	Push<float>( LINEDISCOUNT, 0.f );
}

CNrpDiskMachine::~CNrpDiskMachine(void)
{
}

void CNrpDiskMachine::Load( const NrpText& fileName )
{
	INrpConfig::Load( fileName );
}

void CNrpDiskMachine::AddProducedDisk( const NrpText& companyName, int valuel )
{
	NrpText valueName = DISKPRODUCED + companyName;
	if( !IsExist( valueName ) )
		Push<int>( valueName, valuel );
	else
		AddValue<int>( valueName, valuel );
}

NrpText CNrpDiskMachine::ClassName()
{
	return CLASS_DISKMACHINE;
}

}//end namespace nrp