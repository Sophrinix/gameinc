#pragma once
#include "INrpProject.h"
#include <map>

namespace nrp
{

typedef std::map< int, int > REQUIRE_MAP;

class IUser;
class CNrpCompany;

OPTION_NAME BASE_CODE( "baseCode" );
OPTION_NAME ENGINE_CODE( "engineCode" );
OPTION_NAME LASTWORKER( "lastWorker" );
OPTION_NAME ERRORNUMBER( "moduleErrorNumber");
OPTION_NAME HAVELIDER( "haveLider" );
OPTION_NAME PRICE( "price" );
OPTION_NAME TEXTURENORMAL( "textureNormal" );
OPTION_NAME TEXTUREHOVER( "textureHover" );
OPTION_NAME DESCRIPTIONPATH( "descriptionPath" );
OPTION_NAME INTEREST( "interest" );

class CNrpTechnology : public INrpProject
{
public:
	CNrpTechnology( PROJECT_TYPE typen );
	~CNrpTechnology(void);

	void SetEngineTechRequire( int tech_type, int valuel );
	void SetEmployerSkillRequire( int skill_type, int valuel );

	int GetEngineTechRequire( int tech_type );
	int GetEployerSkillRequire( int skil_require );

	float GetEmployerPosibility( IUser* ptrUser );

	void Save( std::string saveFolder );
	void Load( std::string fileName );

	const REQUIRE_MAP& GetTechRequires() { return techRequires_; }
	const REQUIRE_MAP& GetSkillRequires() { return skillRequires_; }

protected:
	void Load_( char* file_name ) {}
	void InitializeOptions_();

	REQUIRE_MAP techRequires_;
	REQUIRE_MAP skillRequires_;
}; 

typedef CNrpTechnology* PNrpTechnology;

}//namespace nrp