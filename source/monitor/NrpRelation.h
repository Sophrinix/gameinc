#pragma once
#include "nrpConfig.h"
#include "timeHelpers.h"

namespace nrp
{
OPTION_NAME REL_VALUE( "value" );
OPTION_NAME REL_TIME( "time" );

class CNrpRelation : public INrpConfig
{		
	CNrpRelation() : INrpConfig( "CNrpRelation", "" ) {};

public:
	CNrpRelation( const NrpText& name, float valuel ) : INrpConfig( "CNrpRelation", "" )
	{
		Add<NrpText>( NAME, name );
		Add<float>( REL_VALUE, valuel );
		Add<NrpTime>( REL_TIME, NrpTime::FromLocalTime() );
	}

	static NrpText ClassName();
	~CNrpRelation(void);
};


}// end namespace nrp