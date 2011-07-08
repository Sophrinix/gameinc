#include "StdAfx.h"
#include "NrpPlatform.h"
#include "IniFile.h"
#include "NrpApplication.h"
#include "NrpTechnology.h"
#include "OpFileSystem.h"
#include "NrpHistory.h"

namespace nrp
{
CLASS_NAME CLASS_PLATFORM( "CNrpPlatform" );

const NrpText CNrpPlatform::historyTemplate = L"platform.history";
const NrpText CNrpPlatform::saveTemplate = L"item.platform";

CNrpPlatform::CNrpPlatform( const NrpText& fileName ) : INrpProject( CLASS_PLATFORM, "" )
{
	_InitialyzeOptions();

	Load( fileName );
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
	RegProperty( RAM, 0.f );
	RegProperty( CPU, 0.f );
	RegProperty( INTERNAL_NAME, NrpText() );
	RegProperty( MAXWIDTH, 0 );
	RegProperty( MAXHEIGHT, 0 );
	RegProperty( SELLDEVICE, 0 );
	RegProperty( TEXTURENORMAL, NrpText() );
	RegProperty( TECHNUMBER, 0 );
	RegProperty( BASE_CODE, 0.f );
	RegProperty( ENGINE_CODE, 0.f );
	RegProperty( LEVEL, 0 );
    RegProperty( PRICE, 0 );
	RegProperty( QUALITY, 100 );
	RegProperty( STARTDATE, NrpTime( 0. ) );
	RegProperty( ENDDATE, NrpTime( 0. ) );
    RegProperty( HISTORY, (CNrpHistory*)NULL );
}

NrpText CNrpPlatform::ClassName()
{
	return CLASS_PLATFORM;
}

NrpText CNrpPlatform::Save( const NrpText& pathTo )
{
	assert( OpFileSystem::IsExist(pathTo) );

	NrpText localFolder = OpFileSystem::CheckEndSlash( pathTo + Text( INTERNAL_NAME ) );

	OpFileSystem::CreateDirectory( localFolder );

    NrpText fileName = localFolder + CNrpPlatform::saveTemplate;
	INrpProject::Save( fileName );

	IniFile sv( fileName );
	sv.Set( SECTION_TECHS, _techs, CreateKeyTech, INTERNAL_NAME );

	sv.Save();

    assert( _self[ HISTORY ].As<CNrpHistory*>() );
    if( _self[ HISTORY ].As<CNrpHistory*>() )
        _self[ HISTORY ].As<CNrpHistory*>()->Save( localFolder + CNrpPlatform::historyTemplate );

	return fileName;
}

void CNrpPlatform::Load( const NrpText& pathTo )
{
	INrpProject::Load( pathTo );

	IniFile lv( pathTo );

	lv.Get( SECTION_TECHS, CreateKeyTech, _self[ TECHNUMBER ], _techs, &CNrpApplication::GetTechnology, &_nrpApp );

    NrpText historyIni = OpFileSystem::UpDir( pathTo ) + CNrpPlatform::historyTemplate;
    _self[ HISTORY ] = OpFileSystem::IsExist( historyIni ) ? new CNrpHistory( historyIni ) : new CNrpHistory();
}

CNrpTechnology* CNrpPlatform::GetTech( const NrpText& name )
{
	return FindByNameAndIntName< TECHS, CNrpTechnology >( _techs, name );
}

}//end namespace platform