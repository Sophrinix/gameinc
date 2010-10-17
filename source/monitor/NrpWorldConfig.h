#pragma once
#include <string>
#include "nrpconfig.h"

namespace nrp
{

CLASS_NAME CLASS_WORLDCONFIG( "CNrpWorldConfig" );

const std::string WORLD_REAL_SIZE( "worldSize" );
const std::string WORLD_MINEDGE( "minEdge" );
const std::string WORLD_MAXEDGE( "maxEdge" );
const std::string WORLD_WIDTH_COEFF( "widthCoeff" );

class CNrpWorldConfig : public INrpConfig
{
private:
	CNrpWorldConfig();

	void Load_( char* file_name );
	const std::string SECTION_NAME;
public:
	static CNrpWorldConfig& Instance();

	virtual std::string ObjectName() { return CLASS_WORLDCONFIG; }
	static std::string ClassName() { return CLASS_WORLDCONFIG; }
};

}//namespace nrp