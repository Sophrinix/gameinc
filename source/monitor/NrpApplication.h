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
OPTION_NAME DISKMACHINENUMBER( "diskMachineNumber" );
OPTION_NAME BOXADDONNUMBER( "boxAdonNumber" );
OPTION_NAME MARKETGAMENUMBER( "marketGameNumber" );
OPTION_NAME PLAYERCOMPANY( "playerCompany" );

class CNrpCompany;
class IUser;
class INrpProject;
class CNrpGameEngine;
class CNrpTechnology;
class CNrpGame;
class CNrpDiskMachine;
class CNrpRetailer;
class CNrpGameImageList;
class CNrpGameEngine;
class CNrpInvention;
	
class CNrpApplication : public INrpConfig, public ILuaFunctionality
{
	typedef std::vector< IUser* > USER_LIST;
	typedef std::vector< CNrpTechnology* > TECH_LIST;
	typedef std::vector< CNrpDiskMachine* > DISKMACHINES_LIST;
	typedef std::vector< CNrpGame* > GAMES_LIST;
	typedef std::vector< CNrpRetailer* > RETAILER_LIST;
	typedef std::vector< CNrpInvention* > INVENTION_LIST;

	typedef std::map< std::string, CNrpGameImageList* > GAMEIMAGES_MAP; 
	typedef std::map< std::string, CNrpGameEngine* > GAMEENGINES_MAP;
	typedef std::map< std::string, INrpProject* > PROJECTS_MAP;
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
	CNrpCompany* GetCompany( int index ) const;
	int AddCompany( CNrpCompany* company );

	int AddUser( bool player, IUser* user );
	int RemoveUser( IUser* user );
	void CreateNewFreeUsers();
	IUser* GetUser( int index );
	IUser* GetUser( std::string name );

	void AddDevelopProject( INrpProject* project );
	void RemoveDevelopProject( const std::string& name );
	INrpProject* GetDevelopProject( const std::string& name ) const; 

	void AddGameEngine( nrp::CNrpGameEngine* ptrEngine );
	CNrpGameEngine* GetGameEngine( std::string name );
	void RemoveGameEngine( nrp::CNrpGameEngine* ptrEngine );

	bool UpdateTime();

	CNrpGame* GetGame( const std::string& name );
	CNrpGame* GetGame( size_t index );
	void AddGame( CNrpGame* ptrGame );

	void AddProject( nrp::INrpProject* project );
	INrpProject* GetProject( const std::string& name );

	int GetTechsNumber() const { return technologies_.size(); }
	CNrpTechnology* GetTechnology( int index ) const;
	CNrpTechnology* GetTechnology( const std::string& name ) const;

	void AddTechnology( CNrpTechnology* ptrTech );
	void RemoveTechnology( CNrpTechnology* ptrTech );

	void UpdateGameRatings( CNrpGame* ptrGame, bool firstTime=false );

	CNrpTechnology* GetBoxAddon( size_t index ) { return index < boxAddons_.size() ? boxAddons_[ index ] : NULL; }
	CNrpTechnology* GetBoxAddon( std::string name );
	void AddBoxAddon( CNrpTechnology* tech );

	void AddGameToMarket( CNrpGame* game );

	CNrpDiskMachine* GetDiskMachine( std::string name );
	CNrpDiskMachine* GetDiskMachine( size_t index );
	void AddDiskMachine( CNrpDiskMachine* pDm );

	CNrpRetailer* GetRetailer( std::string name );
	void RemoveRetailer( std::string name );

	std::string GetFreeInternalName( CNrpGame* game );
	CNrpGameImageList* GetGameImageList( std::string name );
	void AddGameImageList( CNrpGameImageList* pGList );
	void ClearImageList();
	float GetGameGenreInterest( CNrpGame* game );

	void StartInvention( CNrpTechnology* startTech, CNrpCompany* parentCompany );
	void InventionFinished( CNrpInvention* ptrInvention );
	void InventionCanceled( CNrpInvention* ptrInvention );
	CNrpInvention* GetInvention( std::string name, std::string companyName );

private:
	CNrpApplication(void);
	~CNrpApplication(void);

	COMPANIES_LIST companies_;
	GAMEIMAGES_MAP gameImages_;
	USER_LIST users_;
	TECH_LIST technologies_;					//хранит все технологии игрового мира
	INVENTION_LIST inventions_;
	TECH_LIST boxAddons_;
	DISKMACHINES_LIST diskMachines_;
	GAMES_LIST games_;
	RETAILER_LIST retailers_;
	GAMEENGINES_MAP engines_;
	PROJECTS_MAP projects_;
	PROJECTS_MAP devProjects_;

	SPEED speed_;
	int lastTimeUpdate_;

	void Load_( char* fileName ) {}
	void BeginNewHour_();
	void BeginNewDay_();
	void BeginNewMonth_();
	IUser* CreateRandomUser_( std::string userType );
	int GetGameRating_( CNrpGame* ptrGame, GAME_RATING_TYPE typeRating );
	void UpdateMarketGames_();
	int GetFreePlatformNumberForGame_( CNrpGame* game );
	int GetSalesNumber_( CNrpGame* game, CNrpCompany* cmp );
	void LoadFreeImageLists_( const std::string& fileName );
	void AssociateTechnologies_();
};

}//namespace nrp