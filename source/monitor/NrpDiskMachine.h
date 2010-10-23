#pragma once
#include "nrpConfig.h"

namespace nrp
{

CLASS_NAME CLASS_DISKMACHINE( "CNrpDiskMachine" );

OPTION_NAME DISKPERHOUR( "diskPerHour" );
OPTION_NAME PRICEPERHOUR( "pricePerHour" );

class CNrpDiskMachine : public INrpConfig
{
public:
	CNrpDiskMachine(void);
	~CNrpDiskMachine(void);

	void Load( std::string sectionName, std::string fileName );

	static std::string ClassName() { return CLASS_DISKMACHINE; }
};

typedef CNrpDiskMachine* PNrpDiskMachine;
}//end namespace nrp