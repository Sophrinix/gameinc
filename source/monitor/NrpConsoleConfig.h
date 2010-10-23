#pragma once
#include "nrpconfig.h"

namespace nrp
{

CLASS_NAME CLASS_CONSOLECONFIG( "CNrpConsoleConfig" );

const std::string CONSOLE_INIT_KEY( "initKey" );
const std::string CONSOLE_RELATIVE_SIZE( "relativeSize" );
const std::string CONSOLE_LINE_SPACING( "lineSpacing" );
const std::string CONSOLE_INDENT( "indent" );
const std::string CONSOLE_VERT_ALIGN( "vertAlign" ); //top/bottom/middle
const std::string CONSOLE_HORT_ALIGN( "hortAlign" ); //left/rigth/center
const std::string CONSOLE_SHOW_BACKGROUND( "showBackground" );
const std::string CONSOLE_BG_COLOR( "colorBackground" );
const std::string CONSOLE_FONT_COLOR( "colorFont" );
const std::string CONSOLE_FONTNAME( "fontName" );
const std::string CONSOLE_PROMT( "promt" );
const std::string CONSOLE_HISTORY_SIZE( "historySize" );
const std::string CONSOLE_COMMANDS_FILE( "saveFile" );
const std::string CONSOLE_MAX_BLEND( "maxBlend" );
const std::string CONSOLE_GUINAME( "guiname" );

class CNrpConsoleConfig : public INrpConfig
{
private:
	CNrpConsoleConfig();
	
	void _InitializeOptions();
public:
	static CNrpConsoleConfig& Instance();
	static std::string ClassName() { return CLASS_CONSOLECONFIG; }	
};

} //namespace nrp
