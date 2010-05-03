#pragma once
#include "IUser.h"

namespace nrp
{

class CNrpAiUser : public IUser
{
public:
	CNrpAiUser( std::string name, CNrpCompany* ptrCmp );
	~CNrpAiUser(void);	
};

}//namespace nrp