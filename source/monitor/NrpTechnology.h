#pragma once
#include "INrpProject.h"
#include <map>

namespace nrp
{

OPTION_NAME TECHGROUP( "techGroup" );
OPTION_NAME TECHTYPE( "techType" );
OPTION_NAME BASE_CODE( "baseCode" );
OPTION_NAME ENGINE_CODE( "engineCode" );
OPTION_NAME LEVEL( "level" );
OPTION_NAME PARENT( "parent" );
OPTION_NAME COMPONENTLIDER( "componentLider" );

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
	void Load_( char* file_name ) {}

	std::map< int, int > techRequires_;
	std::map< int, int > skillRequires_;
}; 

typedef CNrpTechnology* PNrpTechnology;

}//namespace nrp