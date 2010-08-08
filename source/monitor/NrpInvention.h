#pragma once
#include "NrpProjectModule.h"

namespace nrp
{
OPTION_NAME REALPRICE( "realprice" );
OPTION_NAME PASSEDPRICE( "passedprice" );
OPTION_NAME INVESTIMENT( "investiment" );

class CNrpInvention : public CNrpProjectModule
{
public:
	CNrpInvention(CNrpTechnology* pTech, CNrpCompany* pCmp);
	~CNrpInvention(void);

	virtual void Update( IUser* ptrUser );
private:
	CNrpInvention(void) : CNrpProjectModule( 0, NULL ) {};
	int _GetRealPrice( CNrpTechnology* tech );
};

}//end namespace nrp