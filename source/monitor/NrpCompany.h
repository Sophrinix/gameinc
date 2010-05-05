#pragma once
#include "nrpConfig.h"
#include "LuaFunctionality.h"
#include <map>
#include <vector>
#include <string>

namespace nrp
{

typedef enum { ET_DAY_END=0, ET_COUNT } EVENT_TYPE;

OPTION_NAME CEO("ceo");
OPTION_NAME ENGINES_NUMBER( "engineNumber" );
OPTION_NAME PROJECTNUMBER( "projectNumber" );
OPTION_NAME GAMENUMBER( "gameNumber" );

class INrpProject;
class CNrpGame;
class CNrpGameEngine;
class IUser;
class CNrpTechnology;
class CNrpGameProject;

class CNrpCompany : public INrpConfig, public ILuaFunctionality
{
public:
	typedef std::vector< CNrpGameEngine* > ENGINE_LIST;
	typedef std::vector< IUser* > USER_LIST;
	typedef std::map< std::string, CNrpTechnology* > TECH_MAP;
	typedef std::map< std::string, INrpProject* > PROJECT_MAP;
	typedef std::map< std::string, CNrpGame* > GAME_MAP;

	CNrpCompany(const char* name);
	~CNrpCompany(void);

	void AddGameEngine( CNrpGameEngine* ptrEng );
	CNrpGameEngine* GetGameEngine( int index );
	CNrpGameEngine* GetGameEngine( std::string name );
	CNrpTechnology* GetTech( int index );
	CNrpGameProject* AddGameProject( CNrpGameProject* ptrProject );
	void RemoveGameProject( CNrpGameProject* ptrProject );
	INrpProject* GetProject( std::string name );
	INrpProject* GetProject( int index );
	void AddUser( IUser* user );
	IUser* GetUser( int index );
	IUser* GetUser( std::string name );
	CNrpGame* GetGame( std::string gameName );
	void CreateGame( CNrpGameProject* ptrProject );
	void Update();
	void PaySalaries();

	void Save( std::string saveFolder );
	void Load( std::string loadFolder );

private:
	void Load_( char* file_name ) {}
	
	PROJECT_MAP projects_; 
	ENGINE_LIST engines_;
	TECH_MAP technologies_;
	GAME_MAP games_;
	USER_LIST employers_;
}; 

typedef CNrpCompany* PNrpCompany;
}//namespace nrp