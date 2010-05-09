#pragma once
#include <string>

namespace nrp
{

class IUser;

class IModificator
{
public:
	virtual std::string& GetName() = 0;
	virtual SYSTEMTIME& GetTime() = 0;
};

template< class ValClass > class CNrpUserModificator : public IModificator
{
public:
	CNrpUserModificator( IUser* ptrUser, SYSTEMTIME time, 
						 std::string paramName, bool absolute, 
						 ValClass offset )
	{
		offset_ = offset;
		endTime_ = time;
		ptrUser_ = ptrUser;
		paramName_ = paramName;
		absolute_ = absolute;
	}

	std::string& GetName() { return paramName_; }
	ValClass& GetValue() { return offset_; }
	SYSTEMTIME& GetTime() { return endTime_; }

private:
	ValClass offset_;
	SYSTEMTIME endTime_;
	IUser* ptrUser_;
	std::string paramName_;
	bool absolute_;
};

}//end of namespace nrp