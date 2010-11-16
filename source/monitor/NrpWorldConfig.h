#pragma once
#include "nrpconfig.h"

namespace nrp
{
OPTION_NAME WORLD_REAL_SIZE( "worldSize" );
OPTION_NAME WORLD_MINEDGE( "minEdge" );
OPTION_NAME WORLD_MAXEDGE( "maxEdge" );
OPTION_NAME WORLD_WIDTH_COEFF( "widthCoeff" );

class CNrpWorldConfig : public INrpConfig
{
private:
	CNrpWorldConfig();
public:
	static CNrpWorldConfig& Instance();

	static NrpText ClassName();
};

}//namespace nrp