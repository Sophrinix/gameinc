#pragma once
#include "IUser.h"

namespace nrp
{

class CNrpCompany;

class CNrpPlayer : public IUser
{
public:
	CNrpPlayer( std::string name, CNrpCompany* ptrCmp );
	~CNrpPlayer(void);
private:
	CNrpCompany* company_;
};

}//namespace nrp