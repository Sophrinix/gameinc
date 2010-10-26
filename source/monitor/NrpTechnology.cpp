#include "StdAfx.h"
#include "NrpTechnology.h"
#include "NrpGameProject.h"
#include "NrpInvention.h"
#include "IUser.h"
#include "NrpCompany.h"
#include "IniFile.h"
#include "OpFileSystem.h"

#include <errno.h>
#include <assert.h>

namespace nrp
{

CNrpTechnology::CNrpTechnology( PROJECT_TYPE typen, CLASS_NAME className ) : INrpProject( className, "" )
{
	InitializeOptions_();

	SetValue<int>( TECHGROUP, typen );
}

CNrpTechnology::CNrpTechnology( CNrpInvention* invention ) : INrpProject( CLASS_TECHNOLOGY, "" )
{
	InitializeOptions_();

	SetValue<TECH_STATUS>( STATUS, TS_READY );
	Load( invention->GetValue<std::string>( BASEFILE ) );
	SetValue<PNrpCompany>( PARENTCOMPANY, invention->GetValue<PNrpCompany>( PARENTCOMPANY ) );
	SetValue<SYSTEMTIME>( STARTDATE, invention->GetValue<SYSTEMTIME>( ENDDATE ) );
}

CNrpTechnology::CNrpTechnology( const std::string& fileTech ) : INrpProject( CLASS_TECHNOLOGY, "" )
{
	InitializeOptions_();

	Load( fileTech );
}

void CNrpTechnology::InitializeOptions_()
{
	CreateValue<std::string>( NAME, "" );
	CreateValue<std::string>( INTERNAL_NAME, "" );
	CreateValue<int>( TECHGROUP, 0 );
	CreateValue<int>( TECHTYPE, 0 );
	CreateValue<float>( BASE_CODE, 0 );
	CreateValue<float>( ENGINE_CODE, 0 );
	CreateValue<int>( LEVEL, 0 );
	CreateValue<std::string>( COMPANYNAME, "" );
	CreateValue<int>( QUALITY, 100 );
	CreateValue<PNrpCompany>( PARENTCOMPANY, NULL );
	CreateValue<std::string>( PARENT, "" );
	CreateValue<std::string>( BASEFILE, "" );
	CreateValue<SYSTEMTIME>( STARTDATE, SYSTEMTIME() );
	CreateValue<SYSTEMTIME>( ENDDATE, SYSTEMTIME() );
	CreateValue<float>( PRICE, 0 );
	CreateValue<std::string>( TEXTURENORMAL, "" );
	CreateValue<std::string>( TEXTUREHOVER, "" );
	CreateValue<std::string>( DESCRIPTIONPATH, "" );
	CreateValue<float>( INTEREST, 1 );
	CreateValue<TECH_STATUS>( STATUS, TS_UNKNOWN );

	CreateValue<int>( NEXTTECHNUMBER, 0 );
	CreateValue<PNrpTechnology>( REQUIRETECH, NULL );
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

std::string CNrpTechnology::Save( const std::string& saveFolder )
{
	OpFileSystem::CreateDirectory( saveFolder );

	std::string fileName = saveFolder + GetValue<std::string>( INTERNAL_NAME ) + ".tech";
	//не должно быть файла с такимже именем в директории
	assert( !OpFileSystem::IsExist( fileName ) );

	INrpProject::Save( fileName );
	SaveRequires_( fileName );

	for( size_t pos=0; pos < futureTech_.size(); pos++ )
		IniFile::Write( SECTION_FUTURE_TECH, "tech_" + IntToStr( pos ), futureTech_[ pos ], fileName );

	return fileName;
}

void CNrpTechnology::Load( const std::string& fileName )
{

	INrpProject::Load( fileName );
	SetValue<std::string>( BASEFILE, fileName );
	LoadRequries_( fileName );

	if( GetValue<TECH_STATUS>( STATUS ) == TS_READY )
		for( int cnt=0; cnt < GetValue<int>( NEXTTECHNUMBER ); cnt++ )
			futureTech_.push_back( IniFile::Read( SECTION_FUTURE_TECH, "tech_"+IntToStr( cnt ), std::string(""), fileName ) );
}

float CNrpTechnology::GetEmployerPosibility( IUser* ptrUser )
{
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

void CNrpTechnology::LoadRequries_( const std::string& fileName )
{
	IniFile::ReadValueList_( SECTION_REQUIRE_TECH, techRequires_, fileName );
	IniFile::ReadValueList_( SECTION_REQUIRE_SKILL, skillRequires_, fileName );
}

void CNrpTechnology::SaveRequires_( const std::string& fileName )
{
	for( REQUIRE_MAP::iterator tIter = techRequires_.begin(); tIter != techRequires_.end(); tIter++ )
		IniFile::Write( SECTION_REQUIRE_TECH, IntToStr( tIter->first ), IntToStr( tIter->second ), fileName );

	for( REQUIRE_MAP::iterator sIter = skillRequires_.begin(); sIter != skillRequires_.end(); sIter++ )
		IniFile::Write( SECTION_REQUIRE_SKILL, IntToStr( sIter->first ), IntToStr( sIter->second ), fileName );
}

const std::string CNrpTechnology::GetFutureTech( size_t index )
{
	return index < futureTech_.size() ? futureTech_[ index ] : "";
}

void CNrpTechnology::AddFutureTech( const std::string& name )
{
	futureTech_.push_back( name );
}
}//namespace nrp