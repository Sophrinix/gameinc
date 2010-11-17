#pragma once
#include "nrpconfig.h"

namespace nrp
{
OPTION_NAME SKY_TEXTURE(L"texture");

class CNrpSkyConfig : public INrpConfig
{
private:
	CNrpSkyConfig();

	static NrpText ClassName();

public:
	static CNrpSkyConfig& Instance();
};

}//namespace nrp