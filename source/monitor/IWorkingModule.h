#pragma once
#include "NrpTechnology.h"

namespace nrp
{

class IWorkingModule : public CNrpTechnology
{
public:
	IWorkingModule( PROJECT_TYPE type, CLASS_NAME className )
		: CNrpTechnology( type, className )
	{
		CNrpTechnology::_InitializeOptions();

		Push<float>( READYWORKPERCENT, 0 );
	}

	virtual void Update( IUser* ptrUser ) = 0;
	virtual int AddUser( IUser* ptrUser ) = 0;
	virtual int RemoveUser( const NrpText& userName ) = 0;

	virtual NrpText Save( const NrpText& saveFolder ) { return ""; }
	virtual void Load( const NrpText& fileName ) {}

	virtual ~IWorkingModule() {};
private:
	IWorkingModule(void) : CNrpTechnology( PROJECT_TYPE(0), "undefined" ) {};
};

}//end namespace nrp