#pragma once
#include "nrpconfig.h"

namespace nrp
{
CLASS_NAME CLASS_HTMLENGCONFIG( "CNrpHtmlEngineConfig" );

class CNrpHtmlEngineConfig : public INrpConfig
{
	const std::string SECTION_NAME;
private:
	CNrpHtmlEngineConfig();

	void Load_( char* file_name );
public:
	static CNrpHtmlEngineConfig& Instance();
	virtual std::string ObjectName() { return CLASS_HTMLENGCONFIG; }
	static std::string ClassName() { return CLASS_HTMLENGCONFIG; }
};

}//namespace nrp