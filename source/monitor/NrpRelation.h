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
		Add<NrpText>( NAME, name );
		Add<int>( REL_VALUE, valuel );
		Add<NrpTime>( REL_TIME, NrpTime::FromLocalTime() );
	}

	static NrpText ClassName();
	~CNrpRelation(void);
};


}// end namespace nrp