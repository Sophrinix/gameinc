#pragma once
#include "nrpConfig.h"

namespace nrp
{
OPTION_NAME REL_VALUE( "rel_value" );
OPTION_NAME REL_TIME( "rel_time" );

class CNrpRelation : public INrpConfig
{		
	CNrpRelation() : INrpConfig( "CNrpRelation", "" ) {};

public:
	CNrpRelation( const NrpText& name, int valuel ) : INrpConfig( "CNrpRelation", "" )
	{
		CreateValue<NrpText>( NAME, name );
		CreateValue<int>( REL_VALUE, valuel );
		SYSTEMTIME time;
		GetLocalTime( &time );
		CreateValue<SYSTEMTIME>( REL_TIME, time );
	}

	static NrpText ClassName();
	~CNrpRelation(void);
};


}// end namespace nrp