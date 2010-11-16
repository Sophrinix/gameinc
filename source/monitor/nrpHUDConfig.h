#pragma once
//////////////////////////////////////////////////////////////////////////
#include "nrpConfig.h"

#include <vector>

namespace nrp
{
OPTION_NAME MIN_FONT_SIZE( "minFontSize" );
OPTION_NAME MAX_FONT_SIZE( "maxFontSize" );
OPTION_NAME FONT_8_INCH( "font_8" );
OPTION_NAME FONT_9_INCH( "font_9" );
OPTION_NAME FONT_10_INCH( "font_10" );
OPTION_NAME FONT_11_INCH( "font_11" );
OPTION_NAME FONT_12_INCH( "font_12" );
OPTION_NAME FONT_13_INCH( "font_13" );
OPTION_NAME FONT_14_INCH( "font_14" );
OPTION_NAME FONT_15_INCH( "font_15" );
OPTION_NAME FONT_16_INCH( "font_16" );
OPTION_NAME FONT_17_INCH( "font_17" );
OPTION_NAME FONT_18_INCH( "font_18" );
OPTION_NAME FONT_19_INCH( "font_19" );
OPTION_NAME FONT_20_INCH( "font_20" );
OPTION_NAME FONT_21_INCH( "font_21" );
OPTION_NAME FONT_22_INCH( "font_22" );
OPTION_NAME FONT_23_INCH( "font_23" );
OPTION_NAME FONT_24_INCH( "font_24" );

OPTION_NAME FONT_TOOLTIP( "font_8" );
OPTION_NAME FONT_SIMPLE( "font_12" );

class CNrpHUDConfig : public INrpConfig
{
private:
	CNrpHUDConfig();

public:
	static CNrpHUDConfig& Instance();

	static NrpText ClassName();
};

}//namespace nrp