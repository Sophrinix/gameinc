#pragma once

#include "nrpconfig.h"

namespace nrp
{
const std::string CAMERA_ROTATE_SPEED( "rotateSpeed" );
const std::string CAMERA_ZOOM_SPEED( "zoomSpeed" );
const std::string CAMERA_TRANSLATION_SPEED( "translationSpeed" );
const std::string CAMERA_FARVALUE( "farValue" );

class CNrpCameraConfig : public INrpConfig
{
	const std::string SECTION_NAME;
private:

	CNrpCameraConfig();
	~CNrpCameraConfig(void);
	void Load_( char* file_name );
public:
	static CNrpCameraConfig& Instance();
};

}//namespace nrp