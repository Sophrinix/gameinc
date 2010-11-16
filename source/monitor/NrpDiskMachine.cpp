#include "StdAfx.h"
#include "NrpDiskMachine.h"

namespace nrp
{
CLASS_NAME CLASS_DISKMACHINE( "CNrpDiskMachine" );

CNrpDiskMachine::CNrpDiskMachine(void) : INrpConfig( CLASS_DISKMACHINE, "" )
{
	CreateValue<NrpText>( NAME, "" );
	CreateValue<SYSTEMTIME>( STARTDATE, SYSTEMTIME() );	
	CreateValue<SYSTEMTIME>( ENDDATE, SYSTEMTIME() );
	CreateValue<int>( DISKPERHOUR, 0 );
	CreateValue<int>( PRICEPERHOUR, 0 );
	CreateValue<int>( RENTPRICE, 0 );
	CreateValue<NrpText>( TEXTURENORMAL, "" );
	CreateValue<float>( DISCOUNT, 0.f );
	CreateValue<float>( MAXDISCOUNT, 0.f );
	CreateValue<float>( REJECT, 0.f );
	CreateValue<float>( LINEDISCOUNT, 0.f );
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
	if( !IsValueExist( valueName ) )
		CreateValue<int>( valueName, valuel );
	else
		AddValue<int>( valueName, valuel );
}

NrpText CNrpDiskMachine::ClassName()
{
	return CLASS_DISKMACHINE;
}

}//end namespace nrp