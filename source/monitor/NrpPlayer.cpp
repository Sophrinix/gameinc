#include "StdAfx.h"
#include "NrpPlayer.h"

namespace nrp
{
CLASS_NAME CLASS_REALPLAYER("RealPlayer");

CNrpPlayer::CNrpPlayer( const NrpText& name, CNrpCompany* ptrCmp ) : IUser( CLASS_REALPLAYER, name )
{
	SetString( NAME, name );
}

CNrpPlayer::CNrpPlayer( const NrpText& fileName ) : IUser( CLASS_REALPLAYER, NrpText("") )
{
	Load( fileName );
}

CNrpPlayer::~CNrpPlayer(void)
{
}

NrpText CNrpPlayer::ClassName()
{
	return CLASS_REALPLAYER;
}

}//namespace nrp