#pragma once
#include "nrpConfig.h"
#include "LuaFunctionality.h"
#include <map>
#include <vector>
#include <string>

namespace nrp
{

OPTION_NAME CLASS_NRPCOMPANY( "CNrpCompany" );

typedef enum { ET_DAY_END=0, ET_COUNT } EVENT_TYPE;

OPTION_NAME CEO("ceo");
OPTION_NAME ENGINES_NUMBER( "engineNumber" );
OPTION_NAME PROJECTNUMBER( "projectNumber" );
OPTION_NAME GAMENUMBER( "gameNumber" );
OPTION_NAME OBJECTSINPORTFELLE( "objectInPortfelle" );
OPTION_NAME DEVELOPPROJECTS_NUMBER( "developProjectsNumber" );

class INrpProject;
class CNrpGame;
class CNrpGameEngine;
class IUser;
class CNrpTechnology;
class CNrpDevelopGame;
class CNrpGameProject;
class CNrpInvention;

class CNrpCompany : public INrpConfig, public ILuaFunctionality
{
public:
	typedef std::vector< CNrpGameEngine* > ENGINE_LIST;
	typedef std::vector< IUser* > USER_LIST;
	typedef std::map< std::string, CNrpTechnology* > TECH_MAP;
	typedef std::map< std::string, INrpProject* > PROJECT_MAP;
	typedef std::map< std::string, CNrpGame* > GAME_MAP;
	typedef std::vector< INrpConfig* > OBJECT_LIST;
	typedef std::vector< CNrpInvention* > INVENTION_LIST;

	CNrpCompany( std::string name, IUser* ceo );
	~CNrpCompany(void);

	void AddGameEngine( CNrpGameEngine* ptrEng );
	CNrpGameEngine* GetGameEngine( int index );
	CNrpGameEngine* GetGameEngine( std::string name );
	CNrpTechnology* GetTechnology( int index );
	CNrpTechnology* GetTechnology( std::string name );
	void AddTechnology( CNrpTechnology* tech );
	INrpConfig* GetFromPortfelle( int index );
	void RemoveFromPortfelle( INrpConfig* ptrObject );
	void AddToPortfelle( INrpConfig* ptrObject );
	float GetUserModificatorForGame( CNrpGame* game );

	void AddDevelopProject( INrpProject* ptrDevProject );
	INrpProject* GetDevelopProject( const std::string name );
	INrpProject* GetDevelopProject( size_t index );
	void RemoveDevelopProject( std::string name );

	void AddProject( INrpProject* ptrProject );
	void RemoveProject( std::string name );
	INrpProject* GetProject( std::string name );
	INrpProject* GetProject( size_t index );

	void StartInvention( CNrpInvention* inv );
	void InventionReleased( CNrpInvention* inv );

	void AddUser( IUser* user );
	IUser* GetUser( int index );
	IUser* GetUser( std::string name );
	
	CNrpGame* GetGame( std::string gameName );
	CNrpGame* GetGame( size_t index );
	CNrpGame* CreateGame( CNrpDevelopGame* ptrProject );

	void BeginNewHour( const SYSTEMTIME& time );
	void BeginNewDay( const SYSTEMTIME& time );
	void BeginNewMonth( const SYSTEMTIME& time );

	void Save( const std::string& saveFolder );
	void Load( const std::string& loadFolder );

private:
	void Load_( char* file_name ) {}

	PROJECT_MAP projects_; 
	ENGINE_LIST engines_;
	TECH_MAP technologies_;
	PROJECT_MAP devProjects_;
	GAME_MAP games_;
	USER_LIST employers_;
	OBJECT_LIST portfelle_;
	INVENTION_LIST inventions_;

	void PaySalaries_();
	void UpdateGameProjectState_();
}; 

typedef CNrpCompany* PNrpCompany;
}//namespace nrp