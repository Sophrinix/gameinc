#pragma once
#include "nrpconfig.h"

namespace nrp
{
const std::string SKY_TEXTURE("texture");

class CNrpSkyConfig : public INrpConfig
{
	const std::string SECTION_NAME;
private:
	CNrpSkyConfig();

	void Load_( char* file_name );
public:
	static CNrpSkyConfig& Instance();
};

}//namespace nrp