#include "StdAfx.h"
#include "NrpAiUser.h"

namespace nrp
{

CNrpAiUser::CNrpAiUser( std::string name, CNrpCompany* ptrCmp ) : IUser( "AiPlayer", name.c_str(), ptrCmp )
{
	SetValue< std::string >( NAME, name );
}

CNrpAiUser::~CNrpAiUser(void)
{
}

}//namespace nrp