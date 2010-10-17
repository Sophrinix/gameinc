#pragma once
#include "IUser.h"

namespace nrp
{

CLASS_NAME CLASS_REALPLAYER("RealPlayer");
class CNrpCompany;

class CNrpPlayer : public IUser
{
public:
	CNrpPlayer( const std::string& name, CNrpCompany* ptrCmp );
	~CNrpPlayer(void);

	static std::string ClassName() { return CLASS_REALPLAYER; }
private:
	CNrpCompany* company_;
};

}//namespace nrp