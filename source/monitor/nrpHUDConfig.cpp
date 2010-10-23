#include "StdAfx.h"
#include <irrlicht.h>
#include "nrpHUDConfig.h"
#include "StrConversation.h"
//////////////////////////////////////////////////////////////////////////

static nrp::CNrpHUDConfig * global_hud_config_instance = 0;

using namespace irr;
using namespace core;
using namespace video;

/////////////////////////////////////////////////////////////////////////

namespace nrp
{

CNrpHUDConfig::CNrpHUDConfig() : INrpConfig("CNrpHUDConfig", "hudConfig")
{		
	CreateValue<int>( MIN_FONT_SIZE, 8 );
	CreateValue<int>( MAX_FONT_SIZE, 8 );

	for( int cnt=GetValue<int>(MIN_FONT_SIZE); cnt < GetValue<int>(MAX_FONT_SIZE); cnt++)
		CreateValue<std::string>( "font_" + nrp::IntToStr(cnt), "" );

	Load( "config/hud.ini" );
}
//////////////////////////////////////////////////////////////////////////

CNrpHUDConfig& CNrpHUDConfig::Instance()
{
	if( !global_hud_config_instance)
		global_hud_config_instance = new CNrpHUDConfig();

	return *global_hud_config_instance;
}

} //namespace nrp