#include "StdAfx.h"
#include "irrlicht.h"
#include "NrpConsoleConfig.h"

using namespace irr;

static nrp::CNrpConsoleConfig * globalConsoleConfigInstance = 0;

namespace nrp
{

void CNrpConsoleConfig::_InitializeOptions()
{
	CreateValue<int>( CONSOLE_INIT_KEY, '`' );
	CreateValue<core::dimension2df>( CONSOLE_RELATIVE_SIZE, core::dimension2df( 0.9f, 0.6f ) );
	CreateValue<int>( CONSOLE_LINE_SPACING, 2 );
	CreateValue<int>( CONSOLE_INDENT, 1 );
	CreateValue<std::string>( CONSOLE_VERT_ALIGN, "top" );
	CreateValue<std::string>( CONSOLE_HORT_ALIGN, "center" );
	CreateValue<bool>( CONSOLE_SHOW_BACKGROUND, true );
	CreateValue<video::SColor>( CONSOLE_BG_COLOR, video::SColor( 150,10,10,70) );
	CreateValue<video::SColor>( CONSOLE_FONT_COLOR, video::SColor( 200,200,200,200 ) );
	CreateValue<std::string>( CONSOLE_FONTNAME, "font_8" ); 
	CreateValue<std::string>( CONSOLE_PROMT, "nrp" );
	CreateValue<int>( CONSOLE_HISTORY_SIZE, 10 );
	CreateValue<int>( CONSOLE_MAX_BLEND, 128 );
	CreateValue<std::string>( CONSOLE_COMMANDS_FILE, "console.commands" );
	CreateValue<std::string>( CONSOLE_GUINAME, "SystemConsole" );
}

CNrpConsoleConfig::CNrpConsoleConfig(void) : INrpConfig( CLASS_CONSOLECONFIG, CLASS_CONSOLECONFIG )
{
	Load( "config/console.ini" );	
}

CNrpConsoleConfig& CNrpConsoleConfig::Instance()
{
	if( !globalConsoleConfigInstance)
		globalConsoleConfigInstance = new CNrpConsoleConfig();

	return *globalConsoleConfigInstance;
}

}//namespace nrp
