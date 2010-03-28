#include "StdAfx.h"
#include "NrpAiUser.h"

namespace nrp
{

CNrpAiUser::CNrpAiUser( std::string name ) : IUser( "AiPlayer", name.c_str() )
{
	SetOption< std::string >( NAME, name );
}

CNrpAiUser::~CNrpAiUser(void)
{
}

}//namespace nrp