#include "StdAfx.h"
#include "NrpPlayer.h"

namespace nrp
{

CNrpPlayer::CNrpPlayer( const std::string& name, CNrpCompany* ptrCmp ) : IUser( CLASS_REALPLAYER, name )
{
	SetValue<std::string>( NAME, name );
}

CNrpPlayer::CNrpPlayer( const std::string& fileName ) : IUser( CLASS_REALPLAYER, std::string("") )
{
	Load( fileName );
}

CNrpPlayer::~CNrpPlayer(void)
{
}

}//namespace nrp