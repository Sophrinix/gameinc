#pragma once
#include "NrpTechnology.h"
#include "timeHelpers.h"

namespace nrp
{

OPTION_NAME PARAMNAME("paramName");
OPTION_NAME DURATION("duration");
OPTION_NAME OFFSET( "offset" );
OPTION_NAME LASTTIMEUPDATE( "lastTimeUpdate" );

class IWorkingModule : public CNrpTechnology
{
public:
	IWorkingModule( PROJECT_TYPE type, CLASS_NAME className )
		: CNrpTechnology( type, className )
	{
		CNrpTechnology::_InitializeOptions();

		Add<float>( READYWORKPERCENT, 0 );
	}

	virtual void Update( CNrpUser* ptrUser, const NrpTime& time ) = 0;
	virtual int AddUser( CNrpUser* ptrUser ) = 0;
	virtual int RemoveUser( const NrpText& userName ) = 0;

	virtual NrpText Save( const NrpText& saveFolder ) { return ""; }
	virtual void Load( const NrpText& fileName ) {}

	virtual ~IWorkingModule() {};
private:
	IWorkingModule(void) : CNrpTechnology( PROJECT_TYPE(0), "undefined" ) {};
};

}//end namespace nrp