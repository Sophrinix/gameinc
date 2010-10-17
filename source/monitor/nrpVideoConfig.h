#pragma once
//////////////////////////////////////////////////////////////////////////
#include "nrpConfig.h"

#include <vector>
#include <string>
//////////////////////////////////////////////////////////////////////////

namespace nrp
{

CLASS_NAME CLASS_VIDEOCONFIG( "CNrpVideoConfig" );

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

	virtual std::string ObjectName() { return CLASS_VIDEOCONFIG; }
	static std::string ClassName() { return CLASS_VIDEOCONFIG; }
public:
	static CNrpVideoConfig& Instance();
};

}//namespace nrp