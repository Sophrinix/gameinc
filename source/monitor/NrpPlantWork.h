#pragma once
#include "nrpConfig.h"

namespace nrp
{

OPTION_NAME CLASS_NRPPLANTWORK( "CNrpPlantWork" );

OPTION_NAME PRODUCETYPE( "produceType" );
OPTION_NAME NUMBERMACHINE( "numberMachine" );
OPTION_NAME NUMBERDAY( "numberDay" );
OPTION_NAME DISKNUMBER( "diskNumber" );
OPTION_NAME DISKPRICE( "diskPrice" );

class CNrpPlantWork : public INrpConfig
{
	void Load_( char* file_name ) {};
public:
	CNrpPlantWork();
	~CNrpPlantWork(void);

	void Load( std::string sectionName, std::string fileName );
};

}//end of namespace nrp