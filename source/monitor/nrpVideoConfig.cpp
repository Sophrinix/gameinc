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

CNrpVideoConfig::CNrpVideoConfig() : INrpConfig( CLASS_VIDEOCONFIG, CLASS_VIDEOCONFIG) 
{		
	CreateValue<dimension2du>( SCREEN_SIZE, dimension2du( 1024, 660 ) );//������ ������
	CreateValue<bool>( FULLSCREEN, false );		//������������� �����
	CreateValue<bool>( USE_SHADOWS, false );		//����������� �����
	CreateValue<bool>( VSYNC, false );	//������������ ����.

	Load( "config/video.ini" );
}

CNrpVideoConfig& CNrpVideoConfig::Instance()
{
	if( !globalVideoConfigInstance)
		globalVideoConfigInstance = new CNrpVideoConfig();

	return *globalVideoConfigInstance;
}

}//namespace nrp