#pragma once
#include "nrpConfig.h"

namespace nrp
{
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

	void Load( const NrpText& fileName );
	void AddProducedDisk( const NrpText& companyName, int valuel );

	static NrpText ClassName();
};

typedef CNrpDiskMachine* PNrpDiskMachine;
}//end namespace nrp