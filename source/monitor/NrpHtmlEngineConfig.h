#pragma once
#include "nrpconfig.h"

namespace nrp
{

class CNrpHtmlEngineConfig : public INrpConfig
{
	const std::string SECTION_NAME;
private:
	CNrpHtmlEngineConfig();

	void Load_( char* file_name );
public:
	static CNrpHtmlEngineConfig& Instance();
};

}//namespace nrp