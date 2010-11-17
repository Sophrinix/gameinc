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
CLASS_NAME CLASS_HUDCONFIG( L"CNrpHUDConfig" );

CNrpHUDConfig::CNrpHUDConfig() : INrpConfig(CLASS_HUDCONFIG, CLASS_HUDCONFIG)
{		
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
} //namespace nrp