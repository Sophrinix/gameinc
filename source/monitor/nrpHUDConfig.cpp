#include "StdAfx.h"
#include <irrlicht.h>
#include "nrpHUDConfig.h"
#include "NrpText.h"
//////////////////////////////////////////////////////////////////////////

static nrp::CNrpHUDConfig * global_hud_config_instance = 0;

using namespace irr;
using namespace core;
using namespace video;

/////////////////////////////////////////////////////////////////////////

namespace nrp
{
CLASS_NAME CLASS_HUDCONFIG( "CNrpHUDConfig" );

CNrpHUDConfig::CNrpHUDConfig() : INrpConfig(CLASS_HUDCONFIG, CLASS_HUDCONFIG)
{		
	CreateValue<int>( MIN_FONT_SIZE, 8 );
	CreateValue<int>( MAX_FONT_SIZE, 8 );

	for( int cnt=GetValue<int>(MIN_FONT_SIZE); cnt < GetValue<int>(MAX_FONT_SIZE); cnt++)
		CreateValue<NrpText>( NrpText("font_") + NrpText(cnt), "" );

	Load( "config/hud.ini" );
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
} //namespace nrp