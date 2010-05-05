#include "StdAfx.h"
#include "NrpTechnology.h"
#include "NrpGameProject.h"
#include "IUser.h"
#include "NrpCompany.h"
#include "NrpApplication.h"

#include <io.h>
#include <errno.h>
#include <assert.h>

namespace nrp
{

CNrpTechnology::CNrpTechnology( PROJECT_TYPE typen, CNrpCompany* ptrCmp ) : INrpProject( "CNrpTechnology", "" )
{
	InitializeOptions_();
	if( ptrCmp != NULL )
	{
		SetValue<std::string>( COMPANYNAME, ptrCmp->GetValue<std::string>( NAME ) );
		SetValue<PNrpCompany>( PARENTCOMPANY, ptrCmp );
	}
	SetValue<int>( TECHGROUP, typen );
}

void CNrpTechnology::InitializeOptions_()
{
	CreateValue<std::string>( NAME, "" );
	CreateValue<int>( TECHGROUP, 0 );
	CreateValue<int>( TECHTYPE, 0 );
	CreateValue<float>( BASE_CODE, 0 );
	CreateValue<float>( ENGINE_CODE, 0 );
	CreateValue<int>( LEVEL, 0 );
	CreateValue<LPVOID>( PARENT, NULL );
	CreateValue<int>( QUALITY, 100 );
	CreateValue<std::string>( COMPONENTLIDER, "" );
	CreateValue<std::string>( LASTWORKER, "" );
	CreateValue<std::string>( COMPANYNAME, "" );
	CreateValue<PNrpCompany>( PARENTCOMPANY, NULL );
	CreateValue<int>( CODEVOLUME, 0 );
	CreateValue<int>( CODEPASSED, 0 );
	CreateValue<float>( READYWORKPERCENT, 0 );
	CreateValue<int>( ERRORNUMBER, 0 );
}

CNrpTechnology::CNrpTechnology( CNrpTechnology* pTech, CNrpCompany* ptrCmp ) 
				: INrpProject( "CNrpTechnology", pTech->GetValue<std::string>( NAME ) )
{
	InitializeOptions_();

	SetValue<std::string>( NAME, pTech->GetValue<std::string>( NAME ) );
	SetValue<int>( TECHGROUP, pTech->GetValue<int>( TECHGROUP ) );
	SetValue<int>( TECHTYPE, pTech->GetValue<int>( TECHTYPE ) );
	SetValue<float>( BASE_CODE, pTech->GetValue<float>( BASE_CODE ) );
	SetValue<float>( ENGINE_CODE, pTech->GetValue<float>( ENGINE_CODE ) );
	SetValue<int>( LEVEL, pTech->GetValue<int>( LEVEL ) );
	SetValue<LPVOID>( PARENT, pTech->GetValue<LPVOID>( PARENT ) );
	SetValue<int>( QUALITY, pTech->GetValue<int>( QUALITY ) );
	SetValue<std::string>( COMPONENTLIDER, pTech->GetValue<std::string>( COMPONENTLIDER ) );
	SetValue<std::string>( LASTWORKER, pTech->GetValue<std::string>( LASTWORKER ) );
	SetValue<int>( CODEVOLUME, pTech->GetValue<int>( CODEVOLUME ) );
	SetValue<int>( CODEPASSED, pTech->GetValue<int>( CODEPASSED ) );
	SetValue<float>( READYWORKPERCENT, pTech->GetValue<float>( READYWORKPERCENT ) );
	SetValue<int>( ERRORNUMBER, pTech->GetValue<int>( ERRORNUMBER ) );
	assert( ptrCmp != NULL );
	if( ptrCmp != NULL )
	{
		SetValue<std::string>( COMPANYNAME, pTech->GetValue<std::string>( COMPANYNAME ) );
		SetValue<PNrpCompany>( PARENTCOMPANY, ptrCmp );
	}
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
	for( ; tIter != techRequires_.end(); tIter++ )
		IniFile::Write( "techRequire", IntToStr( tIter->first ), IntToStr( tIter->second ), fileName );

	REQUIRE_MAP::iterator sIter = skillRequires_.begin();
	for( ; sIter != skillRequires_.end(); sIter++ )
		IniFile::Write( "skillRequire", IntToStr( sIter->first ), IntToStr( sIter->second ), fileName );
}

void CNrpTechnology::Load( std::string fileName )
{
	INrpProject::Load( PROPERTIES, fileName );

	IniFile::ReadValueList_( "techRequire", techRequires_, fileName );
	IniFile::ReadValueList_( "skillRequire", skillRequires_, fileName );

	std::string liderName = GetValue<std::string>( COMPONENTLIDER );
	PNrpCompany ptrCompany = GetValue<PNrpCompany>( PARENTCOMPANY );
	if( !liderName.empty() )
	{
		PUser ptrUser = ptrCompany->GetUser( liderName );
		if( ptrUser )
			ptrUser->AddTechWork( this );
	}
}

float CNrpTechnology::GetEmployerPosibility()
{
	IUser* ptrUser = CNrpApplication::Instance().GetUser( GetValue<std::string>( COMPANYNAME ), GetValue<std::string>( COMPONENTLIDER ) );
	if( !ptrUser )
		return 0;
	
	int minSkill = 40;
	int minSkillName = 0;
	float posibility = 0;
	REQUIRE_MAP::iterator sIter = skillRequires_.begin();
	for( ; sIter != skillRequires_.end(); sIter++ )
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
	std::string currentLider = GetValue<std::string>( COMPONENTLIDER );
	std::string lastLider = GetValue<std::string>( LASTWORKER );
	std::string newUser = ptrUser != NULL ? ptrUser->GetValue<std::string>( NAME ) : "";

	//есть текущий исполнитель
	if( ptrUser == NULL && !currentLider.empty() )
	{
		SetValue<std::string>( LASTWORKER, GetValue<std::string>( COMPONENTLIDER ) );
		SetValue<std::string>( COMPONENTLIDER, "" );
		return;
	}

	if( currentLider != newUser )
	{
		SetValue<std::string>( LASTWORKER, currentLider );
		SetValue<int>( CODEPASSED,(int)(GetValue<int>( CODEPASSED ) * 0.75f) );
		SetValue<std::string>( COMPONENTLIDER, newUser );
	}

	GetValue<PNrpGameProject>( PARENT )->Update();
}

void CNrpTechnology::Update( IUser* ptrUser )
{
	if( GetValue<int>( CODEPASSED ) < GetValue<int>( CODEVOLUME) )
	{
		int reqSkill = 0;
		REQUIRE_MAP::iterator sIter = skillRequires_.begin();
		for( ; sIter != skillRequires_.end(); sIter++ )
			reqSkill += ptrUser->GetSkill( sIter->first );

		float genreSkill = ptrUser->GetGenreExperience( GetValue<int>( TECHTYPE ) ) / 100.f;
	
		if( genreSkill < 0.1f )
			genreSkill = 0.1f;
		float genrePref = ptrUser->GetGenrePreferences( GetValue<int>( TECHTYPE ) ) / 100.f;
		if( genrePref < 0.1f )
			genrePref = 0.1f;

		int codePassed = GetValue<int>( CODEPASSED ) + (int)(reqSkill * (genrePref + genreSkill));
		if( codePassed >= GetValue<int>( CODEVOLUME ) )
			codePassed = GetValue<int>( CODEVOLUME);

		SetValue<int>( CODEPASSED, codePassed );
		SetValue<float>( READYWORKPERCENT, codePassed / (float)GetValue<int>( CODEVOLUME ) );
	}
}
}//namespace nrp