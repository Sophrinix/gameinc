#include "StdAfx.h"
#include <irrlicht.h>
#include "IniFile.h"
#include "nrpVideoConfig.h"
//////////////////////////////////////////////////////////////////////////

static nrp::CNrpVideoConfig * globalVideoConfigInstance = 0;

using namespace irr;
using namespace core;
using namespace video;

namespace nrp
{
CLASS_NAME CLASS_VIDEOCONFIG( L"CNrpVideoConfig" );

CNrpVideoConfig::CNrpVideoConfig() : INrpConfig( CLASS_VIDEOCONFIG, CLASS_VIDEOCONFIG) 
{		
	Push<dimension2du>( SCREEN_SIZE, dimension2du( 1024, 660 ) );//высота экрана
	Push<bool>( FULLSCREEN, false );		//полноэкранный режим
	Push<bool>( USE_SHADOWS, false );		//отображение теней
	Push<bool>( VSYNC, false );	//вертикальная синх.

	Load( "config/video.ini" );
}

CNrpVideoConfig& CNrpVideoConfig::Instance()
{
	if( !globalVideoConfigInstance)
		globalVideoConfigInstance = new CNrpVideoConfig();

	return *globalVideoConfigInstance;
}

NrpText CNrpVideoConfig::ClassName()
{
	return CLASS_VIDEOCONFIG;
}
}//namespace nrp