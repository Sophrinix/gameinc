#pragma once
#include "nrpconfig.h"

namespace nrp
{
OPTION_NAME WORLD_REAL_SIZE( L"worldSize" );
OPTION_NAME WORLD_MINEDGE( L"minEdge" );
OPTION_NAME WORLD_MAXEDGE( L"maxEdge" );
OPTION_NAME WORLD_WIDTH_COEFF( L"widthCoeff" );

class CNrpWorldConfig : public INrpConfig
{
private:
	CNrpWorldConfig();
public:
	static CNrpWorldConfig& Instance();

	static NrpText ClassName();
};

}//namespace nrp