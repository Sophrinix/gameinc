#include "StdAfx.h"
#include "NrpPlantWork.h"
#include "nrpDiskMachine.h"

namespace nrp
{

CNrpPlantWork::CNrpPlantWork() : INrpConfig( CLASS_NRPPLANTWORK, "" )
{
	CreateValue<std::string>( NAME, "" );
	CreateValue<PNrpDiskMachine>( PRODUCETYPE, NULL );
	CreateValue<int>( NUMBERMACHINE, 0 );
	CreateValue<int>( NUMBERDAY, 0 );
	CreateValue<int>( DISKNUMBER, 0 );
	CreateValue<float>( DISKPRICE, 0 );
}

CNrpPlantWork::~CNrpPlantWork(void)
{
}

void CNrpPlantWork::Load( std::string sectionName, std::string fileName )
{
	INrpConfig::Load( sectionName, fileName );
}
}//end namespace nrp 