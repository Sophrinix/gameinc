#pragma once
#include "nrpconfig.h"

namespace nrp
{
OPTION_NAME CONSOLE_INIT_KEY( L"initKey" );
OPTION_NAME CONSOLE_RELATIVE_SIZE( L"relativeSize" );
OPTION_NAME CONSOLE_LINE_SPACING( L"lineSpacing" );
OPTION_NAME CONSOLE_INDENT( L"indent" );
OPTION_NAME CONSOLE_VERT_ALIGN( L"vertAlign" ); //top/bottom/middle
OPTION_NAME CONSOLE_HORT_ALIGN( L"hortAlign" ); //left/rigth/center
OPTION_NAME CONSOLE_SHOW_BACKGROUND( L"showBackground" );
OPTION_NAME CONSOLE_BG_COLOR( L"colorBackground" );
OPTION_NAME CONSOLE_FONT_COLOR( L"colorFont" );
OPTION_NAME CONSOLE_FONTNAME( L"fontName" );
OPTION_NAME CONSOLE_PROMT( L"promt" );
OPTION_NAME CONSOLE_HISTORY_SIZE( L"historySize" );
OPTION_NAME CONSOLE_COMMANDS_FILE( L"saveFile" );
OPTION_NAME CONSOLE_MAX_BLEND( L"maxBlend" );
OPTION_NAME CONSOLE_GUINAME( L"guiname" );

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
