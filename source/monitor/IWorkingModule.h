#pragma once
#include "NrpTechnology.h"

namespace nrp
{

class IWorkingModule : public CNrpTechnology
{
public:
	IWorkingModule( PROJECT_TYPE type, const CLASS_NAME className )
		: CNrpTechnology( type, className )
	{
		CNrpTechnology::InitializeOptions_();

		CreateValue<float>( READYWORKPERCENT, 0 );
	}

	virtual void Update( IUser* ptrUser ) = 0;
	virtual int AddUser( IUser* ptrUser ) = 0;
	virtual int RemoveUser( const std::string& userName ) = 0;

	virtual void Save( std::string saveFolder ) {}
	virtual void Load( std::string fileName ) {}

	virtual ~IWorkingModule() {};
private:
	IWorkingModule(void) : CNrpTechnology( PROJECT_TYPE(0), "undefined" ) {};
};

}//end namespace nrp