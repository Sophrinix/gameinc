#pragma once
#include "timeHelpers.h"
#include "NrpText.h"

namespace nrp
{

class IUser;

class IModificator
{
public:
	virtual NrpText& GetName() = 0;
	virtual NrpTime& GetTime() = 0;
};

template< class ValClass > 
class CNrpUserModificator : public IModificator
{
public:
	CNrpUserModificator( IUser* ptrUser, NrpTime time, 
						 NrpText paramName, bool absolute, 
						 ValClass offset ) : endTime_( 0. )
	{
		offset_ = offset;
		endTime_ = time;
		ptrUser_ = ptrUser;
		paramName_ = paramName;
		absolute_ = absolute;
	}

	NrpText& GetName() { return paramName_; }
	ValClass& GetValue() { return offset_; }
	NrpTime& GetTime() { return endTime_; }

private:
	ValClass offset_;
	NrpTime endTime_;
	IUser* ptrUser_;
	NrpText paramName_;
	bool absolute_;
};

}//end of namespace nrp