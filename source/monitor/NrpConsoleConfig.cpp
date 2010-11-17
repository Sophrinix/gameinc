#include "StdAfx.h"
#include "irrlicht.h"
#include "NrpConsoleConfig.h"

using namespace irr;

static nrp::CNrpConsoleConfig * globalConsoleConfigInstance = 0;

namespace nrp
{
CLASS_NAME CLASS_CONSOLECONFIG( L"CNrpConsoleConfig" );

void CNrpConsoleConfig::_InitializeOptions()
{
	Push<int>( CONSOLE_INIT_KEY, '`' );
	Push<core::dimension2df>( CONSOLE_RELATIVE_SIZE, core::dimension2df( 0.9f, 0.6f ) );
	Push<int>( CONSOLE_LINE_SPACING, 2 );
	Push<int>( CONSOLE_INDENT, 1 );
	Push<NrpText>( CONSOLE_VERT_ALIGN, "top" );
	Push<NrpText>( CONSOLE_HORT_ALIGN, "center" );
	Push<bool>( CONSOLE_SHOW_BACKGROUND, true );
	Push<video::SColor>( CONSOLE_BG_COLOR, video::SColor( 150,10,10,70) );
	Push<video::SColor>( CONSOLE_FONT_COLOR, video::SColor( 200,200,200,200 ) );
	Push<NrpText>( CONSOLE_FONTNAME, "font_8" ); 
	Push<NrpText>( CONSOLE_PROMT, "nrp" );
	Push<int>( CONSOLE_HISTORY_SIZE, 10 );
	Push<int>( CONSOLE_MAX_BLEND, 128 );
	Push<NrpText>( CONSOLE_COMMANDS_FILE, "console.commands" );
	Push<NrpText>( CONSOLE_GUINAME, "SystemConsole" );
}

CNrpConsoleConfig::CNrpConsoleConfig(void) : INrpConfig( CLASS_CONSOLECONFIG, CLASS_CONSOLECONFIG )
{
	_InitializeOptions();

	Load( "config/console.ini" );	
}

CNrpConsoleConfig& CNrpConsoleConfig::Instance()
{
	if( !globalConsoleConfigInstance)
		globalConsoleConfigInstance = new CNrpConsoleConfig();

	return *globalConsoleConfigInstance;
}

NrpText CNrpConsoleConfig::ClassName()
{
	return CLASS_CONSOLECONFIG;
}
}//namespace nrp
