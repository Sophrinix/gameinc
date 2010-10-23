#include "StdAfx.h"
#include "NrpHtmlEngineConfig.h"

static nrp::CNrpHtmlEngineConfig * globalHtmlConfigInstance = 0;

namespace nrp
{

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

}//namespace nrp