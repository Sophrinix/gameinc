#include "StdAfx.h"
#include "NrpTechnology.h"

namespace nrp
{

CNrpTechnology::CNrpTechnology( PROJECT_TYPE typen ) : INrpProject( "CNrpTechnology", "" )
{
	options_[ NAME ] = new std::string( "" );
	options_[ TECHGROUP ] = new int ( typen );
	options_[ TECHTYPE ] = new int( 0 );
	options_[ BASE_CODE ] = new int( 0 );
	options_[ ENGINE_CODE ] = new int( 0 );
	options_[ LEVEL ] = new int( 0 );
	options_[ PARENT ] = new LPVOID( NULL );
	options_[ QUALITY ] = new int( 100 );
}

CNrpTechnology::~CNrpTechnology(void)
{
}

void CNrpTechnology::SetEngineTechRequire( int tech_type, int valuel )
{
	techRequires_[ tech_type ] = valuel;
}

void CNrpTechnology::SetEmployerSkillRequire( int skill_type, int valuel )
{
	skillRequires_[ skill_type ] = valuel;
}

int CNrpTechnology::GetEngineTechRequire( int tech_type )
{
	if( techRequires_.find( tech_type ) == techRequires_.end() )
		return 0;

	return techRequires_[ tech_type ];
}

int CNrpTechnology::GetEployerSkillRequire( int skil_require )
{
	if( skillRequires_.find( skil_require ) == skillRequires_.end() )
		return 0;

	return skillRequires_[ skil_require ];
}
}//namespace nrp