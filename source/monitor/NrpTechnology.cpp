#include "StdAfx.h"
#include "NrpTechnology.h"
#include "IUser.h"

namespace nrp
{

CNrpTechnology::CNrpTechnology( PROJECT_TYPE typen ) : INrpProject( "CNrpTechnology", "" )
{
	CreateValue<std::string>( NAME, "" );
	CreateValue<int>( TECHGROUP, typen );
	CreateValue<int>( TECHTYPE, 0 );
	CreateValue<int>( BASE_CODE, 0 );
	CreateValue<int>( ENGINE_CODE, 0 );
	CreateValue<int>( LEVEL, 0 );
	CreateValue<LPVOID>( PARENT, NULL );
	CreateValue<int>( QUALITY, 100 );
	CreateValue<PUser>( COMPONENTLIDER, NULL );
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

void CNrpTechnology::Save( std::string saveFolder )
{

}

void CNrpTechnology::Load( std::string fileName )
{

}
}//namespace nrp