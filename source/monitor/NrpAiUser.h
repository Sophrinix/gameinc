#pragma once
#include "IUser.h"

namespace nrp
{

class CNrpAiUser : public IUser
{
public:
	CNrpAiUser( std::string name );
	~CNrpAiUser(void);	
};

}//namespace nrp