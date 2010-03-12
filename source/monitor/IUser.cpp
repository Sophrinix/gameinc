#include "StdAfx.h"
#include "IUser.h"

namespace nrp
{

IUser::IUser(void) : INrpConfig( "IUser", "" )
{
	options_[ USER_NAME ] = new std::string( "" );
	options_[ CODE_SPEED ] = new int( 0 );
	options_[ CODE_QUALITY ] = new int( 0 );
	options_[ KNOWLEDGE_LEVEL ] = new int( 0 );
	options_[ TALANT ] = new int( 0 );
	options_[ STAMINA ] = new int( 0 );
	options_[ MOOD ] = new int( 0 );
	options_[ POPULARITY ] = new int( 0 );
	options_[ SALARY ] = new int( 0 );
	options_[ STABILITY ] = new int( 0 );
}

IUser::~IUser(void)
{
}

}//namespace nrp