#include "StdAfx.h"
#include "NrpHtmlEngineConfig.h"
static nrp::CNrpHtmlEngineConfig * globalHtmlConfigInstance = 0;

namespace nrp
{
CLASS_NAME CLASS_HTMLENGCONFIG( "CNrpHtmlEngineConfig" );

const NrpText CNrpHtmlEngineConfig::defaultConfig = L"config/html.ini";

CNrpHtmlEngineConfig::CNrpHtmlEngineConfig(void) : INrpConfig( CLASS_HTMLENGCONFIG, CLASS_HTMLENGCONFIG )
{
	RegProperty(BASEDIR, NrpText(""));
	Load( defaultConfig );
	assert( Param(BASEDIR).As<NrpText>().size() > 0 );
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

}