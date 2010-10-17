#pragma once
#include "nrpconfig.h"

namespace nrp
{
const std::string SKY_TEXTURE("texture");
CLASS_NAME CLASS_SKYCONFIG( "CNrpSkyConfig" );

class CNrpSkyConfig : public INrpConfig
{
	const std::string SECTION_NAME;
private:
	CNrpSkyConfig();

	void Load_( char* file_name );

	virtual std::string ObjectName() { return CLASS_SKYCONFIG; }
	static std::string ClassName() { return CLASS_SKYCONFIG; }
public:
	static CNrpSkyConfig& Instance();
};

}//namespace nrp