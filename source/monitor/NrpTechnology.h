#pragma once
#include "INrpProject.h"
#include <map>

namespace nrp
{

OPTION_NAME TECHGROUP( "techGroup" );
OPTION_NAME BASE_CODE( "baseCode" );
OPTION_NAME ENGINE_CODE( "engineCode" );
OPTION_NAME LEVEL( "level" );
OPTION_NAME PARENT( "parent" );
OPTION_NAME COMPONENTLIDER( "componentLider" );
OPTION_NAME LASTWORKER( "lastWorker" );
OPTION_NAME ERRORNUMBER( "moduleErrorNumber");
OPTION_NAME HAVELIDER( "haveLider" );
OPTION_NAME CODEPASSED( "codePassed" );

class CNrpTechnology : public INrpProject
{
public:
	CNrpTechnology( PROJECT_TYPE typen );
	~CNrpTechnology(void);

	void SetEngineTechRequire( int tech_type, int valuel );
	void SetEmployerSkillRequire( int skill_type, int valuel );

	int GetEngineTechRequire( int tech_type );
	int GetEployerSkillRequire( int skil_require );

	void Save( std::string saveFolder );
	void Load( std::string fileName );

private:
	typedef std::map< int, int > REQUIRE_MAP;

	void Load_( char* file_name ) {}
	void ReadValueList_( std::string sectionName, REQUIRE_MAP& mapt, std::string fileName );
	
	REQUIRE_MAP techRequires_;
	REQUIRE_MAP skillRequires_;
}; 

typedef CNrpTechnology* PNrpTechnology;

}//namespace nrp