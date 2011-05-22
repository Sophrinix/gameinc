#include "StdAfx.h"
#include "NrpDiskMachine.h"
#include "NrpTime.h"

namespace nrp
{
CLASS_NAME CLASS_DISKMACHINE( "CNrpDiskMachine" );

CNrpDiskMachine::CNrpDiskMachine(void) : INrpConfig( CLASS_DISKMACHINE, "" )
{
	Add<NrpText>( NAME, "" );
	Add( STARTDATE, NrpTime( 0. ) );	
	Add( ENDDATE, NrpTime( 0. ) );
	Add<int>( DISKPERHOUR, 0 );
	Add<int>( PRICEPERHOUR, 0 );
	Add<int>( RENTPRICE, 0 );
	Add<NrpText>( TEXTURENORMAL, "" );
	Add<float>( DISCOUNT, 0.f );
	Add<float>( MAXDISCOUNT, 0.f );
	Add<float>( REJECT, 0.f );
	Add<float>( LINEDISCOUNT, 0.f );
	Add<NrpText>( INTERNAL_NAME, "" );
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
		Add<int>( valueName, valuel );
	else
		Param( valueName ) += valuel;
}

NrpText CNrpDiskMachine::ClassName()
{
	return CLASS_DISKMACHINE;
}

}//end namespace nrp