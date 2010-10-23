#pragma once
//////////////////////////////////////////////////////////////////////////
#include "nrpConfig.h"

#include <vector>
#include <string>
//////////////////////////////////////////////////////////////////////////

namespace nrp
{

CLASS_NAME CLASS_VIDEOCONFIG( "CNrpVideoConfig" );

OPTION_NAME FULLSCREEN("fullScreen");
OPTION_NAME USE_SHADOWS("useShadows");
OPTION_NAME VSYNC("vSync");
OPTION_NAME SCREEN_SIZE("screenSize");

class CNrpVideoConfig : public INrpConfig
{
private:
	CNrpVideoConfig();
	static std::string ClassName() { return CLASS_VIDEOCONFIG; }

public:
	static CNrpVideoConfig& Instance();
};

}//namespace nrp