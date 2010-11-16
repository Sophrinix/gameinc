#include "StdAfx.h"
#include "NrpHtmlEngineConfig.h"

static nrp::CNrpHtmlEngineConfig * globalHtmlConfigInstance = 0;

namespace nrp
{
CLASS_NAME CLASS_HTMLENGCONFIG( "CNrpHtmlEngineConfig" );

CNrpHtmlEngineConfig::CNrpHtmlEngineConfig(void) : INrpConfig( CLASS_HTMLENGCONFIG, CLASS_HTMLENGCONFIG )
{
	Load( "config/html.ini" );
}

CNrpHtmlEngineConfig& CNrpHtmlEngineConfig::Instance()
{
	if( !globalHtmlConfigInstance)
		globalHtmlConfigInstance = new CNrpHtmlEngineConfig();

	return *globalHtmlConfigInstance;
}

NrpText CNrpHtmlEngineConfig::ClassName()
{
	return CLASS_HTMLENGCONFIG;
}

}//namespace nrp