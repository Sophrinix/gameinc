#include "StdAfx.h"
#include <irrlicht.h>
#include "IniFile.h"
#include "nrpVideoConfig.h"
//////////////////////////////////////////////////////////////////////////

static nrp::CNrpVideoConfig * globalVideoConfigInstance = 0;

using namespace irr;
using namespace core;
using namespace video;

//////////////////////////////////////////////////////////////////////////

namespace nrp
{

CNrpVideoConfig::CNrpVideoConfig() : INrpConfig( "CNrpVideoConfig", "videoConfig"), 
									 SECTION_NAME("options")							//читаем конфиг для видео опций
{		
	//DbgLog(conf) << con << "OptionsVideo object created" << term;
}
//////////////////////////////////////////////////////////////////////////

CNrpVideoConfig& CNrpVideoConfig::Instance()
{
	if( !globalVideoConfigInstance)
	{
		globalVideoConfigInstance = new CNrpVideoConfig();
		globalVideoConfigInstance->Load_( "config/video.ini" );
	}

	return *globalVideoConfigInstance;
}

void CNrpVideoConfig::Load_( char* file_name )
{
	CreateValue< std::string >( CONFIG_FILE, file_name );								//запоминаем путь к файлу настроек
	CreateValue<dimension2du>( SCREEN_SIZE, Read_< dimension2du >( SECTION_NAME, SCREEN_SIZE, dimension2du( 1024, 768 ) ) );//высота экрана
	CreateValue<bool>( FULLSCREEN, Read_< bool >( SECTION_NAME, FULLSCREEN,	false ) );		//полноэкранный режим
	CreateValue<bool>( USE_SHADOWS, Read_< bool >( SECTION_NAME, USE_SHADOWS,	false ) );		//отображение теней
	CreateValue<bool>( VSYNC, Read_< bool >( SECTION_NAME, VSYNC,		false ) );	//вертикальная синх.
}
//////////////////////////////////////////////////////////////////////////

}//namespace nrp