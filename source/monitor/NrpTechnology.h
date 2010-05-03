#pragma once
#include "INrpProject.h"
#include <map>

namespace nrp
{

class IUser;
class CNrpCompany;

OPTION_NAME TECHGROUP( "techGroup" );
OPTION_NAME BASE_CODE( "baseCode" );
OPTION_NAME ENGINE_CODE( "engineCode" );
OPTION_NAME LEVEL( "level" );
OPTION_NAME PARENT( "parent" );
OPTION_NAME LASTWORKER( "lastWorker" );
OPTION_NAME ERRORNUMBER( "moduleErrorNumber");
OPTION_NAME HAVELIDER( "haveLider" );
OPTION_NAME CODEPASSED( "codePassed" );

class CNrpTechnology : public INrpProject
{
public:
	CNrpTechnology( PROJECT_TYPE typen, CNrpCompany* ptrCmp );
	CNrpTechnology( CNrpTechnology* pTech, CNrpCompany* ptrCmp );
	~CNrpTechnology(void);

	void SetEngineTechRequire( int tech_type, int valuel );
	void SetEmployerSkillRequire( int skill_type, int valuel );

	int GetEngineTechRequire( int tech_type );
	int GetEployerSkillRequire( int skil_require );

	float GetEmployerPosibility();

	void SetLider( IUser* ptrUser );
	void Update( IUser* ptrUser );

	void Save( std::string saveFolder );
	void Load( std::string fileName );

private:
	void Load_( char* file_name ) {}
	void InitializeOptions_();
	
	REQUIRE_MAP techRequires_;
	REQUIRE_MAP skillRequires_;
}; 

typedef CNrpTechnology* PNrpTechnology;

}//namespace nrp