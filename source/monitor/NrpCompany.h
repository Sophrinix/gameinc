#pragma once
#include "nrpConfig.h"
#include <map>
#include <vector>
#include <string>

namespace nrp
{

CLASS_NAME CLASS_NRPCOMPANY( "CNrpCompany" );

typedef enum { ET_DAY_END=0, ET_COUNT } EVENT_TYPE;

OPTION_NAME CEO("ceo");
OPTION_NAME ENGINES_NUMBER( "engineNumber" );
OPTION_NAME PROJECTNUMBER( "projectNumber" );
OPTION_NAME GAMENUMBER( "gameNumber" );
OPTION_NAME OBJECTSINPORTFELLE( "objectInPortfelle" );
OPTION_NAME DEVELOPPROJECTS_NUMBER( "developProjectsNumber" );
OPTION_NAME INVENTIONSNUMBER( "inventionsNumber" );

class INrpProject;
class CNrpGame;
class CNrpGameEngine;
class IUser;
class CNrpTechnology;
class CNrpDevelopGame;
class CNrpGameProject;
class CNrpInvention;
class INrpDevelopProject;

class CNrpCompany : public INrpConfig
{
public:
	typedef std::vector< CNrpGameEngine* > ENGINE_LIST;
	typedef std::vector< IUser* > USER_LIST;
	typedef std::map< std::string, CNrpTechnology* > TECH_MAP;
	typedef std::map< std::string, INrpProject* > PROJECT_MAP;
	typedef std::map< std::string, INrpDevelopProject* > DEVPROJECT_MAP;
	typedef std::map< std::string, CNrpGame* > GAME_MAP;
	typedef std::vector< INrpConfig* > OBJECT_LIST;
	typedef std::vector< CNrpInvention* > INVENTION_LIST;

	CNrpCompany( const std::string& name, IUser* ceo );
	CNrpCompany( const std::string& fileName );
	~CNrpCompany(void);

	void AddGameEngine( CNrpGameEngine* ptrEng );
	CNrpGameEngine* GetGameEngine( int index ) const;
	CNrpGameEngine* GetGameEngine( const std::string& name ) const;

	CNrpTechnology* GetTechnology( int index ) const;
	CNrpTechnology* GetTechnology( const std::string& name ) const;
	void AddTechnology( CNrpTechnology* tech );
	INrpConfig* GetFromPortfelle( size_t index ) const;
	void RemoveFromPortfelle( const INrpConfig* ptrObject );
	void AddToPortfelle( INrpConfig* const ptrObject );
	float GetUserModificatorForGame( CNrpGame* game );

	void AddDevelopProject( INrpDevelopProject* ptrDevProject );
	INrpDevelopProject* GetDevelopProject( const std::string& name ) const;
	INrpDevelopProject* GetDevelopProject( size_t index ) const;
	void RemoveDevelopProject( const std::string& name );

	void AddProject( INrpProject* ptrProject );
	void RemoveProject( const std::string& name );
	INrpProject* GetProject( const std::string& name ) const;
	INrpProject* GetProject( size_t index ) const;

	void AddInvention( CNrpInvention* const inv );
	void InventionReleased( const CNrpInvention* inv );
	CNrpInvention* GetInvention( const std::string& name );
	CNrpInvention* GetInvention( int index );
	void RemoveInvention( CNrpInvention* inv );

	void AddUser( IUser* user );
	void RemoveUser( const std::string& name );
	IUser* GetUser( int index ) const;
	IUser* GetUser( const std::string& name ) const;
	
	CNrpGame* GetGame( const std::string& gameName ) const;
	CNrpGame* GetGame( size_t index ) const;
	CNrpGame* CreateGame( CNrpDevelopGame* ptrProject );

	void BeginNewHour( const SYSTEMTIME& time );
	void BeginNewDay( const SYSTEMTIME& time );
	void BeginNewMonth( const SYSTEMTIME& time );

	std::string Save( const std::string& saveFolder );
	void Load( const std::string& loadFolder );

	static std::string ClassName() { return CLASS_NRPCOMPANY; }
private:
	PROJECT_MAP _projects; 
	ENGINE_LIST _engines;
	TECH_MAP _technologies;
	DEVPROJECT_MAP _devProjects;
	GAME_MAP _games;
	USER_LIST _employers;
	OBJECT_LIST _portfelle;
	INVENTION_LIST _inventions;

	void _PaySalaries();
	void _UpdateGameProjectState();
	void _InitialyzeOptions();
}; 

typedef CNrpCompany* PNrpCompany;
}//namespace nrp