#pragma once
#include "nrpconfig.h"

namespace nrp
{
OPTION_NAME CONSOLE_INIT_KEY( "initKey" );
OPTION_NAME CONSOLE_RELATIVE_SIZE( "relativeSize" );
OPTION_NAME CONSOLE_LINE_SPACING( "lineSpacing" );
OPTION_NAME CONSOLE_INDENT( "indent" );
OPTION_NAME CONSOLE_VERT_ALIGN( "vertAlign" ); //top/bottom/middle
OPTION_NAME CONSOLE_HORT_ALIGN( "hortAlign" ); //left/rigth/center
OPTION_NAME CONSOLE_SHOW_BACKGROUND( "showBackground" );
OPTION_NAME CONSOLE_BG_COLOR( "colorBackground" );
OPTION_NAME CONSOLE_FONT_COLOR( "colorFont" );
OPTION_NAME CONSOLE_FONTNAME( "fontName" );
OPTION_NAME CONSOLE_PROMT( "promt" );
OPTION_NAME CONSOLE_HISTORY_SIZE( "historySize" );
OPTION_NAME CONSOLE_COMMANDS_FILE( "saveFile" );
OPTION_NAME CONSOLE_MAX_BLEND( "maxBlend" );
OPTION_NAME CONSOLE_GUINAME( "guiname" );

class CNrpConsoleConfig : public INrpConfig
{
private:
	CNrpConsoleConfig();
	
	void _InitializeOptions();
public:
	static CNrpConsoleConfig& Instance();
	static NrpText ClassName();	
};

} //namespace nrp
