#pragma once
//////////////////////////////////////////////////////////////////////////
#include "nrpConfig.h"

#include <vector>
//////////////////////////////////////////////////////////////////////////

namespace nrp
{

OPTION_NAME FULLSCREEN(L"fullScreen");
OPTION_NAME USE_SHADOWS(L"useShadows");
OPTION_NAME VSYNC(L"vSync");
OPTION_NAME SCREEN_SIZE(L"screenSize");

class CNrpVideoConfig : public INrpConfig
{
private:
	CNrpVideoConfig();
	static NrpText ClassName();

public:
	static CNrpVideoConfig& Instance();
};

}//namespace nrp