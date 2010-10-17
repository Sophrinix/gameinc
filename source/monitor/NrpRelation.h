#pragma once
#include "nrpConfig.h"

namespace nrp
{
CLASS_NAME CLASS_NRPRELATION( "CNrpRelation" );

OPTION_NAME REL_VALUE( "rel_value" );
OPTION_NAME REL_TIME( "rel_time" );

class CNrpRelation : public INrpConfig
{		
	void Load_( char * ) {}
	CNrpRelation() : INrpConfig( "CNrpRelation", "" ) {};

public:
	CNrpRelation( const std::string& name, int valuel ) : INrpConfig( "CNrpRelation", "" )
	{
		CreateValue<std::string>( NAME, name );
		CreateValue<int>( REL_VALUE, valuel );
		SYSTEMTIME time;
		GetLocalTime( &time );
		CreateValue<SYSTEMTIME>( REL_TIME, time );
	}

	static std::string ClassName() { return CLASS_NRPRELATION; }
	~CNrpRelation(void);
};


}// end namespace nrp