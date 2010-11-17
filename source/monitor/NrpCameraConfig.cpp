#include "StdAfx.h"
#include <irrlicht.h>
#include "IniFile.h"
#include "NrpCameraConfig.h"

using namespace irr;
static nrp::CNrpCameraConfig * globalCameraConfigInstance = 0;

namespace nrp
{
CLASS_NAME CLASS_CAMERACONFIG( L"CNrpCameraConfig" );

CNrpCameraConfig::CNrpCameraConfig(void) : INrpConfig( CLASS_CAMERACONFIG, CLASS_CAMERACONFIG)
{
	Load( "config/camera.ini" );
}

CNrpCameraConfig::~CNrpCameraConfig(void)
{
}

CNrpCameraConfig& CNrpCameraConfig::Instance()
{
	if( !globalCameraConfigInstance )
		globalCameraConfigInstance = new CNrpCameraConfig();

	return *globalCameraConfigInstance;
}

NrpText CNrpCameraConfig::ClassName()
{
	return CLASS_CAMERACONFIG;
}
/*
CreateValue<float>( CAMERA_ROTATE_SPEED, Read_<float>( SECTION_NAME, CAMERA_ROTATE_SPEED, 0 ) );
CreateValue<float>( CAMERA_ZOOM_SPEED, Read_<float>( SECTION_NAME, CAMERA_ZOOM_SPEED, 0 ) );
CreateValue<float>( CAMERA_TRANSLATION_SPEED, Read_<float>( SECTION_NAME, CAMERA_TRANSLATION_SPEED, 0 ) );
CreateValue<float>( CAMERA_FARVALUE, Read_<float>( SECTION_NAME, CAMERA_FARVALUE, 0 ) ); 
*/
}//namespace nrp
