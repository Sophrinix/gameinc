#pragma once
#include "IUser.h"

namespace nrp
{

class NrpTester : public CNrpUser
{
public:
	NrpTester( const NrpText& systemName );
	~NrpTester(void);
	void BeginNewHour( const NrpTime& time );

	void BeginNewDay( const NrpTime& time );

	static NrpText ClassName(); 
};

}//end namespace nrp