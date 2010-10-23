#include "StdAfx.h"
#include "NrpSkyConfig.h"

static nrp::CNrpSkyConfig * globalSkyConfigInstance = 0;

namespace nrp
{

CNrpSkyConfig::CNrpSkyConfig(void) : INrpConfig( CLASS_SKYCONFIG, CLASS_SKYCONFIG )
{
	CreateValue<std::string>( SKY_TEXTURE, "" );

	Load( "config/sky.ini" );
}

CNrpSkyConfig& CNrpSkyConfig::Instance()
{
	if( !globalSkyConfigInstance)
		globalSkyConfigInstance = new CNrpSkyConfig();

	return *globalSkyConfigInstance;
}

}//namespace nrp