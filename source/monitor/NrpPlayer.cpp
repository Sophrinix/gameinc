#include "StdAfx.h"
#include "NrpPlayer.h"

namespace nrp
{
CLASS_NAME CLASS_REALPLAYER("RealPlayer");

CNrpPlayer::CNrpPlayer( const NrpText& name, CNrpCompany* ptrCmp ) : IUser( CLASS_REALPLAYER, name )
{
	assert( name.size() > 0 );
	_self[ NAME ] = name;
}

CNrpPlayer::CNrpPlayer( const NrpText& fileName ) : IUser( CLASS_REALPLAYER, fileName )
{
	Load( fileName );
	assert( ((NrpText)_self[ NAME ]).size() > 0 );
}

CNrpPlayer::~CNrpPlayer(void)
{
}

NrpText CNrpPlayer::ClassName()
{
	return CLASS_REALPLAYER;
}

}//namespace nrp