#include "StdAfx.h"
#include "NrpSkyConfig.h"

static nrp::CNrpSkyConfig * globalSkyConfigInstance = 0;

namespace nrp
{

CNrpSkyConfig::CNrpSkyConfig(void) : INrpConfig( "CNrpSkyConfig", "skyConfig" ), SECTION_NAME( "options" )
{
}

CNrpSkyConfig& CNrpSkyConfig::Instance()
{
	if( !globalSkyConfigInstance)
	{
		globalSkyConfigInstance = new CNrpSkyConfig();
		globalSkyConfigInstance->Load_( "config/sky.ini" );
	}

	return *globalSkyConfigInstance;
}

void CNrpSkyConfig::Load_( char* file_name )
{
	CreateValue<std::string>( CONFIG_FILE, file_name );								//запоминаем путь к файлу настроек
	CreateValue<std::string>( SKY_TEXTURE, Read_<std::string>( SECTION_NAME, SKY_TEXTURE, "" ) );
}
}//namespace nrp