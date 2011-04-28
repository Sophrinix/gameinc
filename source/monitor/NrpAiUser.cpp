#include "StdAfx.h"
#include "NrpAiUser.h"

namespace nrp
{
CLASS_NAME CLASS_AIUSER( "CNrpAiUser" );	

CNrpAiUser::CNrpAiUser( const NrpText& name, CNrpCompany* ptrCmp ) : CNrpUser( CLASS_AIUSER, name )
{
	assert( name.size() > 0 );
	_self[ NAME ] = name;
}

CNrpAiUser::CNrpAiUser( const NrpText& fileName ) : CNrpUser( CLASS_AIUSER, fileName )
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