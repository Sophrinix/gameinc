#pragma once

#include "nrpconfig.h"

namespace nrp
{
const NrpText CAMERA_ROTATE_SPEED( L"rotateSpeed" );
const NrpText CAMERA_ZOOM_SPEED( L"zoomSpeed" );
const NrpText CAMERA_TRANSLATION_SPEED( L"translationSpeed" );
const NrpText CAMERA_FARVALUE( L"farValue" );

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