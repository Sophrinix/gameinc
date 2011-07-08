#pragma once
#include "nrpConfig.h"
#include "NrpTime.h"

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
		RegProperty<NrpText>( NAME, name );
		RegProperty<float>( REL_VALUE, valuel );
		RegProperty<NrpTime>( REL_TIME, NrpTime::FromLocalTime() );
	}

	static NrpText ClassName();
	~CNrpRelation(void);
};


}// end namespace nrp