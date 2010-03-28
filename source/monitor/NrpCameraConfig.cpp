#include "StdAfx.h"
#include <irrlicht.h>
#include "IniFile.h"
#include "NrpCameraConfig.h"

using namespace irr;
static nrp::CNrpCameraConfig * globalCameraConfigInstance = 0;

namespace nrp
{

CNrpCameraConfig::CNrpCameraConfig(void) : INrpConfig( "CNrpCameraConfig", "cameraConfig"), 
										   SECTION_NAME( "options" )
{
}

CNrpCameraConfig::~CNrpCameraConfig(void)
{
}

CNrpCameraConfig& CNrpCameraConfig::Instance()
{
	if( !globalCameraConfigInstance)
	{
		globalCameraConfigInstance = new CNrpCameraConfig();
		globalCameraConfigInstance->Load_( "config/camera.ini" );
	}

	return *globalCameraConfigInstance;
}

void CNrpCameraConfig::Load_( char* file_name )
{
	options_[ CONFIG_FILE ] = (LPVOID)new std::string( file_name );								//запоминаем путь к файлу настроек
	options_[ CAMERA_ROTATE_SPEED ] = Read_<float>( SECTION_NAME, CAMERA_ROTATE_SPEED, 0 );
	options_[ CAMERA_ZOOM_SPEED ] = Read_<float>( SECTION_NAME, CAMERA_ZOOM_SPEED, 0 );
	options_[ CAMERA_TRANSLATION_SPEED ] = Read_<float>( SECTION_NAME, CAMERA_TRANSLATION_SPEED, 0 );
	options_[ CAMERA_FARVALUE ] = Read_<float>( SECTION_NAME, CAMERA_FARVALUE, 0 ); 
}

}//namespace nrp
