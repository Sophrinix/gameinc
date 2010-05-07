#pragma once
#include "nrpConfig.h"
#include "LuaFunctionality.h"
#include <vector>

namespace nrp
{

OPTION_NAME BANK( "bank" );
OPTION_NAME PROFILENAME( "profileName" );
OPTION_NAME PROFILECOMPANY( "profileCompany" );
OPTION_NAME FULLPATH( "fullPath" );
OPTION_NAME CURRENTTIME( "currentTime" );

class CNrpCompany;
class IUser;
class INrpProject;
class CNrpGameEngine;
class CNrpTechnology;
class CNrpGame;
	
class CNrpApplication : public INrpConfig, public ILuaFunctionality
{
	typedef std::vector< IUser* > USER_LIST;
	typedef std::vector< CNrpTechnology* > TECH_LIST;
public:
	typedef std::vector< CNrpCompany* > COMPANIES_LIST;
	typedef enum { SPD_MINUTE=0, SPD_HOUR, SPD_DAY, SPD_MONTH, SPD_COUNT } SPEED;
	static CNrpApplication& Instance();

	void ResetData();

	void SaveProfile();
	void LoadProfile( std::string profileName, std::string companyName );
	void CreateProfile( std::string profileName, std::string companyName );

	COMPANIES_LIST& GetCompanies();
	CNrpCompany* GetCompany( std::string companyName ) const;
	int AddCompany( CNrpCompany* company );
	CNrpCompany* GetPlayerCompany();

	int AddUser( bool player, IUser* user );
	int RemoveUser( IUser* user );
	void UpdateUsers();
	IUser* GetUser( int index );
	IUser* GetUser( std::string name );
	IUser* GetUser( std::string company, std::string name );

	nrp::INrpProject* CreateGameProject( std::string name );

	nrp::CNrpGameEngine* CreateGameEngine( std::string name );
	CNrpGameEngine* GetGameEngine( std::string name );

	bool UpdateTime();
	SYSTEMTIME& GetDateTime() { return GetValue<SYSTEMTIME>( CURRENTTIME ); }

	int GetTechsNumber() const { return technologies_.size(); }
	CNrpTechnology* GetTechnology( int index ) const;
	CNrpTechnology* GetTechnology( const std::string& name ) const;
	CNrpTechnology* GetTechnology( CNrpCompany* ptrCompany, const std::string& name );

	void AddTechnology( CNrpTechnology* ptrTech );
	void UpdateGameRatings( CNrpGame* ptrGame, bool firstTime=false );
	nrp::CNrpTechnology* CreateTechnology( int typeTech );

private:
	CNrpApplication(void);
	~CNrpApplication(void);

	COMPANIES_LIST companies_;
	USER_LIST users_;
	TECH_LIST technologies_;

	SPEED speed_;
	int lastTimeUpdate_;

	void Load_( char* fileName ) {}
	void UpdateGameState_();
	void PayCompanySalaries_();
	IUser* CreateRandomUser_( std::string userType );
	int GetGameRating_( CNrpGame* ptrGame, GAME_RATING_TYPE typeRating );
};

}//namespace nrp