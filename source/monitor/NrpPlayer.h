#pragma once
#include "IUser.h"

namespace nrp
{

const std::string CLASS_REALPLAYER("RealPlayer");
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