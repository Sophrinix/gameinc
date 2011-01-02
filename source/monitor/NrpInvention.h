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
	CNrpInvention( CNrpTechnology* pTech, CNrpCompany* pCmp );
	CNrpInvention( const NrpText& fileName );
	virtual ~CNrpInvention(void);

	bool Equale( const NrpText& name, const NrpText& company );

	virtual void Update( IUser* ptrUser );
	void BeginNewMonth( const SYSTEMTIME& time );
	void CheckParams();
	IUser* GetUser( u32 index );
	int AddUser( IUser* user );
	int RemoveUser( const NrpText& userName );

	NrpText Save( const NrpText& saveFolder, bool k );
	void Load( const NrpText& fileName );

	static NrpText ClassName();
private:
	int _GetRealPrice();
	
	void InitializeOptions_();
	NrpText _GetFileName();

	USERS _users;
};

}//end namespace nrp