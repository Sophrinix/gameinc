#pragma once
#include "IWorkingModule.h"

namespace nrp
{

CLASS_NAME CLASS_INVENTION( "CNrpInvention" );

OPTION_NAME SECTIONS_USERS( "users" );

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
	CNrpInvention( const std::string& fileName );
	virtual ~CNrpInvention(void);

	virtual void Update( IUser* ptrUser );
	void CheckParams();
	IUser* GetUser( size_t index );
	int AddUser( IUser* user );
	int RemoveUser( const std::string& userName );

	std::string Save( const std::string& saveFolder, bool k );
	void Load( std::string fileName );

	static std::string ClassName() { return CLASS_INVENTION; }
private:
	int _GetRealPrice( CNrpTechnology* tech );
	
	//deperecated 
	void Save( std::string saveFolder ) {};
	//

	void InitializeOptions_();
	std::string _GetFileName();
	typedef std::vector< IUser* > USERS_LIST;
	USERS_LIST _users;
};

}//end namespace nrp