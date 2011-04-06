#pragma once
#include "IWorkingModule.h"
#include "nrpArrays.h"

namespace nrp
{

OPTION_NAME REALPRICE( "realprice" );
OPTION_NAME PASSEDPRICE( "passedprice" );
OPTION_NAME INVESTIMENT( "investiment" );
OPTION_NAME INVENTIONSPEED( "inventionspeed" );
OPTION_NAME PROGNOSEDATEFINISH( "prognosedatefinish" );
OPTION_NAME DAYLEFT( "dayleft" );
OPTION_NAME USERSTARTDATE( "userStartDate" );
OPTION_NAME MONEY_TODECREASE( "moneyToDecrease" );

class CNrpInvention : public IWorkingModule
{
public:
	CNrpInvention( CNrpTechnology* pTech, CNrpCompany* pCmp, NrpTime time );
	CNrpInvention( const NrpText& fileName );
	virtual ~CNrpInvention(void);

	bool Equale( const NrpText& name, const NrpText& company );

	virtual void Update( CNrpUser* ptrUser, const NrpTime& time );
	void BeginNewMonth( const NrpTime& time );
	void CheckParams( NrpTime time );
	CNrpUser* GetUser( u32 index );
	int AddUser( CNrpUser* user );
	int RemoveUser( const NrpText& userName );

	NrpText Save( const NrpText& saveFolder, bool k );
	void Load( const NrpText& fileName );

	static NrpText ClassName();
private:
	int _GetRealPrice( const NrpTime& time );
	
	void InitializeOptions_();
	NrpText _GetFileName();

	USERS _users;
};

}//end namespace nrp