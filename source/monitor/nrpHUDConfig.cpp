#include "StdAfx.h"
#include <irrlicht.h>
#include "nrpHUDConfig.h"
#include "NrpText.h"
#include "IniFile.h"
//////////////////////////////////////////////////////////////////////////

static nrp::CNrpHUDConfig * global_hud_config_instance = 0;

using namespace irr;
using namespace core;
using namespace video;

/////////////////////////////////////////////////////////////////////////

namespace nrp
{
CLASS_NAME CLASS_HUDCONFIG( L"CNrpHUDConfig" );

CNrpHUDConfig::CNrpHUDConfig() : INrpConfig(CLASS_HUDCONFIG, CLASS_HUDCONFIG)
{		
	RegProperty( FONT_SIMPLE, NrpText("") );
	RegProperty( FONT_TOOLTIP, NrpText("") );

	Load( L"config/hud.ini" );
}
//////////////////////////////////////////////////////////////////////////

CNrpHUDConfig& CNrpHUDConfig::Instance()
{
	if( !global_hud_config_instance)
		global_hud_config_instance = new CNrpHUDConfig();

	return *global_hud_config_instance;
}

NrpText CNrpHUDConfig::ClassName()
{
	return CLASS_HUDCONFIG;
}

void CNrpHUDConfig::Load( const NrpText& fileName )
{
    INrpConfig::Load( fileName );

    IniFile rv( fileName );

    IniSection* fonts = rv.GetSection( SECTION_FONTS );
   
    assert( fonts );
    if( fonts )
    {
        const IniSection::KeyIndexA& keys = fonts->GetKeys();
        for( IniSection::KeyIndexA::const_iterator pIter = keys.begin();
             pIter != keys.end(); pIter++ )
             RegProperty<NrpText>( (*pIter)->GetShortKey().c_str(), (*pIter)->GetValue().c_str() );
    }
    else
        Log( HW ) << "Can't find section \"fonts\" in " << fileName << term;
}
} //namespace nrp