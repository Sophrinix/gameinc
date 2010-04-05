#pragma once
#include "IUser.h"

namespace nrp
{

class CNrpCompany;

class CNrpPlayer : public IUser
{
public:
	CNrpPlayer( std::string name );
	~CNrpPlayer(void);
private:
	CNrpCompany* company_;
};

}//namespace nrp