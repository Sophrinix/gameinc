#pragma once
#include "INrpProject.h"
#include "nrpArrays.h"

namespace nrp
{
typedef enum { TS_UNKNOWN=0, TS_READY, TS_INDEVELOP, TS_PROJECT, TS_COUNT } TECH_STATUS;
const NrpText TECH_STATUS_NAME[TS_COUNT] = { L"unknown", L"ready", L"indevelop", L"project" };

class IUser;
class CNrpCompany;
class CNrpInvention;

OPTION_NAME BASE_CODE( L"baseCode" );
OPTION_NAME ENGINE_CODE( L"engineCode" );
OPTION_NAME LASTWORKER( L"lastWorker" );
OPTION_NAME ERRORNUMBER( L"moduleErrorNumber");
OPTION_NAME HAVELIDER( L"haveLider" );
OPTION_NAME PRICE( L"price" );
OPTION_NAME BASEFILE( L"baseFile" );
OPTION_NAME INTEREST( L"interest" );
OPTION_NAME NEXTTECHNUMBER( L"nexttechnumber" );
OPTION_NAME REQUIRETECH( L"requireTech" );
OPTION_NAME STATUS( L"status" );

class CNrpTechnology : public INrpProject
{
public:
	CNrpTechnology( PROJECT_TYPE typen, const CLASS_NAME& className=CNrpTechnology::ClassName() );
	CNrpTechnology( CNrpInvention* invention );
	CNrpTechnology( const NrpText& fileTech );
	~CNrpTechnology(void);

	void SetEngineTechRequire( int tech_type, int valuel );
	void SetEmployerSkillRequire( int skill_type, int valuel );

	int GetEngineTechRequire( int tech_type );
	int GetEployerSkillRequire( int skil_require );

	const NrpText GetFutureTech( size_t index );
	void AddFutureTech( const NrpText& techName );

	float GetEmployerPosibility( IUser* ptrUser );

	NrpText Save( const NrpText& saveFolder );
	void Load( const NrpText& fileName );

	const REQUIRE_MAP& GetTechRequires() { return _techRequires; }
	const REQUIRE_MAP& GetSkillRequires() { return _skillRequires; }

	static NrpText ClassName();
protected:
	CNrpTechnology();
	CNrpTechnology( const CNrpTechnology& p );

	void _InitializeOptions();

	REQUIRE_MAP _techRequires;
	REQUIRE_MAP _skillRequires;
	STRINGS _futureTech;
}; 

typedef CNrpTechnology* PNrpTechnology;

}//namespace nrp