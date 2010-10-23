#pragma once
#include "nrpconfig.h"

namespace nrp
{
const std::string SKY_TEXTURE("texture");
CLASS_NAME CLASS_SKYCONFIG( "CNrpSkyConfig" );

class CNrpSkyConfig : public INrpConfig
{
private:
	CNrpSkyConfig();

	static std::string ClassName() { return CLASS_SKYCONFIG; }

public:
	static CNrpSkyConfig& Instance();
};

}//namespace nrp