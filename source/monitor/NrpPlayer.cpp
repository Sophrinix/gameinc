#include "StdAfx.h"
#include "NrpPlayer.h"

namespace nrp
{

CNrpPlayer::CNrpPlayer( std::string name ) : IUser( "RealPlayer", name.c_str() )
{
	SetOption<std::string>( NAME, name );
}

CNrpPlayer::~CNrpPlayer(void)
{
}

}//namespace nrp