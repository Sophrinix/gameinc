#pragma once

#include "nrpconfig.h"

namespace nrp
{
const NrpText CAMERA_ROTATE_SPEED( "rotateSpeed" );
const NrpText CAMERA_ZOOM_SPEED( "zoomSpeed" );
const NrpText CAMERA_TRANSLATION_SPEED( "translationSpeed" );
const NrpText CAMERA_FARVALUE( "farValue" );

class CNrpCameraConfig : public INrpConfig
{
private:
	CNrpCameraConfig();
	~CNrpCameraConfig(void);

	static NrpText ClassName();

public:
	static CNrpCameraConfig& Instance();
};

}//namespace nrp