#pragma once
//////////////////////////////////////////////////////////////////////////
#include "nrpConfig.h"

#include <vector>

namespace nrp
{
OPTION_NAME FONT_TOOLTIP( L"fontTooltip" );
OPTION_NAME FONT_SIMPLE( L"fontSimple" );

class CNrpHUDConfig : public INrpConfig
{
private:
	CNrpHUDConfig();

    void Load( const NrpText& fileName );
public:
	static CNrpHUDConfig& Instance();

	static NrpText ClassName();
};

}//namespace nrp