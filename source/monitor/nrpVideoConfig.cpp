#include "StdAfx.h"
#include <irrlicht.h>
#include "IniFile.h"
#include "nrpVideoConfig.h"
//////////////////////////////////////////////////////////////////////////

static nrp::CNrpVideoConfig * globalVideoConfigInstance = 0;

using namespace irr;
using namespace core;
using namespace video;

//////////////////////////////////////////////////////////////////////////

namespace nrp
{

CNrpVideoConfig::CNrpVideoConfig() : INrpConfig( "CNrpVideoConfig", "videoConfig"), 
									 SECTION_NAME("options")							//������ ������ ��� ����� �����
{		
	//DbgLog(conf) << con << "OptionsVideo object created" << term;
}
//////////////////////////////////////////////////////////////////////////

CNrpVideoConfig& CNrpVideoConfig::Instance()
{
	if( !globalVideoConfigInstance)
	{
		globalVideoConfigInstance = new CNrpVideoConfig();
		globalVideoConfigInstance->Load_( "config/video.ini" );
	}

	return *globalVideoConfigInstance;
}

void CNrpVideoConfig::Load_( char* file_name )
{
	options_[ CONFIG_FILE ] = (LPVOID)(new std::string( file_name ) );								//���������� ���� � ����� ��������
	options_[ SCREEN_SIZE ] = Read_< dimension2du >( SECTION_NAME, SCREEN_SIZE, dimension2du( 1024, 768 ) );//������ ������

	options_[ FULLSCREEN  ] = Read_< bool >( SECTION_NAME, FULLSCREEN,	false );		//������������� �����
	options_[ USE_SHADOWS ] = Read_< bool >( SECTION_NAME, USE_SHADOWS,	false );		//����������� �����
	options_[ VSYNC ]		= Read_< bool >( SECTION_NAME, VSYNC,		false );	//������������ ����.
}
//////////////////////////////////////////////////////////////////////////

}//namespace nrp