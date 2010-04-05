#pragma once
#include "nrpConfig.h"
#include "LuaFunctionality.h"
#include <vector>

namespace nrp
{

OPTION_NAME BANK( "bank" );
OPTION_NAME TECHNUMBER( "techNumber" );
OPTION_NAME EMPLOYERNUMBER( "employerNumber" );

class CNrpCompany;
class IUser;
class INrpProject;
class CNrpGameEngine;
class CNrpTechnology;
	
class CNrpApplication : public INrpConfig, public ILuaFunctionality
{
	typedef std::vector< CNrpCompany* > COMPANY_LIST;
	typedef std::vector< IUser* > USER_LIST;
	typedef std::vector< CNrpTechnology* > TECH_LIST;
public:
	typedef enum { SPD_MINUTE=0, SPD_HOUR, SPD_DAY, SPD_MONTH, SPD_COUNT } SPEED;
	static CNrpApplication& Instance();

	int AddCompany( CNrpCompany* company );
	int AddUser( bool player, IUser* user );

	nrp::INrpProject* CreateGameProject( std::string name );
	nrp::CNrpGameEngine* CreateGameEngine( std::string name );
	nrp::CNrpTechnology* CreateTechnology( int typeTech );
	bool UpdateTime();

	SYSTEMTIME& GetDateTime() { return time_; }
	COMPANY_LIST& GetCompanies();
	CNrpCompany* GetCompany( std::string companyName ) const;

	int GetTechsNumber() const { return technologies_.size(); }
	CNrpTechnology* GetTechnology( int index ) const;
	void AddTechnology( CNrpTechnology* ptrTech );

	CNrpCompany* GetPlayerCompany();
private:
	CNrpApplication(void);
	~CNrpApplication(void);

	COMPANY_LIST companies_;
	USER_LIST employers_;
	USER_LIST ceo_;
	TECH_LIST technologies_;

	SPEED speed_;
	SYSTEMTIME time_;
	int lastTimeUpdate_;

	virtual void Load_( char* fileName ) {}
};

}//namespace nrp