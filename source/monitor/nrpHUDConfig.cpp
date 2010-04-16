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

CNrpHUDConfig::CNrpHUDConfig() : INrpConfig("CNrpHUDConfig", "hudConfig"), SECTION_NAME("Options")							//читаем конфиг для видео опций
{		
	//DbgLog(conf) << con << "OptionsVideo object created" << term;
}
//////////////////////////////////////////////////////////////////////////

CNrpHUDConfig& CNrpHUDConfig::Instance()
{
	if( !global_hud_config_instance)
	{
		global_hud_config_instance = new CNrpHUDConfig();
		global_hud_config_instance->Load_( "config/hud.ini" );
	}

	return *global_hud_config_instance;
}

void CNrpHUDConfig::Load_( char* file_name )
{
	CreateValue<std::string>( CONFIG_FILE, file_name );								//запоминаем путь к файлу настроек
	CreateValue<int>( MIN_FONT_SIZE, Read_<int>( SECTION_NAME, MIN_FONT_SIZE, 8 ) );
	CreateValue<int>( MAX_FONT_SIZE, Read_<int>( SECTION_NAME, MAX_FONT_SIZE, 8 ) );

	for( int cnt=GetValue<int>(MIN_FONT_SIZE);
		 cnt < GetValue<int>(MAX_FONT_SIZE);
		 cnt++)
	{
		std::string fname = "font_" + nrp::IntToStr(cnt);
		CreateValue<std::string>( fname, Read_< std::string >( SECTION_NAME, fname, "" ) );
	}
}
//////////////////////////////////////////////////////////////////////////

} //namespace nrp