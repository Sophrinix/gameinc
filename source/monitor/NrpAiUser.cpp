#include "StdAfx.h"
#include "NrpAiUser.h"

namespace nrp
{
CLASS_NAME CLASS_AIUSER( "CNrpAiUser" );	

CNrpAiUser::CNrpAiUser( const NrpText& name, CNrpCompany* ptrCmp ) : IUser( CLASS_AIUSER, name )
{
	assert( name.size() > 0 );
	Param( NAME ) = name;
}

CNrpAiUser::CNrpAiUser( const NrpText& fileName ) : IUser( CLASS_AIUSER, "" )
{
	Load( fileName );
}

CNrpAiUser::~CNrpAiUser(void)
{
}

NrpText CNrpAiUser::ClassName()
{
	return CLASS_AIUSER;
}

}//namespace nrp