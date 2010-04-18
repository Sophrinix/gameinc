#include "StdAfx.h"
#include "NrpTechnology.h"
#include "NrpGameProject.h"
#include "IUser.h"

#include <io.h>
#include <errno.h>

namespace nrp
{

CNrpTechnology::CNrpTechnology( PROJECT_TYPE typen ) : INrpProject( "CNrpTechnology", "" )
{
	CreateValue<std::string>( NAME, "" );
	CreateValue<int>( TECHGROUP, typen );
	CreateValue<int>( TECHTYPE, 0 );
	CreateValue<float>( BASE_CODE, 0 );
	CreateValue<float>( ENGINE_CODE, 0 );
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
	if( _access( saveFolder.c_str(), 0 ) == -1 )
		CreateDirectory( saveFolder.c_str(), NULL );

	std::string fileName = saveFolder + GetValue<std::string>( NAME ) + ".ini";

	DeleteFile( fileName.c_str() );
	INrpProject::Save( PROPERTIES, fileName );

	if( GetValue<LPVOID>( PARENT ) )
		IniFile::Write( PROPERTIES, "parent", GetValue<PNrpGameProject>( PARENT )->GetValue<std::string>(NAME), fileName );
	
	if( GetValue<PUser>( COMPONENTLIDER ) )
		IniFile::Write( PROPERTIES, "componentLider", GetValue<PUser>( COMPONENTLIDER )->GetValue<std::string>( NAME ), fileName );
	
	REQUIRE_MAP::iterator tIter = techRequires_.begin();
	for( ; tIter != techRequires_.end(); ++tIter )
		IniFile::Write( "rechRequire", IntToStr( tIter->first ), IntToStr( tIter->second ), fileName );

	REQUIRE_MAP::iterator sIter = skillRequires_.begin();
	for( ; sIter != skillRequires_.end(); ++sIter )
		IniFile::Write( "skillRequire", IntToStr( sIter->first ), IntToStr( sIter->second ), fileName );

}

void CNrpTechnology::Load( std::string fileName )
{

}
}//namespace nrp