#include "StdAfx.h"
#include "NrpTechnology.h"
#include "NrpGameProject.h"
#include "IUser.h"
#include "NrpCompany.h"
#include "NrpApplication.h"

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
	CreateValue<std::string>( COMPONENTLIDER, "" );
	CreateValue<std::string>( LASTWORKER, "" );
	CreateValue<std::string>( COMPANY, "" );
	CreateValue<int>( CODEVOLUME, 0 );
	CreateValue<int>( CODEPASSED, 0 );
	CreateValue<float>( READYWORKPERCENT, 0 );
	CreateValue<int>( ERRORNUMBER, 0 );
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
	
	REQUIRE_MAP::iterator tIter = techRequires_.begin();
	for( ; tIter != techRequires_.end(); ++tIter )
		IniFile::Write( "techRequire", IntToStr( tIter->first ), IntToStr( tIter->second ), fileName );

	REQUIRE_MAP::iterator sIter = skillRequires_.begin();
	for( ; sIter != skillRequires_.end(); ++sIter )
		IniFile::Write( "skillRequire", IntToStr( sIter->first ), IntToStr( sIter->second ), fileName );
}

void CNrpTechnology::Load( std::string fileName )
{
	INrpProject::Load( PROPERTIES, fileName );

	ReadValueList_( "techRequire", techRequires_, fileName );
	ReadValueList_( "skillRequire", skillRequires_, fileName );
}

void CNrpTechnology::ReadValueList_( std::string sectionName, REQUIRE_MAP& mapt, std::string fileName )
{
	char buffer[ 32000 ];
	memset( buffer, 0, 32000 );
	GetPrivateProfileSection( sectionName.c_str(), buffer, 32000, fileName.c_str() );

	std::string readLine = buffer;
	while( readLine != "" )
	{
		std::string name, valuel;
		name = readLine.substr( 0, readLine.find( '=' ) );
		valuel = readLine.substr( readLine.find( '=' ) + 1, 0xff );
		mapt[ StrToInt( name.c_str() ) ] = StrToInt( valuel.c_str() );
		
		memcpy( buffer, buffer + strlen(buffer) + 1, 32000 );  
		readLine = buffer;
	}
}

float CNrpTechnology::GetEmployerPosibility()
{
	IUser* ptrUser = CNrpApplication::Instance().GetUser( GetValue<std::string>( COMPANY ), GetValue<std::string>( COMPONENTLIDER ) );
	if( !ptrUser )
		return 0;
	
	int minSkill = 40;
	int minSkillName = 0;
	float posibility = 0;
	REQUIRE_MAP::iterator sIter = skillRequires_.begin();
	for( ; sIter != skillRequires_.end(); ++sIter )
	{
		int skillValue = ptrUser->GetSkill( sIter->first );
		if( skillValue < minSkill )
		{
			minSkill = skillValue;
			minSkillName = sIter->first;
		}

		posibility += ( skillValue > 100 ? 100 : skillValue ) / 100.f;
		posibility /= 2.f;
	}

	return posibility;
}

void CNrpTechnology::SetLider( IUser* ptrUser )
{
	if( !GetValue<std::string>( COMPONENTLIDER ).empty() )
		SetValue<std::string>( LASTWORKER, GetValue<std::string>( COMPONENTLIDER ) );

	SetValue<std::string>( COMPONENTLIDER, ptrUser->GetValue<std::string>( NAME ) );
}
}//namespace nrp