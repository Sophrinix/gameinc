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
	options_[ CONFIG_FILE ] = (LPVOID)new std::string( file_name );								//запоминаем путь к файлу настроек
	options_[ MIN_FONT_SIZE ] = Read_<int>( SECTION_NAME, MIN_FONT_SIZE, 8 );
	options_[ MAX_FONT_SIZE ] = Read_<int>( SECTION_NAME, MAX_FONT_SIZE, 8 );

	for( int cnt=GetOption<int>(MIN_FONT_SIZE);
		 cnt < GetOption<int>(MAX_FONT_SIZE);
		 cnt++)
	{
		std::string fname = "font_" + nrp::IntToStr(cnt);
		options_[ fname ] = Read_< std::string >( SECTION_NAME, fname, "" );
	}
}
//////////////////////////////////////////////////////////////////////////

} //namespace nrp