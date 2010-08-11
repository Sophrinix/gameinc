#pragma once
#include "NrpProjectModule.h"

namespace nrp
{
OPTION_NAME REALPRICE( "realprice" );
OPTION_NAME PASSEDPRICE( "passedprice" );
OPTION_NAME INVESTIMENT( "investiment" );
OPTION_NAME INVENTIONSPEED( "inventionspeed" );
OPTION_NAME PROGNOSEDATEFINISH( "prognosedatefinish" );
OPTION_NAME DAYLEFT( "dayleft" );

class CNrpInvention : public CNrpProjectModule
{
public:
	CNrpInvention(CNrpTechnology* pTech, CNrpCompany* pCmp);
	~CNrpInvention(void);

	virtual void Update( IUser* ptrUser );
	void CheckParams();

private:
	CNrpInvention(void) : CNrpProjectModule( 0, NULL ) {};
	int _GetRealPrice( CNrpTechnology* tech );
};

}//end namespace nrp