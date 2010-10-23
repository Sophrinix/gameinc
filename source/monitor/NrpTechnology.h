#pragma once
#include "INrpProject.h"
#include <map>
#include <string>

namespace nrp
{

CLASS_NAME CLASS_TECHNOLOGY( "CNrpTechnology" );

typedef std::map< int, int > REQUIRE_MAP;
typedef enum { TS_UNKNOWN=0, TS_READY, TS_INDEVELOP, TS_PROJECT, TS_COUNT } TECH_STATUS;
const std::string TECH_STATUS_NAME[TS_COUNT] = { "unknown", "ready", "indevelop", "project" };

class IUser;
class CNrpCompany;
class CNrpInvention;

OPTION_NAME BASE_CODE( "baseCode" );
OPTION_NAME ENGINE_CODE( "engineCode" );
OPTION_NAME LASTWORKER( "lastWorker" );
OPTION_NAME ERRORNUMBER( "moduleErrorNumber");
OPTION_NAME HAVELIDER( "haveLider" );
OPTION_NAME PRICE( "price" );
OPTION_NAME BASEFILE( "baseFile" );
OPTION_NAME DESCRIPTIONPATH( "descriptionPath" );
OPTION_NAME INTEREST( "interest" );
OPTION_NAME NEXTTECHNUMBER( "nexttechnumber" );
OPTION_NAME REQUIRETECH( "requireTech" );
OPTION_NAME STATUS( "status" );

class CNrpTechnology : public INrpProject
{
public:
	CNrpTechnology( PROJECT_TYPE typen, const CLASS_NAME className=CLASS_TECHNOLOGY );
	CNrpTechnology( CNrpInvention* invention );
	CNrpTechnology( const std::string& fileTech );
	~CNrpTechnology(void);

	void SetEngineTechRequire( int tech_type, int valuel );
	void SetEmployerSkillRequire( int skill_type, int valuel );

	int GetEngineTechRequire( int tech_type );
	int GetEployerSkillRequire( int skil_require );

	const std::string GetFutureTech( size_t index );
	void AddFutureTech( const std::string& techName );

	float GetEmployerPosibility( IUser* ptrUser );

	std::string Save( const std::string& saveFolder );
	void Load( const std::string& fileName );

	const REQUIRE_MAP& GetTechRequires() { return techRequires_; }
	const REQUIRE_MAP& GetSkillRequires() { return skillRequires_; }

	static std::string ClassName() { return CLASS_TECHNOLOGY; }
protected:
	CNrpTechnology( CLASS_NAME className, PROJECT_TYPE typen );
	CNrpTechnology();

	void InitializeOptions_();
	void SaveRequires_( const std::string& fileName );
	void LoadRequries_( const std::string& fileName );

	REQUIRE_MAP techRequires_;
	REQUIRE_MAP skillRequires_;
	std::vector< std::string > futureTech_;
}; 

typedef CNrpTechnology* PNrpTechnology;

}//namespace nrp