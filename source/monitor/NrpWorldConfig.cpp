#include "StdAfx.h"
#include "NrpWorldConfig.h"

using namespace irr;
static nrp::CNrpWorldConfig * globalTerrainConfigInstance = 0;

namespace nrp 
{

CNrpWorldConfig::CNrpWorldConfig(void) : INrpConfig( "CNrpWorldConfig", "worlConfig"), SECTION_NAME("options")
{
}

CNrpWorldConfig& CNrpWorldConfig::Instance()
{
	if( !globalTerrainConfigInstance)
	{
		globalTerrainConfigInstance = new CNrpWorldConfig();
		globalTerrainConfigInstance->Load_( "config/world.ini" );
	}

	return *globalTerrainConfigInstance;
}

void CNrpWorldConfig::Load_( char* file_name )
{
	options_[ CONFIG_FILE ] = new std::string( file_name );								//запоминаем путь к файлу настроек
	options_[ WORLD_REAL_SIZE ] = Read_< core::dimension2df >( SECTION_NAME, WORLD_REAL_SIZE, core::dimension2df( 0, 0 ) );
	options_[ WORLD_MINEDGE ] = Read_< core::vector3df >( SECTION_NAME, WORLD_MINEDGE, core::vector3df( 99999, 99999, 99999 ) );
	options_[ WORLD_MAXEDGE ] = Read_< core::vector3df >( SECTION_NAME, WORLD_MAXEDGE, core::vector3df( -99999, -99999, -99999 ) );
	options_[ WORLD_WIDTH_COEFF ] = new float(1);

}

}//namespace nrp