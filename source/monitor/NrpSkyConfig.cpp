#include "StdAfx.h"
#include "NrpSkyConfig.h"

static nrp::CNrpSkyConfig * globalSkyConfigInstance = 0;

namespace nrp
{
CLASS_NAME CLASS_SKYCONFIG( "CNrpSkyConfig" );

CNrpSkyConfig::CNrpSkyConfig(void) : INrpConfig( CLASS_SKYCONFIG, CLASS_SKYCONFIG )
{
	CreateValue<NrpText>( SKY_TEXTURE, "" );

	Load( "config/sky.ini" );
}

CNrpSkyConfig& CNrpSkyConfig::Instance()
{
	if( !globalSkyConfigInstance)
		globalSkyConfigInstance = new CNrpSkyConfig();

	return *globalSkyConfigInstance;
}

NrpText CNrpSkyConfig::ClassName()
{
	return CLASS_SKYCONFIG;
}
}//namespace nrp