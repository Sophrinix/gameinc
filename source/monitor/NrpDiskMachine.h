#pragma once
#include "nrpConfig.h"

namespace nrp
{

OPTION_NAME CLASS_DISKMACHINE( "CNrpDiskMachine" );

OPTION_NAME DISKPERHOUR( "diskPerHour" );
OPTION_NAME PRICEPERHOUR( "pricePerHour" );

class CNrpDiskMachine : public INrpConfig
{
	void Load_( char* file_name ) {} ;
public:
	CNrpDiskMachine(void);
	~CNrpDiskMachine(void);

	void Load( std::string sectionName, std::string fileName );
};

typedef CNrpDiskMachine* PNrpDiskMachine;
}//end namespace nrp