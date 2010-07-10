#pragma once
#include "INrpProject.h"
#include <map>

namespace nrp
{

typedef std::map< int, int > REQUIRE_MAP;
typedef enum { TS_UNKNOWN=0, TS_READY, TS_INDEVELOP, TS_PROJECT } TECH_STATUS;

class IUser;
class CNrpCompany;

OPTION_NAME BASE_CODE( "baseCode" );
OPTION_NAME ENGINE_CODE( "engineCode" );
OPTION_NAME LASTWORKER( "lastWorker" );
OPTION_NAME ERRORNUMBER( "moduleErrorNumber");
OPTION_NAME HAVELIDER( "haveLider" );
OPTION_NAME PRICE( "price" );
OPTION_NAME BASEFILE( "baseFile" );
OPTION_NAME TEXTURENORMAL( "textureNormal" );
OPTION_NAME TEXTUREHOVER( "textureHover" );
OPTION_NAME DESCRIPTIONPATH( "descriptionPath" );
OPTION_NAME INTEREST( "interest" );
OPTION_NAME NEXTTECHNUMBER( "nexttechnumber" );
OPTION_NAME REQUIRETECH( "requireTech" );
OPTION_NAME STATUS( "status" );

class CNrpTechnology : public INrpProject
{
public:
	CNrpTechnology( PROJECT_TYPE typen );
	~CNrpTechnology(void);

	void SetEngineTechRequire( int tech_type, int valuel );
	void SetEmployerSkillRequire( int skill_type, int valuel );

	int GetEngineTechRequire( int tech_type );
	int GetEployerSkillRequire( int skil_require );

	CNrpTechnology* GetFutureTech( size_t index );
	void AddFutureTech( CNrpTechnology* tech );

	float GetEmployerPosibility( IUser* ptrUser );

	virtual void Save( std::string saveFolder );
	virtual void Load( std::string fileName );

	const REQUIRE_MAP& GetTechRequires() { return techRequires_; }
	const REQUIRE_MAP& GetSkillRequires() { return skillRequires_; }

protected:
	void Load_( char* file_name ) {}
	void InitializeOptions_();
	void SaveRequires_( const std::string& fileName );
	void LoadRequries_( const std::string& fileName );

	REQUIRE_MAP techRequires_;
	REQUIRE_MAP skillRequires_;
	std::vector< CNrpTechnology* > futureTech_;
}; 

typedef CNrpTechnology* PNrpTechnology;

}//namespace nrp