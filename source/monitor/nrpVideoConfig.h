#pragma once
//////////////////////////////////////////////////////////////////////////
#include "nrpConfig.h"

#include <vector>
#include <string>
//////////////////////////////////////////////////////////////////////////

namespace nrp
{

const std::string FULLSCREEN("fullScreen");
const std::string USE_SHADOWS("useShadows");
const std::string VSYNC("vSync");
const std::string SCREEN_SIZE("screenSize");

class CNrpVideoConfig : public INrpConfig
{
private:
	CNrpVideoConfig();

	void Load_( char* file_name );

	const std::string SECTION_NAME;
public:
	static CNrpVideoConfig& Instance();
};

}//namespace nrp