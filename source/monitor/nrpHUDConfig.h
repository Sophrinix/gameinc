#pragma once
//////////////////////////////////////////////////////////////////////////
#include "nrpConfig.h"

#include <vector>
#include <string>
//////////////////////////////////////////////////////////////////////////

namespace nrp
{

const std::string MIN_FONT_SIZE( "minFontSize" );
const std::string MAX_FONT_SIZE( "maxFontSize" );
const std::string FONT_8_INCH( "font_8" );
const std::string FONT_9_INCH( "font_9" );
const std::string FONT_10_INCH( "font_10" );
const std::string FONT_11_INCH( "font_11" );
const std::string FONT_12_INCH( "font_12" );
const std::string FONT_13_INCH( "font_13" );
const std::string FONT_14_INCH( "font_14" );
const std::string FONT_15_INCH( "font_15" );
const std::string FONT_16_INCH( "font_16" );
const std::string FONT_17_INCH( "font_17" );
const std::string FONT_18_INCH( "font_18" );
const std::string FONT_19_INCH( "font_19" );
const std::string FONT_20_INCH( "font_20" );
const std::string FONT_21_INCH( "font_21" );
const std::string FONT_22_INCH( "font_22" );
const std::string FONT_23_INCH( "font_23" );
const std::string FONT_24_INCH( "font_24" );

const std::string FONT_TOOLTIP( "font_8" );
const std::string FONT_SIMPLE( "font_12" );

class CNrpHUDConfig : public INrpConfig
{
private:
	CNrpHUDConfig();

	void Load_( char* file_name );
	const std::string SECTION_NAME;
public:
	static CNrpHUDConfig& Instance();
};

}//namespace nrp