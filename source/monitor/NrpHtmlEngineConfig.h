#pragma once
#include "nrpconfig.h"

namespace nrp
{
CLASS_NAME CLASS_HTMLENGCONFIG( "CNrpHtmlEngineConfig" );

class CNrpHtmlEngineConfig : public INrpConfig
{
private:
	CNrpHtmlEngineConfig();

public:
	static CNrpHtmlEngineConfig& Instance();
	static std::string ClassName() { return CLASS_HTMLENGCONFIG; }
};

}//namespace nrp