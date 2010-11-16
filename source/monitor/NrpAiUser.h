#pragma once
#include "IUser.h"

namespace nrp
{

class CNrpAiUser : public IUser
{
public:
	CNrpAiUser( const NrpText& name, CNrpCompany* ptrCmp );
	CNrpAiUser( const NrpText& fileName );
	~CNrpAiUser(void);	

	static NrpText ClassName(); 
};

}//namespace nrp