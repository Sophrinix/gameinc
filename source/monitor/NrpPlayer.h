#pragma once
#include "IUser.h"

namespace nrp
{
class CNrpCompany;

class CNrpPlayer : public CNrpUser
{
public:
	CNrpPlayer( const NrpText& name, CNrpCompany* ptrCmp );
	CNrpPlayer( const NrpText& fileName );

	~CNrpPlayer(void);

	static NrpText ClassName();
private:
	CNrpCompany* company_;
};

}//namespace nrp