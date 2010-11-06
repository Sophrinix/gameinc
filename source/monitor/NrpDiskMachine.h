#pragma once
#include "nrpConfig.h"

namespace nrp
{

CLASS_NAME CLASS_DISKMACHINE( "CNrpDiskMachine" );

OPTION_NAME DISKPERHOUR( "diskPerHour" );
OPTION_NAME PRICEPERHOUR( "pricePerHour" );
OPTION_NAME DISCOUNT( "discount" );
OPTION_NAME LINEDISCOUNT( "linediscount" );
OPTION_NAME MAXDISCOUNT( "maxdiscount" );
OPTION_NAME REJECT( "reject" );
OPTION_NAME DISKPRODUCED( "diskproduced" );

class CNrpDiskMachine : public INrpConfig
{
public:
	CNrpDiskMachine(void);
	~CNrpDiskMachine(void);

	void Load( const std::string& fileName );
	void AddProducedDisk( const std::string& companyName, int valuel );

	static std::string ClassName() { return CLASS_DISKMACHINE; }
};

typedef CNrpDiskMachine* PNrpDiskMachine;
}//end namespace nrp