#pragma once
#include "IUser.h"
#include <string>

namespace nrp
{

CLASS_NAME CLASS_AIUSER( "CNrpAiUser" );	

class CNrpAiUser : public IUser
{
public:
	CNrpAiUser( const std::string& name, CNrpCompany* ptrCmp );
	~CNrpAiUser(void);	

	static std::string ClassName() { return CLASS_AIUSER; } 
};

}//namespace nrp