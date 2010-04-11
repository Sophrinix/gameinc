#include "StdAfx.h"
#include "NrpHtmlEngineConfig.h"

static nrp::CNrpHtmlEngineConfig * globalHtmlConfigInstance = 0;

namespace nrp
{

CNrpHtmlEngineConfig::CNrpHtmlEngineConfig(void) : INrpConfig( "CNrpHtmlEngineConfig", "htmlConfig" ), SECTION_NAME( "options" )
{
}

CNrpHtmlEngineConfig& CNrpHtmlEngineConfig::Instance()
{
	if( !globalHtmlConfigInstance)
	{
		globalHtmlConfigInstance = new CNrpHtmlEngineConfig();
		globalHtmlConfigInstance->Load_( "config/html.ini" );
	}

	return *globalHtmlConfigInstance;
}

void CNrpHtmlEngineConfig::Load_( char* file_name )
{
	options_[ CONFIG_FILE ] = (LPVOID)(new std::string( file_name ) );								//���������� ���� � ����� ��������
}
}//namespace nrp