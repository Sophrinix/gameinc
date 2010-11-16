#pragma once
//////////////////////////////////////////////////////////////////////////
#include "nrpConfig.h"

#include <vector>
//////////////////////////////////////////////////////////////////////////

namespace nrp
{

OPTION_NAME FULLSCREEN("fullScreen");
OPTION_NAME USE_SHADOWS("useShadows");
OPTION_NAME VSYNC("vSync");
OPTION_NAME SCREEN_SIZE("screenSize");

class CNrpVideoConfig : public INrpConfig
{
private:
	CNrpVideoConfig();
	static NrpText ClassName();

public:
	static CNrpVideoConfig& Instance();
};

}//namespace nrp