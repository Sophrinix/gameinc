#include "StdAfx.h"
#include "NrpAiUser.h"

namespace nrp
{

CNrpAiUser::CNrpAiUser( const std::string& name, CNrpCompany* ptrCmp ) : IUser( CLASS_AIUSER, name )
{
	SetValue< std::string >( NAME, name );
}

CNrpAiUser::CNrpAiUser( const std::string& fileName ) : IUser( CLASS_AIUSER, "" )
{
	Load( fileName );
}

CNrpAiUser::~CNrpAiUser(void)
{
}

}//namespace nrp