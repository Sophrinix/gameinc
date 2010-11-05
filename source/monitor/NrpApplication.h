#pragma once
#include "nrpConfig.h"
#include "LuaFunctionality.h"
#include <vector>

namespace nrp
{

CLASS_NAME CLASS_NRPAPPLICATION( "CNrpApplication" );

OPTION_NAME BANK( "bank" );
OPTION_NAME PROFILENAME( "profileName" );
OPTION_NAME PROFILECOMPANY( "profileCompany" );
OPTION_NAME WORKDIR( "workDir" );
OPTION_NAME SAVEDIR( "saveDir" );
OPTION_NAME SAVEDIR_INVENTIONS( "saveDirInvention" );
OPTION_NAME SAVEDIR_COMPANIES( "saveDirCompanies" );
OPTION_NAME SAVEDIR_DEVPR( "saveDirDevProjects" );
OPTION_NAME SAVEDIR_GAMES( "saveDirGames" );
OPTION_NAME SAVEDIR_PROJECTS( "saveDirProjects" );
OPTION_NAME SAVEDIR_ENGINES( "saveDirEngines" );
OPTION_NAME SAVEDIR_PLANT( "saveDirPlant" );
OPTION_NAME SAVEDIR_USERS( "saveDirUsers" );
OPTION_NAME SAVEINI_PROFILE( "saveIniProfile" );
OPTION_NAME SAVEDIR_PROFILE( "saveDirProfile" );
OPTION_NAME SAVEDIR_TECHS( "saveDirTechs" );
OPTION_NAME CURRENTTIME( "currentTime" );
OPTION_NAME DISKMACHINENUMBER( "diskMachineNumber" );
OPTION_NAME BOXADDONNUMBER( "boxAdonNumber" );
OPTION_NAME MARKETGAMENUMBER( "marketGameNumber" );
OPTION_NAME PLAYERCOMPANY( "playerCompany" );
OPTION_NAME MINIMUM_USER_SALARY( "minimumUserSalary" );
OPTION_NAME PDA( "pda" ); 
OPTION_NAME SYSTEMINI( "systemIni" );
OPTION_NAME GAME_TIME( "objectGameTime" );
OPTION_NAME PAUSEBTWSTEP( "pausebtwstep" );

class CNrpCompany;
class IUser;
class INrpProject;
class CNrpGameEngine;
class CNrpTechnology;
class CNrpGame;
class CNrpDiskMachine;
class CNrpRetailer;
class CNrpScreenshot;
class CNrpGameEngine;
class CNrpInvention;
class INrpDevelopProject;
	
class CNrpApplication : public INrpConfig, public ILuaFunctionality
{
	friend class CNrpGameTime;

	typedef std::vector< IUser* > USER_LIST;
	typedef std::vector< CNrpTechnology* > TECH_LIST;
	typedef std::vector< CNrpDiskMachine* > DISKMACHINES_LIST;
	typedef std::vector< CNrpGame* > GAMES_LIST;
	typedef std::vector< CNrpRetailer* > RETAILER_LIST;
	typedef std::vector< CNrpInvention* > INVENTION_LIST;
	typedef std::vector< CNrpGameEngine* > GAMEENGINES_LIST;

	typedef std::map< std::string, CNrpScreenshot* > SCREENSHOTS_MAP; 
	typedef std::map< std::string, INrpProject* > PROJECTS_MAP;
	typedef std::map< std::string, INrpDevelopProject* > DEVPROJECTS_MAP;
public:
	typedef std::vector< CNrpCompany* > COMPANIES_LIST;
	static CNrpApplication& Instance();

	void ResetData();

	void SaveProfile();
	void LoadProfile( const std::string& profileName, const std::string& companyName );
	void CreateProfile( const std::string& profileName, const std::string& companyName );

	COMPANIES_LIST& GetCompanies();
	CNrpCompany* GetCompany( std::string companyName ) const;
	CNrpCompany* GetCompany( int index ) const;
	int AddCompany( CNrpCompany* company );

	int AddUser( IUser* user );
	int RemoveUser( IUser* user );
	void CreateNewFreeUsers();
	IUser* GetUser( int index ) const;
	IUser* GetUser( const std::string& name ) const;

	void AddDevelopProject( INrpDevelopProject* project );
	void RemoveDevelopProject( const std::string& name );
	INrpDevelopProject* GetDevelopProject( const std::string& name ) const; 

	void AddGameEngine( nrp::CNrpGameEngine* ptrEngine );
	CNrpGameEngine* GetGameEngine( const std::string& name ) const;
	void RemoveGameEngine( nrp::CNrpGameEngine* ptrEngine );

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
	CNrpScreenshot* GetScreenshot( const std::string& name );
	void LoadScreenshots( const std::string& fileName );
	float GetGameGenreInterest( CNrpGame* game );

	void AddInvention( const std::string& startTech, CNrpCompany* parentCompany );
	void InventionFinished( CNrpInvention* ptrInvention );
	void InventionCanceled( CNrpInvention* ptrInvention );
	CNrpInvention* GetInvention( std::string name, std::string companyName );

	static std::string ClassName() { return CLASS_NRPAPPLICATION; }
private:
	CNrpApplication(void);
	~CNrpApplication(void);

	COMPANIES_LIST companies_;
	SCREENSHOTS_MAP _screenshots;
	USER_LIST users_;
	TECH_LIST technologies_;					//хранит все технологии игрового мира
	INVENTION_LIST inventions_;
	TECH_LIST boxAddons_;
	DISKMACHINES_LIST diskMachines_;
	GAMES_LIST games_;
	RETAILER_LIST retailers_;
	GAMEENGINES_LIST engines_;
	PROJECTS_MAP projects_;
	DEVPROJECTS_MAP devProjects_;

	void _BeginNewHour();
	void _BeginNewDay();
	void _BeginNewMonth();
	IUser* CreateRandomUser_( std::string userType );
	void UpdateMarketGames_();
	int GetFreePlatformNumberForGame_( CNrpGame* game );
	int GetSalesNumber_( CNrpGame* game, CNrpCompany* cmp );
	void UpdateInvention_();

	void _LoadUsers( const std::string& iniFile );
	void _InitialyzeSaveDirectories( const std::string& profileName );
	void _UpdateGameRating( CNrpGame* ptrGame, GAME_RATING_TYPE typeRating );
	void _CreateDirectoriesMapForSave();
};

}//namespace nrp