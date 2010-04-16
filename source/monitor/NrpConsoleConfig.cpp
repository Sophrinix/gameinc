#include "StdAfx.h"
#include "irrlicht.h"
#include "NrpConsoleConfig.h"

using namespace irr;

static nrp::CNrpConsoleConfig * globalConsoleConfigInstance = 0;

namespace nrp
{

CNrpConsoleConfig::CNrpConsoleConfig(void) : INrpConfig( "CNrpConsoleConfig", "consoleConfig" ), 
											 SECTION_NAME( "Options" )
{
}

CNrpConsoleConfig& CNrpConsoleConfig::Instance()
{
	if( !globalConsoleConfigInstance)
	{
		globalConsoleConfigInstance = new CNrpConsoleConfig();
		globalConsoleConfigInstance->Load_( "config/console.ini" );
	}

	return *globalConsoleConfigInstance;
}

void CNrpConsoleConfig::Load_( char* file_name )
{
	CreateValue<std::string>( CONFIG_FILE, file_name );								//запоминаем путь к файлу настроек
	CreateValue<int>( CONSOLE_INIT_KEY, Read_<int>( SECTION_NAME, CONSOLE_INIT_KEY, '`' ) );
	CreateValue<core::dimension2df>( CONSOLE_RELATIVE_SIZE, Read_<core::dimension2df>( SECTION_NAME, CONSOLE_RELATIVE_SIZE, core::dimension2df( 1.f, 0.6f ) ) );
	CreateValue<int>( CONSOLE_LINE_SPACING, Read_<int>( SECTION_NAME, CONSOLE_LINE_SPACING, 2 ) );
	CreateValue<int>( CONSOLE_INDENT, Read_< int >( SECTION_NAME, CONSOLE_INDENT, 1 ) );
	CreateValue<std::string>( CONSOLE_VERT_ALIGN, Read_<std::string>( SECTION_NAME, CONSOLE_VERT_ALIGN, "top" ) );
	CreateValue<std::string>( CONSOLE_HORT_ALIGN, Read_<std::string>( SECTION_NAME, CONSOLE_HORT_ALIGN, "center" ) );
	CreateValue<bool>( CONSOLE_SHOW_BACKGROUND, Read_<bool>(SECTION_NAME, CONSOLE_SHOW_BACKGROUND, true ) );
	CreateValue<video::SColor>( CONSOLE_BG_COLOR, video::SColor( 150,10,10,70) );
	CreateValue<video::SColor>( CONSOLE_FONT_COLOR, video::SColor( 200,200,200,200 ) );
	CreateValue<std::string>( CONSOLE_FONTNAME, Read_< std::string >( SECTION_NAME, CONSOLE_FONTNAME, "font_8" ) ); 
	CreateValue<std::string>( CONSOLE_PROMT, Read_< std::string >( SECTION_NAME, CONSOLE_PROMT, "nrp" ) );
	CreateValue<int>( CONSOLE_HISTORY_SIZE, Read_< int >( SECTION_NAME, CONSOLE_HISTORY_SIZE, 10 ) );
	CreateValue<int>( CONSOLE_MAX_BLEND, Read_< int >( SECTION_NAME, CONSOLE_MAX_BLEND, 128 ) );
	CreateValue<std::string>( CONSOLE_COMMANDS_FILE, Read_< std::string >( SECTION_NAME, CONSOLE_PROMT, "console.commands" ) );
	CreateValue<std::string>( CONSOLE_GUINAME, Read_<std::string>( SECTION_NAME, CONSOLE_GUINAME, "SystemConsole" ) );
}

}//namespace nrp
