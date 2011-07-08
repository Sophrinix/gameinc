#include "StdAfx.h"
#include "NrpDiskMachine.h"
#include "NrpTime.h"

namespace nrp
{
CLASS_NAME CLASS_DISKMACHINE( "CNrpDiskMachine" );

CNrpDiskMachine::CNrpDiskMachine(void) : INrpConfig( CLASS_DISKMACHINE, "" )
{
	RegProperty<NrpText>( NAME, "" );
	RegProperty( STARTDATE, NrpTime( 0. ) );	
	RegProperty( ENDDATE, NrpTime( 0. ) );
	RegProperty<int>( DISKPERHOUR, 0 );
	RegProperty<int>( PRICEPERHOUR, 0 );
	RegProperty<int>( RENTPRICE, 0 );
	RegProperty<NrpText>( TEXTURENORMAL, "" );
	RegProperty<float>( DISCOUNT, 0.f );
	RegProperty<float>( MAXDISCOUNT, 0.f );
	RegProperty<float>( REJECT, 0.f );
	RegProperty<float>( LINEDISCOUNT, 0.f );
	RegProperty<NrpText>( INTERNAL_NAME, "" );
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
		RegProperty<int>( valueName, valuel );
	else
		Param( valueName ) += valuel;
}

NrpText CNrpDiskMachine::ClassName()
{
	return CLASS_DISKMACHINE;
}

}//end namespace nrp