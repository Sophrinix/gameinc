#include "StdAfx.h"
#include "NrpPlatform.h"

namespace nrp
{
CLASS_NAME CLASS_PLATFORM( "CNrpPlatform" );

CNrpPlatform::CNrpPlatform( const NrpText& name ) : INrpProject( CLASS_PLATFORM, name )
{
	_InitialyzeOptions();
}

CNrpPlatform::CNrpPlatform( void ) : INrpProject( CLASS_PLATFORM, CLASS_PLATFORM )
{
	throw std::exception( "deprecated constructor" );
}

CNrpPlatform::~CNrpPlatform(void)
{
}

void CNrpPlatform::_InitialyzeOptions()
{
	Add( RAM, 0.f );
	Add( CPU, 0.f );
	Add( INTERNAL_NAME, NrpText() );
	Add( MAXWIDTH, 0 );
	Add( MAXHEIGHT, 0 );
	Add( MAXVIDEO, NrpText() );
	Add( SELLDEVICE, 0 );
	Add( TEXTURENORMAL, NrpText() );
}

NrpText CNrpPlatform::ClassName()
{
	return CLASS_PLATFORM;
}

NrpText CNrpPlatform::Save( const NrpText& pathTo )
{
	throw std::exception( "unfinished function" );
	return "";
}

void CNrpPlatform::Load( const NrpText& pathTo )
{
	INrpProject::Load( pathTo );
}

}//end namespace platform