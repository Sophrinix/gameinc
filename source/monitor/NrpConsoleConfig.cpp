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
	options_[ CONFIG_FILE ] = (LPVOID)(new std::string( file_name ) );								//запоминаем путь к файлу настроек
	options_[ CONSOLE_INIT_KEY ] = Read_<int>( SECTION_NAME, CONSOLE_INIT_KEY, '`' );
	options_[ CONSOLE_RELATIVE_SIZE ] = Read_<core::dimension2df>( SECTION_NAME, CONSOLE_RELATIVE_SIZE, core::dimension2df( 1.f, 0.6f ) );
	options_[ CONSOLE_LINE_SPACING ] = Read_<int>( SECTION_NAME, CONSOLE_LINE_SPACING, 2 );
	options_[ CONSOLE_INDENT ] = Read_< int >( SECTION_NAME, CONSOLE_INDENT, 1 );
	options_[ CONSOLE_VERT_ALIGN ] =  Read_<std::string>( SECTION_NAME, CONSOLE_VERT_ALIGN, "top" );
	options_[ CONSOLE_HORT_ALIGN ] = Read_<std::string>( SECTION_NAME, CONSOLE_HORT_ALIGN, "center" );
	options_[ CONSOLE_SHOW_BACKGROUND ] = Read_<bool>(SECTION_NAME, CONSOLE_SHOW_BACKGROUND, true );
	options_[ CONSOLE_BG_COLOR ] = (LPVOID)new video::SColor(150,10,10,70);
	options_[ CONSOLE_FONT_COLOR ] = (LPVOID)new video::SColor(200,200,200,200);
	options_[ CONSOLE_FONTNAME ] = Read_< std::string >( SECTION_NAME, CONSOLE_FONTNAME, "font_8" ); 
	options_[ CONSOLE_PROMT ] = Read_< std::string >( SECTION_NAME, CONSOLE_PROMT, "nrp" );
	options_[ CONSOLE_HISTORY_SIZE ] = Read_< int >( SECTION_NAME, CONSOLE_HISTORY_SIZE, 10 );
	options_[ CONSOLE_MAX_BLEND ] = Read_< int >( SECTION_NAME, CONSOLE_MAX_BLEND, 128 );
	options_[ CONSOLE_COMMANDS_FILE ] = Read_< std::string >( SECTION_NAME, CONSOLE_PROMT, "console.commands" );
	options_[ CONSOLE_GUINAME ] = Read_<std::string>( SECTION_NAME, CONSOLE_GUINAME, "SystemConsole" );
}

}//namespace nrp
