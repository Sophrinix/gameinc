#pragma once
#include "nrpConfig.h"
#include "LuaFunctionality.h"
#include "nrpArrays.h"

namespace nrp
{
OPTION_NAME PROFILENAME( L"profileName" );
OPTION_NAME PROFILECOMPANY( L"profileCompany" );
OPTION_NAME WORKDIR( L"workDir" );
OPTION_NAME SAVEDIR( L"saveDir" );
OPTION_NAME SAVEDIR_INVENTIONS( L"saveDirInvention" );
OPTION_NAME SAVEDIR_COMPANIES( L"saveDirCompanies" );
OPTION_NAME SAVEDIR_DEVPR( L"saveDirDevProjects" );
OPTION_NAME SAVEDIR_GAMES( L"saveDirGames" );
OPTION_NAME SAVEDIR_PLATFORMS( L"platforms" );
OPTION_NAME SAVEDIR_PROJECTS( L"saveDirProjects" );
OPTION_NAME SAVEDIR_ENGINES( L"saveDirEngines" );
OPTION_NAME SAVEDIR_PLANT( L"saveDirPlant" );
OPTION_NAME SAVEDIR_USERS( L"saveDirUsers" );
OPTION_NAME SAVEINI_PROFILE( L"saveIniProfile" );
OPTION_NAME SAVEDIR_PROFILE( L"saveDirProfile" );
OPTION_NAME SAVEDIR_TECHS( L"saveDirTechs" );
OPTION_NAME SAVEDIR_BRIDGE( L"saveDirBridge" );
OPTION_NAME CURRENTTIME( L"currentTime" );
OPTION_NAME BOXADDONNUMBER( L"boxAdonNumber" );
OPTION_NAME MARKETGAMENUMBER( L"marketGameNumber" );
OPTION_NAME PLAYERCOMPANY( L"playerCompany" );
OPTION_NAME MINIMUM_USER_SALARY( L"minimumUserSalary" );
OPTION_NAME PDA( L"pda" ); 
OPTION_NAME SYSTEMINI( L"systemIni" );
OPTION_NAME GAME_TIME( L"objectGameTime" );
OPTION_NAME PAUSEBTWSTEP( L"pausebtwstep" );
OPTION_NAME BRIDGE( L"bridge" );
OPTION_NAME INFLATION( L"infaltion" );
OPTION_NAME PROFIT_TAX( L"profit_tax" );

class CNrpUser;
class INrpProject;
class CNrpGameEngine;
class CNrpTechnology;
class CNrpGame;
class CNrpDiskMachine;
class CNrpGameEngine;
class CNrpInvention;
class INrpDevelopProject;
class CNrpBridge;
	
class CNrpApplication : public INrpConfig, public ILuaFunctionality
{
	friend class CNrpGameTime;
public:
	static CNrpApplication& Instance();

	void ResetData();

	void Save();
	void Load( const NrpText& profileName, const NrpText& companyName );
	void CreateProfile( const NrpText& profileName, const NrpText& companyName );

	COMPANIES& GetCompanies();
	CNrpCompany* GetCompany( const NrpText& companyName );
	CNrpCompany* GetCompany( u32 index );
	int AddCompany( CNrpCompany* company );

	int AddUser( CNrpUser* user );
	int RemoveUser( CNrpUser* user );

	void CreateNewFreeUsers();
	CNrpUser* CreateRandomUser( NrpText userType );
	
	CNrpUser* GetUser( u32 index );
	CNrpUser* GetUser( const NrpText& name );

	void AddDevelopProject( INrpDevelopProject* project );
	void RemoveDevelopProject( const NrpText& name );
	INrpDevelopProject* GetDevelopProject( const NrpText& name ); 

	void AddGameEngine( nrp::CNrpGameEngine* ptrEngine );
	CNrpGameEngine* GetGameEngine( const NrpText& name );
	void RemoveGameEngine( nrp::CNrpGameEngine* ptrEngine );

	CNrpGame* GetGame( const NrpText& name );
	CNrpGame* GetGame( u32 index );
	const GAMES& GetGames() { return _games; }
	void AddGame( CNrpGame* ptrGame );

	void AddProject( nrp::INrpProject* project );
	INrpProject* GetProject( const NrpText& name );

	int GetTechsNumber() { return _technologies.size(); }
	CNrpTechnology* GetTechnology( int index );
	CNrpTechnology* GetTechnology( const NrpText& name );

	void AddTechnology( CNrpTechnology* ptrTech );
	void RemoveTechnology( CNrpTechnology* ptrTech );

	bool AddPlatform( CNrpPlatform* platform );
	void RemovePlatform( const NrpText& name );
	CNrpPlatform* GetPlatform( const NrpText& name );
	CNrpPlatform* GetPlatform( size_t index );

	void UpdateGameRatings( CNrpGame* ptrGame, bool firstTime=false );

	CNrpTechnology* GetBoxAddon( size_t index ) { return index < _boxAddons.size() ? _boxAddons[ index ] : NULL; }
	CNrpTechnology* GetBoxAddon( const NrpText& name );
	bool AddBoxAddon( CNrpTechnology* tech );

	void AddGameToMarket( CNrpGame* game );

	CNrpRetailer* GetRetailer( const NrpText& name );
	void RemoveRetailer( const NrpText& name );

	NrpText GetFreeInternalName( CNrpGame* game );
	CNrpScreenshot* GetScreenshot( const NrpText& name );
	void LoadScreenshot( const NrpText& fileName );
	float GetGameGenreInterest( CNrpGame* game );

	void AddInvention( const NrpText& startTech, CNrpCompany& parentCompany );
	void InventionFinished( CNrpInvention& ptrInvention );
	void InventionCanceled( CNrpInvention* ptrInvention );
	CNrpInvention* GetInvention( const NrpText& name, const NrpText& companyName );
	void Init();

	void SetLink( const NrpText& name, const NrpText& pathto );
	NrpText GetLink( const NrpText& name );
	void LoadLinks( const NrpText& fileName, const NrpText& templateName );

	static NrpText ClassName();
private:
	CNrpApplication(void);
	~CNrpApplication(void);

	COMPANIES _companies;
	SCREENSHOTS _screenshots;
	USERS _users;
	TECHS _technologies;					//хранит все технологии игрового мира
	INVENTIONS _inventions;
	TECHS _boxAddons;
	GAMES _games;
	PLATFORMS _platforms;
	RETAILERS _retailers;
	ENGINES _engines;
	PROJECTS _projects;
	DEVPROJECTS _devProjects;
	LINK_MAP _links;

	void _BeginNewHour();
	void _BeginNewDay();
	void _BeginNewMonth();
	void _UpdateMarketGames();
	int _GetFreePlatformNumberForGame( CNrpGame* game );
	int _GetSalesNumber( CNrpGame* game );
	void _UpdateInvention();

	void _LoadUsers( const NrpText& iniFile );
	void _InitialyzeSaveDirectories( const NrpText& profileName );
	void _UpdateGameRating( CNrpGame* ptrGame, GAME_RATING_TYPE typeRating );
	void _CreateDirectoriesMapForSave();
	int _GetTechsByGroup( int type, TECHS& arrayt );
};

#define _nrpApp CNrpApplication::Instance()

}//namespace nrp