#include "StdAfx.h"
#include "NrpAiUser.h"

namespace nrp
{

CNrpAiUser::CNrpAiUser( const std::string& name, CNrpCompany* ptrCmp ) : IUser( CLASS_AIUSER, name )
{
	SetValue< std::string >( NAME, name );
}

CNrpAiUser::~CNrpAiUser(void)
{
}

}//namespace nrp