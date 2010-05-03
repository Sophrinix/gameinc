#include "StdAfx.h"
#include "NrpPlayer.h"

namespace nrp
{

CNrpPlayer::CNrpPlayer( std::string name, CNrpCompany* ptrCmp ) : IUser( "RealPlayer", name.c_str(), ptrCmp )
{
	SetValue<std::string>( NAME, name );
}

CNrpPlayer::~CNrpPlayer(void)
{
}

}//namespace nrp