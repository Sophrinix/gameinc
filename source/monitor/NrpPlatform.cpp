#include "StdAfx.h"
#include "NrpPlatform.h"
#include "IniFile.h"
#include "NrpApplication.h"
#include "NrpTechnology.h"
#include "OpFileSystem.h"

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
	Add( SELLDEVICE, 0 );
	Add( TEXTURENORMAL, NrpText() );
	Add( TECHNUMBER, 0 );
	Add( BASE_CODE, 0.f );
}

NrpText CNrpPlatform::ClassName()
{
	return CLASS_PLATFORM;
}

NrpText CNrpPlatform::Save( const NrpText& pathTo )
{
	assert( OpFileSystem::IsExist(pathTo) );

	NrpText localFolder = OpFileSystem::CheckEndSlash( pathTo + Text( NAME ) );

	OpFileSystem::CreateDirectory( localFolder );

	NrpText fileName = localFolder + "item.platform";
	INrpProject::Save( fileName );

	IniFile sv( fileName );
	sv.Set( SECTION_TECHS, _techs, CreateKeyTech, INTERNAL_NAME );

	return fileName;
}

void CNrpPlatform::Load( const NrpText& pathTo )
{
	INrpProject::Load( pathTo );

	IniFile lv( pathTo );

	lv.Get( SECTION_TECHS, CreateKeyTech, _self[ TECHNUMBER ], _techs, &CNrpApplication::GetTechnology, &_nrpApp );
}

CNrpTechnology* CNrpPlatform::GetTech( const NrpText& name )
{
	return FindByNameAndIntName< TECHS, CNrpTechnology >( _techs, name );
}

}//end namespace platform