#include "StdAfx.h"
#include "NrpPlayer.h"

namespace nrp
{

CNrpPlayer::CNrpPlayer( const std::string& name, CNrpCompany* ptrCmp ) : IUser( CLASS_REALPLAYER, name )
{
	SetValue<std::string>( NAME, name );
}

CNrpPlayer::~CNrpPlayer(void)
{
}

}//namespace nrp